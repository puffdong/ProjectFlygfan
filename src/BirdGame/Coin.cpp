#include "Coin.h"

#include <iostream>

Coin::Coin(Shader* s, ModelObject* m, glm::vec3 pos)
	: shader(s), model(m), position(pos), rotation(0.f), yaw(0.f), wobble(0.f), goingUpOrDown(0.3f)
{

}

Coin::~Coin() {

}

void Coin::tick(float delta) {
	yaw += 360.f * delta;
	Coin::wobble += goingUpOrDown;
	if (wobble > 1.5f) {
		goingUpOrDown = -0.3f;
	}
	if (wobble < -1.5) {
		goingUpOrDown = 0.3f;
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
	return glm::translate(glm::mat4(1.f), Coin::position + glm::vec3(0.f, Coin::wobble, 0.f)) * glm::rotate(glm::mat4(1.f), yaw * 5.f, glm::vec3(1.f, 0.f, 0.f));
}

void Coin::render(glm::mat4 mvp) {
	shader->Bind();
	shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	shader->SetUniformMat4("u_MVP", mvp);
	model->render();
}