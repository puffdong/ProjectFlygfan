#include "Skybox.h"

Skybox::Skybox(const std::string& modelPath, const std::string& shaderPath, const std::string& texturePath)
	: model(modelPath), shader(shaderPath), texture(texturePath)
{
	texture.Bind(0);
}

void Skybox::draw(glm::mat4 projMatrix, Camera* camera)
{
	shader.Bind();
	shader.SetUniform1i("u_Texture", 0);
	glm::mat4 modelTrans = glm::translate(glm::mat4(1.f), camera->mTargetPos);
	glm::mat4 mvp = projMatrix * camera->getLookAt() * modelTrans;
	shader.SetUniformMat4("u_MVP", mvp);
	model.render();
}
