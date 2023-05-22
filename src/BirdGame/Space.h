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
#include "Water.h"


#include "Utility/ButtonMap.h"
#include "Utility/LightSource.h"

class Space {
private:
	Player* player;
	glm::vec3 playerStartPos;
	float zoomOutTimer = -1.f;
	float deathTimer = -1.f;

	std::vector<WorldObject*> wObjects;
	std::vector<WorldObject*> wTransparentObjects;
	Skybox* skybox;
	
	glm::mat4 proj = glm::perspective(glm::radians(70.f), 16.f / 9.0f, 1.0f, 256.0f);
	Camera* camera;
	
public:
	Space();

	void resetPlayer();

	void tick(float delta, ButtonMap bm);

	void renderWorld(float delta);

private:
	void loadLevel1();

	// Coin logic
	ModelObject* coinModel; // Use the same model data for every coin
	Shader* coinShader;
	Ground* ground;

	std::vector<Coin*> coins;

	void addCoin(glm::vec3 pos);
	
	void setUpCoinsLevel1();


	//water stuff
	Shader* waterShader;
	ModelObject* waterModel;
	Texture perlinNoise;
	WorldObject* waterObject;
	float waterTime;
};