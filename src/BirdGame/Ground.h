#pragma once
#include "../OBJLoader.h"
#include "../Shader.h"
#include "../Texture.h"

class Ground
{
	ModelObject* model;
	Shader* shader;
	Texture* mapTexture;
	Texture* graphicTexture;

	float mWidth;
	float mDepth;
	float mHeight;

	glm::mat4 mTranslation;
public:
	Ground(glm::vec3 dims, glm::mat4 translation, const std::string& mapTexturePath, Shader* shader, const std::string& graphicTexturePath);
	void draw(glm::mat4 projMatrix, glm::mat4 worldMatrix);

	float calcHeight(float x, float z);
};

