#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../OBJLoader.h"

#include <string>
#include "../Shader.h"

class WorldObject {
private:
	Shader* shader;
	ModelObject* model;
	
	glm::vec3 position;
	glm::vec3 rotation;


public:
	WorldObject(Shader* s, std::string& modelPath, glm::vec3 pos, glm::vec3 rot);
	WorldObject(Shader* s, ModelObject* m, glm::vec3 pos, glm::vec3 rot);
	glm::vec3 getPosition();
	void draw(glm::mat4 mvp);
	glm::mat4 getModelMatrix();

	Shader* getShader();
};