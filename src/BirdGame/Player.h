#pragma once
#include "glm/glm.hpp"

#include "../Shader.h"
#include "../OBJLoader.h"
#include "Utility/ButtonMap.h"

class Player {
private: 
	ModelObject model;
	Shader* shader;

	void move(ButtonMap bm);
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

	void setPosition(glm::vec3 pos);
};
