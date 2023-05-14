#include "WorldObject.h"

WorldObject::WorldObject(Shader* s, std::string& modelPath, glm::vec3 pos, glm::vec3 rot) 
{
	model = new ModelObject(modelPath);
	shader = s;
	position = pos;
	rotation = rot;
}

WorldObject::WorldObject(Shader* s, ModelObject* m, glm::vec3 pos, glm::vec3 rot) {
	model = m;
	shader = s;
	position = pos;
	rotation = rot;
}

void WorldObject::draw(glm::mat4 mvp) {
	shader->Bind();
	shader->SetUniform4f("u_Color", 1.0f, 1.0f, 0.8f, 1.0f);
	shader->SetUniformMat4("u_MVP", mvp);
	model->render();
}

glm::vec3 WorldObject::getPosition() {
	return position;
}

glm::mat4 WorldObject::getModelMatrix() {
	return glm::translate(glm::mat4(1.f), position);
}

Shader* WorldObject::getShader() {
	return shader;
}