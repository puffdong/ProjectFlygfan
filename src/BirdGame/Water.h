#pragma once
#include "WorldObject.h"
#include "Player.h"

class WaterSurface : public WorldObject {
private:
	//water stuff
	Texture* perlinNoise;
	float waterTime;

	Player* player;

public:
	WaterSurface(Shader* s, const std::string& modelPath, glm::vec3 pos, glm::vec3 rot, Texture* perlin, Player* p);
	WaterSurface(Shader* s, ModelObject* m, glm::vec3 pos, glm::vec3 rot, Texture* perlin, Player* p);
	void tick(float deltaTime) override;
	void draw(glm::mat4 projMatrix, glm::mat4 worldMatrix, glm::mat4 modelMatrix) override;
};