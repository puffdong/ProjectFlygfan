#include "Camera.h"

Camera::Camera(glm::vec3 front, Player *p)
{
	player = p;
	mDir = glm::normalize(front);
	mUp = glm::vec3(0.f, 1.f, 0.f);
	mPitch = 0.f;
	pPlayerYaw = p->getYaw();
	mYaw = acos(glm::dot(mDir, mUp) / (glm::length(mUp) * glm::length(mDir)));
	updateTargetPos();
}

void Camera::updateTargetPos()
{
	mTargetPos = player->getPosition();
	pPlayerYaw = player->getYaw();
}

glm::mat4 Camera::getLookAt()
{
	glm::vec3 dir(0.f);
	dir.x = cos(mYaw - pPlayerYaw) * cos(mPitch);
	dir.y = sin(mPitch);
	dir.z = sin(mYaw - pPlayerYaw) * cos(mPitch);
	mDir = glm::normalize(dir);

	return glm::lookAt(mTargetPos - mDir * kTargetDistance, mTargetPos, mUp);
}

glm::vec3 Camera::getPosition()
{
	return mTargetPos - mDir * kTargetDistance;
}

void Camera::rotate(float pitchDiff, float yawDiff)
{
	pPlayerYaw = player->getYaw();
	mPitch += pitchDiff * kSensitivity;
	mYaw += yawDiff * kSensitivity;

	float angleLimit = (float)(M_PI / 2.f * 0.99f);
	if (mPitch > angleLimit)
		mPitch = angleLimit;
	else if (mPitch < -angleLimit)
		mPitch = -angleLimit;

	glm::vec3 dir(0.f);
	dir.x = cos(mYaw - pPlayerYaw) * cos(mPitch);
	dir.y = sin(mPitch);
	dir.z = sin(mYaw - pPlayerYaw) * cos(mPitch);
	mDir = glm::normalize(dir);
}

std::string Camera::toString()
{
	return "Target position: " + glm::to_string(mTargetPos) + ", Direction: " + glm::to_string(mDir) + ", Pitch: " + std::to_string(mPitch) + ", Yaw: " + std::to_string(mYaw);
}