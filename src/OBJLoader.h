#pragma once
#include <GL/glew.h>

#include <string>

class ModelObject {
private:
	// Store the VAO and other related information in your ModelObject class, assuming you have the following member variables:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLsizei m_indexCount;

public:
	unsigned int numIndices;

	ModelObject(const std::string& filepath);
	ModelObject(float widht, float depth);
	~ModelObject();
	void loadThroughTiny(const std::string& filepath);

	void render();

	void Bind() const;
	void Unbind() const;
	void createFlatGround(float width, float depth);
};