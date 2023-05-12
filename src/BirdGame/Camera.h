#pragma once
#define _USE_MATH_DEFINES
#include <algorithm>
#include <string>
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

class Camera {
private:
	const float kSensitivity = 1.f;
	const float kTargetDistance = 5.f;

public:
	glm::vec3 mPos, mDir, mUp;
	float mPitch, mYaw;
	Camera(glm::vec3 pos, glm::vec3 front);

	glm::mat4 getLookAt();
	void rotate(float pitchDiff, float yawDiff);
	void updatePosition(glm::vec3 targetPos);
	std::string toString();
};