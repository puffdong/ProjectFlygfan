#pragma once
#include "glm/glm.hpp"

class Player {
private: 

public:
	glm::vec3 position;
	glm::vec3 velocity;
	bool isOnGround;

	float pitch; 
	float mYaw; 
	
	// Game logic related
	int numberOfCoins;
	
	Player();


};