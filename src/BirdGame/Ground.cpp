#include "Ground.h"
#include "glm/gtc/matrix_transform.hpp"

Ground::Ground(
	glm::vec3 dims,
	glm::mat4 translation,
	const std::string& mapTexturePath, 
	Shader* shader, 
	const std::string& graphicTexturePath
) 
	: mWidth(dims.x), mDepth(dims.z), mHeight(dims.y), mTranslation(translation), shader(shader)
{
	mapTexture = new Texture(mapTexturePath);
	graphicTexture = new Texture(graphicTexturePath);

	model = new ModelObject(mWidth, mDepth, mHeight, &mapTexture->tgaData);
}

void Ground::draw(glm::mat4 projMatrix, glm::mat4 worldMatrix) {
	
	shader->Bind();
	shader->SetUniformMat4("u_MVP", projMatrix * worldMatrix * mTranslation);
	shader->SetUniformMat4("modelMatrix", mTranslation);
	shader->SetUniformMat4("worldMatrix", worldMatrix);
	graphicTexture->Bind(0);
	shader->SetUniform1i("u_Texture", 0);
	model->render();
}

float Ground::calcHeight(float x, float z) {
	glm::vec3 pos = glm::vec3(mTranslation * glm::vec4(x, 0, z, 1.f));

    if (x < 0 || z < 0) {
        return -1.0F;
    }
    int mapWidth = mapTexture->GetWidth();
    int mapDepth = mapTexture->GetHeight();
    std::vector<glm::vec3> vertexArray = model->getVertexArray();
    GLfloat xStep = vertexArray[1].x - vertexArray[0].x;
    GLfloat zStep = vertexArray[mapWidth].z - vertexArray[0].z;


    GLint xIndex = -1;
    GLint zIndex = -1;
    unsigned int i;
    GLfloat xVal, zVal;
    for (i = 0; i < mapWidth; i++)
    {
        xVal = vertexArray[i].x;
        if (x <= xVal) {
            xIndex = i;
            break;
        }
    }
    for (i = 0; i <= mapDepth; i++)
    {
        zVal = vertexArray[(i * mapWidth)].z;
        if (z < zVal) {
            zIndex = i;
            break;
        }
    }

    if (xIndex == -1 || zIndex == -1)
    {
        printf("calcHeight: Coords not found");
        return -1.0f;
    }

    glm::vec3 p1 = vertexArray[(xIndex - 1 + (zIndex * mapWidth))];
    glm::vec3 p2 = vertexArray[(xIndex + ((zIndex - 1) * mapWidth))];
    glm::vec3 p3;
    if (x < xVal - xStep / 2 && z < zVal - zStep / 2)
    {
        p3 = vertexArray[(xIndex - 1 + ((zIndex - 1) * mapWidth))];
    }
    else
    {
        p3 = vertexArray[(xIndex + (zIndex * mapWidth))];
    }


    GLfloat a = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
    GLfloat b = (p2.z - p1.z) * (p3.x - p1.x) - (p3.z - p1.z) * (p2.x - p1.x);
    GLfloat c = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);
    GLfloat d = -(a * p1.x + b * p1.y + c * p1.z);

    GLfloat y = -(d + a * x + c * z) / b;
    return y;
}