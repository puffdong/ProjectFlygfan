#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../Shader.h"
#include "../OBJLoader.h"
#include "../Texture.h"
#include "Utility/ButtonMap.h"

class Player
{
private:
	ModelObject model;
	Shader *shader;
	Texture texture;
	const float kMoveSpeed = 5.f;
	const float kPitchSpeed = 1.f;
	const float kYawSpeed = 0.5f;
	const float kRollSpeed = 1.f;

	void move(ButtonMap bm, float delta);

public:
	glm::vec3 position;
	glm::vec3 velocity;
	bool isOnGround;

	float pitch;
	float yaw;
	float roll;

	// Game logic related
	int numberOfCoins;

	Player(glm::vec3 startPos);
	void tick(float delta, ButtonMap bm);
	void draw(glm::mat4 proj, glm::mat4 view, glm::mat4 modelM);
	glm::vec3 getPosition();
	glm::mat4 getModelMatrix();
	float getYaw();

	void setPosition(glm::vec3 pos);
};
