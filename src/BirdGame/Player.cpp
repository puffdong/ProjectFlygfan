#include "Player.h"

Player::Player(glm::vec3 startPos)
	: model("res/models/lowpolybird.obj"), position(startPos), velocity(0.f, 0.f, 0.f),
	  isOnGround(true), pitch(0.f), yaw(0.f), numberOfCoins(0.f)
{
	shader = new Shader("res/shaders/Basic.shader");
}

void Player::tick(float delta, ButtonMap bm)
{
	move(bm);
}

void Player::move(ButtonMap bm)
{
	glm::vec3 movement = glm::vec3(0.f, 0.f, 0.f);

	if (bm.W)
	{
		movement = glm::vec3(glm::sin(yaw), 0, cos(yaw)) * 2.f;
	}

	if (bm.A)
	{
		yaw += 0.02f;
	}

	if (bm.D)
	{
		yaw -= 0.02f;
	}
	if (bm.Space)
	{
		movement.y += 1.f;
	}
	else if (bm.Ctrl)
	{
		movement.y -= 1.f;
	}
	else
	{
		movement.y -= 0.04f;
	}

	/*
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
	}*/

	position += movement * 0.02f;
}

void Player::draw(glm::mat4 mvp)
{
	shader->Bind();
	shader->SetUniform4f("u_Color", 1.0f, 0.f, 1.0f, 1.0f);
	shader->SetUniformMat4("u_MVP", mvp);
	model.render();
}

glm::vec3 Player::getPosition()
{
	return position;
}

glm::mat4 Player::getModelMatrix() {
	return glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), yaw, glm::vec3(0.f, 1.f, 0.f)) 
		* glm::scale(glm::mat4(1.f), glm::vec3(7.f));
}

float Player::getYaw()
{
	return yaw;
}

void Player::setPosition(glm::vec3 pos)
{
	position = pos;
}