#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

#include "Player.h"

class Camera {
private:
	const float kSensitivity = 1.f;
	const float kTargetDistance = 5.f;

public:
	Player* player;
	glm::vec3 mTargetPos, mDir, mUp;
	float mPitch, mYaw;
	Camera(glm::vec3 front, Player* p);

	glm::mat4 getLookAt();
	void rotate(float pitchDiff, float yawDiff);
	void updateTargetPos();
	std::string toString();
};
