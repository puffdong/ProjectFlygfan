#include "Player.h"

Player::Player(glm::vec3 startPos) 
	: model("res/models/bunny.obj"), position(startPos), velocity(0.f, 0.f, 0.f), 
	isOnGround(true), pitch(0.f), yaw(0.f), numberOfCoins(0.f) 
{
	shader = new Shader("res/shaders/Basic.shader");
}

void Player::tick(float delta, ButtonMap bm) {
	move(bm);
}

void Player::move(ButtonMap bm) {
	glm::vec3 movement = glm::vec3(0.f, 0.f, 0.f);

	if (bm.W) {
		movement.x += 0.01f;
	}
	if (bm.A) {
		movement.z += 0.01f;
	}
	if (bm.S) {
		movement.x -= 0.01f;
	}
	if (bm.D) {
		movement.z -= 0.01f;
	}
	if (bm.Space) {
		movement.y += 0.01f;
	}

	if (movement != glm::vec3(0)) {
		movement = glm::normalize(movement);
	}
	
	position += movement * 0.01f;
}

void Player::draw(glm::mat4 mvp) {
	shader->Bind();
	shader->SetUniform4f("u_Color", 1.0f, 0.f, 1.0f, 1.0f);
	shader->SetUniformMat4("u_MVP", mvp);
	model.render();
}

glm::vec3 Player::getPosition() {
	return position;
}

void Player::setPosition(glm::vec3 pos) {
	position = pos;
}