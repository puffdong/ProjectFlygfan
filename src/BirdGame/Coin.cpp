#include "Coin.h"

#include <iostream>

Coin::Coin(Shader* s, ModelObject* m, glm::vec3 pos)
	: shader(s), model(m), position(pos), rotation(0.f)
{
	yaw = 0.f;
	wobble = 0.f;
	goingUpOrDown = 0.05f;
}

Coin::~Coin() {

}

void Coin::tick(float delta) {
	yaw += 1.5f * delta;
	std::cout << delta << " : delta " << std::endl;
	std::cout << yaw << std::endl;
	Coin::wobble += goingUpOrDown;
	if (wobble > 0.8f) {
		goingUpOrDown = -0.05f;
	}
	if (wobble < -0.8f) {
		goingUpOrDown = 0.05f;
	}
}

bool Coin::isWithingCollectionRange(glm::vec3 pos1, glm::vec3 pos2) {
	float distance = glm::distance(pos1, pos2);
	std::cout << distance << std::endl;
	if (distance <= 5.f) {
		return true;
	}
	else {
		return false;
	}
}

glm::mat4 Coin::getModelMatrix() {
	return glm::translate(glm::mat4(1.f), position + glm::vec3(0.f, wobble, 0.f)) * glm::rotate(glm::mat4(1.f), yaw, glm::vec3(0.f, 1.f, 0.f));
}

void Coin::render(glm::mat4 mvp) {
	shader->Bind();
	shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	shader->SetUniformMat4("u_MVP", mvp);
	model->render();
}