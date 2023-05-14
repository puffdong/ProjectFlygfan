#pragma once

#include "../OBJLoader.h"

#include "glm/glm.hpp"
#include <vector>

#include "Coin.h"
#include "Player.h"
#include "WorldObject.h"

#include "Camera.h"

#include "Utility/ButtonMap.h"

class Space {
private:
	Player* player;
	Camera* camera;
	

	std::vector<WorldObject*> wObjects;

	glm::mat4 proj = glm::frustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 256.0f);
	
	glm::vec3 lightColors[1];
	glm::vec3 lightDirs[1];
	int isDirectional[1];

	float specularExponent = 100.f;
	
public:
	Space();

	void tick(float delta, ButtonMap bm);

	void renderWorld();


private:
	void loadLevel1();

	// Coin logic
	ModelObject* coinModel; // Use the same model data for every coin
	Shader* coinShader;
	std::vector<Coin*> coins;

	void addCoin(glm::vec3 pos);
	
	void setUpCoinsLevel1();
};