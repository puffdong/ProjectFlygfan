#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

#include "Player.h"

class Camera
{
private:
	const float kSensitivity = 1.f;
	float kTargetDistance = 5.f;

public:
	Player *player;
	glm::vec3 mTargetPos, mDir, mUp;
	float mPitch, mYaw, mCurrentDistance, pPlayerYaw;
	Camera(glm::vec3 front, Player *p);

	glm::mat4 getLookAt();
	glm::vec3 getPosition();
	void rotate(float pitchDiff, float yawDiff);
	void tick(float delta, ButtonMap bm);

	bool zoomIn(float diff);
	bool zoomOut(float diff);

	std::string toString();

private:
	void updateTargetPos();
};
