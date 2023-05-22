#include "Water.h"
#include <iostream>

WaterSurface::WaterSurface(Shader* s, const std::string& modelPath, glm::vec3 pos, glm::vec3 rot, Texture* perlin, Player* p)
: WorldObject(s, modelPath, pos, rot) {
	waterTime = 0.0f;
	perlinNoise = perlin;
	player = p;
}

WaterSurface::WaterSurface(Shader* s, ModelObject* m, glm::vec3 pos, glm::vec3 rot, Texture* perlin, Player* p)
	: WorldObject(s, m, pos, rot) {
	waterTime = 0.0f;
	perlinNoise = perlin;
	player = p;
}

void WaterSurface::tick(float deltaTime) {
	waterTime += deltaTime * 0.02f;
}

void WaterSurface::draw(glm::mat4 projMatrix, glm::mat4 worldMatrix, glm::mat4 modelMatrix) {
	shader->Bind();
	perlinNoise->Bind(1);
	shader->SetUniform1i("perlinNoise", 1);
	shader->SetUniform1f("time", waterTime);
	shader->SetUniform3f("sunDir", glm::vec3(1.f, 0.5f, 0.f));
	shader->SetUniform3f("sunColor", glm::vec3(1.f, 1.f, 1.f));
	shader->SetUniform3f("playerPosition", player->getPosition());

	shader->SetUniformMat4("modelMatrix", modelMatrix);
	shader->SetUniformMat4("worldMatrix", worldMatrix);
	shader->SetUniformMat4("u_MVP", projMatrix * worldMatrix * modelMatrix);
	shader->SetUniformMat4("proj", projMatrix);
	model->render();
}