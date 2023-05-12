#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 front)
{
	mPos = pos;
	mDir = glm::normalize(front);
	mUp = glm::vec3(0.f, 1.f, 0.f);
	mPitch = mYaw = 0.f;
}

void Camera::updatePosition(glm::vec3 targetPos)
{
	mPos = targetPos - mDir * kTargetDistance;
}

glm::mat4 Camera::getLookAt()
{
	return glm::lookAt(mPos, mPos + mDir, mUp);
}

void Camera::rotate(float pitchDiff, float yawDiff)
{
	mPitch += pitchDiff * kSensitivity;
	mYaw += yawDiff * kSensitivity;
	
	if (mPitch > M_PI / 2.f * 0.99f)
		mPitch = M_PI / 2.f * 0.99;
	else if (mPitch < -M_PI / 2.f * 0.99f)
		mPitch = -M_PI / 2.f * 0.99f;

	glm::vec3 dir(0.f);
	dir.x = cos(mYaw) * cos(mPitch);
	dir.y = sin(mPitch);
	dir.z = sin(mYaw) * cos(mPitch);
	
	mDir = glm::normalize(dir);
}

std::string Camera::toString()
{
	return "Position: " + glm::to_string(mPos) + ", Front: " + glm::to_string(mDir)
		+ ", Pitch: " + std::to_string(mPitch) + ", Yaw: " + std::to_string(mYaw);
}
