#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../Shader.h"
#include "../OBJLoader.h"

class Coin {
private:
	ModelObject* model;
	Shader* shader;
	//Texture* texture;

	

public:
	glm::vec3 position;
	glm::vec3 rotation;

	float yaw;
	float wobble;
	float goingUpOrDown;

	Coin(Shader* s, ModelObject* m, glm::vec3 pos);
	~Coin();
	void tick(float delta);
	bool isWithingCollectionRange(glm::vec3 pos1, glm::vec3 pos2);

	glm::mat4 getModelMatrix();
	void render(glm::mat4 mvp);

};