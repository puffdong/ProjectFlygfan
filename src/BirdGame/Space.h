#pragma once

#include "../OBJLoader.h"
#include "../Texture.h"

#include "glm/glm.hpp"
#include <vector>

#include "Camera.h"
#include "Coin.h"
#include "Player.h"
#include "WorldObject.h"
#include "Skybox.h"
#include "Ground.h"


#include "Utility/ButtonMap.h"
#include "Utility/LightSource.h"

class Space {
private:
	Player* player;
	Camera* camera;
	Skybox* skybox;
	glm::vec3 playerStartPos;

	std::vector<WorldObject*> wObjects;

	//glm::mat4 proj = glm::frustum(-0.5f * 16.f/9.0f, 0.5f * 16.f / 9.0f, -0.5f, 0.5f, 1.0f, 256.0f);
	
	glm::mat4 proj = glm::perspective(glm::radians(70.f), 16.f / 9.0f, 1.0f, 256.0f);

	float zoomOutTimer = -1.f;
	
public:
	Space();

	void resetPlayer();

	void tick(float delta, ButtonMap bm);

	void renderWorld();


private:
	void loadLevel1();

	// Coin logic
	ModelObject* coinModel; // Use the same model data for every coin
	Shader* coinShader;
	Ground* ground;

	std::vector<Coin*> coins;

	void addCoin(glm::vec3 pos);
	
	void setUpCoinsLevel1();
};