#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Player.h"
#include "../Texture.h"
#include "../Shader.h"
#include "../OBJLoader.h"

class Coin {
private:
	ModelObject* model;
	Shader* shader;
	Texture texture;
	Player* player;

public:
	glm::vec3 position;
	glm::vec3 rotation;

	float yaw;
	float wobble;
	float goingUpOrDown;

	// Variables for when getting picked up :)
	bool gotPickedUp;
	float t; //interpolation value
	float scale; // shrinks upon getting picked up
	glm::vec3 originalPosition;


	bool deleteMe;

	Coin(Shader* s, ModelObject* m, glm::vec3 pos, Player* p);
	~Coin();
	void tick(float delta);

	glm::mat4 getModelMatrix();
	glm::vec3 getPosition();
	void draw(glm::mat4 proj, glm::mat4 view, glm::mat4 modelM);

private:
	bool isWithinCollectionRange();
	void interpolateTowardPlayer(float delta);
	float smoothstep(glm::vec3 edge0, glm::vec3 edge1, float x);
};