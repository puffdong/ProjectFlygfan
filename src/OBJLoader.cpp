#define TINYOBJLOADER_IMPLEMENTATION
#include "OBJloader.h"
#include "Renderer.h"
#include "tiny_obj_loader.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <tuple>

ModelObject::ModelObject(const std::string& filepath) {
    loadThroughTiny(filepath);
}

ModelObject::ModelObject(float width, float depth) {
    createFlatGround(width, depth);
}

ModelObject::~ModelObject() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void ModelObject::loadThroughTiny(const std::string& filepath) {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filepath, reader_config)) {
        if (!reader.Error().empty()) {
            std::cout << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    struct TupleHasher {
        std::size_t operator()(const std::tuple<int, int, int>& tuple) const {
            std::size_t h1 = std::hash<int>{}(std::get<0>(tuple));
            std::size_t h2 = std::hash<int>{}(std::get<1>(tuple));
            std::size_t h3 = std::hash<int>{}(std::get<2>(tuple));

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    // Create data arrays
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    std::unordered_map<std::tuple<int, int, int>, unsigned int, TupleHasher> vertex_map;


    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                auto key = std::make_tuple(idx.vertex_index, idx.normal_index, idx.texcoord_index);

                // Check if the vertex has already been added to the vertex_map
                if (vertex_map.find(key) == vertex_map.end()) {
                    // Add the vertex to the vertex_map and vertices vector
                    unsigned int newIndex = static_cast<unsigned int>(vertices.size() / 8);
                    vertex_map[key] = newIndex;

                    // Vertex position
                    vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 0]);
                    vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 1]);
                    vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 2]);

                    // Vertex normal
                    if (idx.normal_index >= 0) {
                        vertices.push_back(attrib.normals[3 * size_t(idx.normal_index) + 0]);
                        vertices.push_back(attrib.normals[3 * size_t(idx.normal_index) + 1]);
                        vertices.push_back(attrib.normals[3 * size_t(idx.normal_index) + 2]);
                    }
                    else {
                        vertices.push_back(0.0f);
                        vertices.push_back(0.0f);
                        vertices.push_back(0.0f);
                    }

                    // Vertex texture coordinates
                    if (idx.texcoord_index >= 0) {
                        vertices.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]);
                        vertices.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]);
                    }
                    else {
                        vertices.push_back(0.0f);
                        vertices.push_back(0.0f);
                    }
                }

                // Add the index to the indices vector
                indices.push_back(vertex_map[key]);
            }

            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind and populate VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    GLsizei stride = 8 * sizeof(float); // 3 for position, 3 for normals, and 2 for texture coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Store the VAO and other related information in your ModelObject class, assuming you have the following member variables:
    // GLuint m_VAO;
    // GLuint m_VBO;
    // GLuint m_EBO;
    // GLsizei m_indexCount;
    m_VAO = VAO;
    m_VBO = VBO;
    m_EBO = EBO;
    m_indexCount = static_cast<GLsizei>(indices.size());
}

void ModelObject::render() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}



void ModelObject::Bind() const {
	
}

void ModelObject::Unbind() const {

}

void ModelObject::createFlatGround(float width, float depth) {
    // Create vertex data
    std::vector<float> vertices = {
        // Positions        // Normals         // Texture Coords
        -width / 2.0f, 0.0f, -depth / 2.0f,     0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
         width / 2.0f, 0.0f, -depth / 2.0f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
         width / 2.0f, 0.0f,  depth / 2.0f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
        -width / 2.0f, 0.0f,  depth / 2.0f,     0.0f, 1.0f, 0.0f,     0.0f, 1.0f
    };

    // Create index data
    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    // ... (The existing code to generate VAO, VBO, and EBO remains the same)

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind and populate VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    GLsizei stride = 8 * sizeof(float); // 3 for position, 3 for normals, and 2 for texture coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Store the VAO and other related information in your ModelObject class, assuming you have the following member variables:
    // GLuint m_VAO;
    // GLuint m_VBO;
    // GLuint m_EBO;
    // GLsizei m_indexCount;
    m_VAO = VAO;
    m_VBO = VBO;
    m_EBO = EBO;
    m_indexCount = static_cast<GLsizei>(indices.size());
}