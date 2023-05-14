#include "Space.h"



Space::Space() {
	lightColors.push_back(glm::vec3(1.f, 1.f, 1.f));
	lightDirs.push_back(glm::vec3(1.f, 0.f, 0.f));
	isDirectional.push_back(1);

	player = new Player(glm::vec3(0.f, 0.f, 0.f));

	glm::vec3 cameraTarget(0.f, 0.f, -2.f);
	glm::vec3 cameraDir(0.f, 0.f, 2.f);
	camera = new Camera(cameraDir, player);

	// coin stuff
	coinModel = new ModelObject("res/models/Coin.obj");
	coinShader = new Shader("res/shaders/Coin.shader");


	loadLevel1();
}

void Space::tick(float delta, ButtonMap bm) {
	player->tick(delta, bm);
	camera->updateTargetPos();
	float pitch = 0.0f;
	float yaw = 0.0f;

	if (bm.Up) {
		pitch -= 0.01f;
	}
	if (bm.Down) {
		pitch += 0.01f;
	}
	if (bm.Left) {
		yaw += 0.01f;
	}
	if (bm.Right) {
		yaw -= 0.01f;
	}

	camera->rotate(pitch, yaw);

	for (Coin* c : coins) {
		c->tick(delta);
	}
}


void Space::renderWorld() {
	glm::mat4 viewMatrix = camera->getLookAt();

	for (WorldObject* o : wObjects) {
		glm::mat4 mvp = proj * viewMatrix * o->getModelMatrix();
		o->draw(mvp);
	}
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), player->getPosition());
	glm::mat4 mvp = proj * camera->getLookAt() * modelMatrix;

	for (Coin* c : coins) {
		glm::mat4 mvp = proj * viewMatrix * c->getModelMatrix();
		c->render(mvp);
	}

	player->draw(mvp);
}

void Space::loadLevel1() {
	player->setPosition(glm::vec3(0.f, 1.f, 0.f));

	// load all the world objects and set up the world
	Shader* shader = new Shader("res/shaders/Basic.shader");

	ModelObject* m1 = new ModelObject(10.f, 10.f);
	WorldObject* obj1 = new WorldObject(shader, m1, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));

	wObjects.push_back(obj1);

	std::string testString = "res/models/teddy.obj";

	WorldObject* testobject = new WorldObject(shader, testString, glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0));
	wObjects.push_back(testobject);

	setUpCoinsLevel1();
}

void Space::addCoin(glm::vec3 pos) {
	Coin* c = new Coin(coinShader, coinModel, pos);
	coins.push_back(c);
}

void Space::setUpCoinsLevel1() {
	addCoin(glm::vec3(3, 3, 3));
	addCoin(glm::vec3(1, 2, 5));
	addCoin(glm::vec3(-5, 2, 10));
}


