#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../Shader.h"
#include "../OBJLoader.h"
#include "Utility/ButtonMap.h"

class Player
{
private:
	ModelObject model;
	Shader *shader;
	const float moveSpeed = 1.f;
	const float kPitchSpeed = 1.f;
	const float kYawSpeed = 1.f;

	void move(ButtonMap bm, float delta);

public:
	glm::vec3 position;
	glm::vec3 velocity;
	bool isOnGround;

	float pitch;
	float yaw;

	// Game logic related
	int numberOfCoins;

	Player(glm::vec3 startPos);
	void tick(float delta, ButtonMap bm);
	void draw(glm::mat4 mvp);
	glm::vec3 getPosition();
	glm::mat4 getModelMatrix();
	float getYaw();

	void setPosition(glm::vec3 pos);
};
