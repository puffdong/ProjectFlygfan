#include "Camera.h"

Camera::Camera(glm::vec3 targetPos, glm::vec3 front)
{
	mTargetPos = targetPos;
	mDir = glm::normalize(front);
	mUp = glm::vec3(0.f, 1.f, 0.f);
	mPitch = 0.f;
	mYaw = acos(glm::dot(mDir, mUp) / (glm::length(mUp) * glm::length(mDir)));
	mYaw -= (float)(M_PI / 2.f);
}

void Camera::updateTargetPos(glm::vec3 targetPos)
{
	mTargetPos = targetPos;
}

glm::mat4 Camera::getLookAt()
{
	return glm::lookAt(mTargetPos - mDir * kTargetDistance, mTargetPos, mUp);
}

void Camera::rotate(float pitchDiff, float yawDiff)
{
	mPitch += pitchDiff * kSensitivity;
	mYaw += yawDiff * kSensitivity;

	float angleLimit = (float)(M_PI / 2.f * 0.99f);
	if (mPitch > angleLimit)
		mPitch = angleLimit;
	else if (mPitch < -angleLimit)
		mPitch = -angleLimit;

	glm::vec3 dir(0.f);
	dir.x = cos(mYaw) * cos(mPitch);
	dir.y = sin(mPitch);
	dir.z = sin(mYaw) * cos(mPitch);
	mDir = glm::normalize(dir);
}

std::string Camera::toString()
{
	return "Target position: " + glm::to_string(mTargetPos) + ", Direction: " + glm::to_string(mDir)
		+ ", Pitch: " + std::to_string(mPitch) + ", Yaw: " + std::to_string(mYaw);
}
