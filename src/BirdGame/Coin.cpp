#include "Coin.h"


#include <iostream>

Coin::Coin(Shader* s, ModelObject* m, glm::vec3 pos, Player* p)
	: shader(s), model(m), position(pos), player(p), rotation(0.f), texture("res/textures/coin/coin_texture.tga")
{
	yaw = 0.f;
	wobble = 0.f;
	goingUpOrDown = 0.05f;
	gotPickedUp = false;
	t = 0.0f;
	scale = 0.65f;
	deleteMe = false;
}

Coin::~Coin() {
	//Don't do anything out of the ordinary huh
}

void Coin::tick(float delta) {
	yaw += 1.5f * delta;
	Coin::wobble += goingUpOrDown;
	if (wobble > 0.8f) {
		goingUpOrDown = -0.05f;
	}
	if (wobble < -0.8f) {
		goingUpOrDown = 0.05f;
	}

	if (isWithinCollectionRange() && !gotPickedUp) {
		gotPickedUp = true;
		goingUpOrDown = 0.0f; // stop wobbling
		t = 0.0f; 
		originalPosition = glm::vec3(position);
	}
	if (gotPickedUp) {
		interpolateTowardPlayer(delta);
		if (t > 1.0f) {
			deleteMe = true; // a sign for Space.h that it is picked up and collected
		}
	}
}

bool Coin::isWithinCollectionRange() {
	float range = 4.0f;
	float distance = glm::distance(position, player->getPosition());
	if (distance <= range) {
		return true;
	}
	else {
		return false;
	}
}

glm::mat4 Coin::getModelMatrix() {
	return glm::translate(glm::mat4(1.f), position + glm::vec3(0.f, wobble, 0.f)) * glm::rotate(glm::mat4(1.f), yaw, glm::vec3(0.f, 1.f, 0.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(scale));
}

glm::vec3 Coin::getPosition() {
	return position;
}

void Coin::draw(glm::mat4 proj, glm::mat4 view, glm::mat4 modelM) {
	shader->Bind();
	texture.Bind(0);
	shader->SetUniform1i("u_Texture", 0);

	shader->SetUniformMat4("proj", proj);
	shader->SetUniformMat4("view", view);
	shader->SetUniformMat4("model", modelM);

	shader->SetUniform3f("sunDir", glm::vec3(1.f, 0.5f, 0.f));
	shader->SetUniform3f("sunColor", glm::vec3(1.f, 1.f, 1.f));

	shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	model->render();
}

void Coin::interpolateTowardPlayer(float delta) {
	t += 0.8 * delta;
	scale -= 0.4 * delta;
	float weight = smoothstep(glm::vec3(0.0f), glm::vec3(1.0f), t);
	position = originalPosition + weight * (player->getPosition() - originalPosition);
}

float Coin::smoothstep(glm::vec3 edge0, glm::vec3 edge1, float x) {
	// Clamp x between 0 and 1
	float te = glm::clamp((x - edge0.x) / (edge1.x - edge0.x), 0.0f, 1.0f);

	// Smoothstep function
	return te * te * (3.0f - 2.0f * te);
}