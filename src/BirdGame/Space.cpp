#include "Space.h"
#include <iostream>

Space::Space()
	: perlinNoise("res/textures/perlinnoise.tga")
{

	player = new Player(glm::vec3(0.f, 0.f, 0.f));

	glm::vec3 cameraDir(0.f, 0.f, 1.f);
	camera = new Camera(cameraDir, player);

	// coin stuff
	coinModel = new ModelObject("res/models/Coin.obj");
	coinShader = new Shader("res/shaders/Coin.shader");

	loadLevel1();
}

void Space::tick(float delta, ButtonMap bm)
{
	if (deathTimer > 1.f) {
		deathTimer -= delta;
	}
	else if (deathTimer > 0.f) {
		deathTimer -= delta;
		resetPlayer();
	}
	else {
		player->tick(delta, bm);
		glm::vec3 playerPos = player->getPosition();
		float groundHeightPlayer = ground->calcHeight(playerPos.x, playerPos.z);
		float distanceToGround = playerPos.y - groundHeightPlayer;
		if (groundHeightPlayer < -1.f || distanceToGround < 0.f) {
			deathTimer = 4.f;
		}
	}

	glm::vec3 cameraPos = camera->getPosition();
	float groundHeightCamera = ground->calcHeight(cameraPos.x, cameraPos.z);
	if (cameraPos.y - groundHeightCamera < 2.f) {
		bool succeeded;
		zoomOutTimer = 0.5f;
		// Zoom in until camera stops colliding
		do {
			succeeded = camera->zoomIn(0.1f);
			cameraPos = camera->getPosition();
			groundHeightCamera = ground->calcHeight(cameraPos.x, cameraPos.z);

		} while (succeeded && cameraPos.y - groundHeightCamera < 0.f);
	}
	else if (zoomOutTimer < 0.f) {
		// Try to zoom out if the timer has reached 0
		camera->zoomOut(0.1f);
	}
	else {
		zoomOutTimer -= delta;
	}

	camera->tick(delta, bm);

	std::vector<Coin*> coinDeletionQueue;

	for (Coin *c : coins) {
		c->tick(delta);
		if (c->deleteMe) {
			coinDeletionQueue.push_back(c);
			player->numberOfCoins++;
		}
	}

	for (Coin* c : coinDeletionQueue) {
		auto iter = std::find(coins.begin(), coins.end(), c);

		if (iter != coins.end()) {
			// Erase the coin pointer from the coins vector
			coins.erase(iter);

			// Delete the coin object
			delete c;
		}
	}
}

void Space::resetPlayer()
{
	player->setPosition(playerStartPos);
	player->yaw = 0.f;
	player->pitch = 0.f;
	player->roll = 0.f;
}

void Space::renderWorld(float delta)
{
	glm::mat4 viewMatrix = camera->getLookAt();
	skybox->draw(proj, camera);

	ground->draw(proj, viewMatrix);

	for (WorldObject *o : wObjects)
	{
		o->draw(proj, viewMatrix, o->getModelMatrix());
	}

	for (Coin *c : coins)
	{
		c->draw(proj, viewMatrix, c->getModelMatrix());
	}

	player->draw(proj, viewMatrix, player->getModelMatrix());

	//render water
	waterTime += delta * 0.01f;
	waterShader->Bind();
	perlinNoise.Bind(1);
	waterShader->SetUniform1i("perlinNoise", 1);
	waterShader->SetUniform1f("time", waterTime);
	waterShader->SetUniform3f("sunDir", glm::vec3(1.f, 0.5f, 0.f));
	waterShader->SetUniform3f("sunColor", glm::vec3(1.f, 1.f, 1.f));
	waterShader->SetUniform3f("playerPosition", player->getPosition());
	waterShader->SetUniformMat4("proj", proj);
	waterObject->draw(proj, viewMatrix, waterObject->getModelMatrix());

}

void Space::loadLevel1()
{
	skybox = new Skybox(
		std::string("res/models/skybox-full-tweaked.obj"),
		std::string("res/shaders/Skybox.shader"),
		std::string("res/textures/skybox/cloud-landscape.tga")
	);

	playerStartPos = glm::vec3(10.f, 10.f, 0.f);
	player->setPosition(playerStartPos);

	// Setup shader with lighting
	Shader *worldShader = new Shader("res/shaders/WorldObject.shader");
	LightSource newLightSources[] = {
//		LightSource(glm::vec3(0.f, 0.5f, 0.f), glm::vec3(-1.f, 0.f, 0.f), true),
		LightSource(glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 0.f), true)
	};

	std::vector<glm::vec3> lightColors;
	std::vector<glm::vec3> lightDirs;
	std::vector<int> isDirectional;
	for (LightSource& light : newLightSources)
	{
		lightColors.push_back(light.color);
		lightDirs.push_back(light.dir);
		isDirectional.push_back((int)light.isDirectional);
	}
	worldShader->Bind();
	worldShader->SetUniform1i("numLights", lightColors.size());
	worldShader->SetUniform3fv("lightColors", lightColors);
	worldShader->SetUniform3fv("lightDirs", lightDirs);
	worldShader->SetUniform1iv("isDirectional", isDirectional);

	// load all the world objects and set up the world
	WorldObject *teapotObject = new WorldObject(worldShader, "res/models/teapot.obj", glm::vec3(-10.f, 0.f, 0.f), glm::vec3(0.f));
	wObjects.push_back(teapotObject);

	/*Shader* groundShader = new Shader("res/shader/Ground.shader");
	groundShader->Bind();
	groundShader->SetUniform1i("numLights", lightColors.size());
	groundShader->SetUniform3fv("lightColors", lightColors);
	groundShader->SetUniform3fv("lightDirs", lightDirs);
	groundShader->SetUniform1iv("isDirectional", isDirectional);*/
	glm::vec3 groundDims(100.f, 20.f, 100.f);
	glm::mat4 groundTrans = glm::translate(glm::mat4(1.f), glm::vec3(0.f));
	ground = new Ground(
		groundDims, groundTrans, "res/textures/terrain/fft-terrain.tga", worldShader, "res/textures/grass.tga"
	);


	//water stuff
	waterShader = new Shader("res/shaders/Water.shader");
	waterModel = new ModelObject(30.f, 30.f, 60, 60);
	waterObject = new WorldObject(waterShader, waterModel, glm::vec3(20.f, 10.f, 5.f), glm::vec3(0.f));
	waterTime = 0.0f;

	setUpCoinsLevel1();
}

void Space::addCoin(glm::vec3 pos)
{
	Coin *c = new Coin(coinShader, coinModel, pos, player);
	coins.push_back(c);
}

void Space::setUpCoinsLevel1()
{
	addCoin(glm::vec3(3, 3, 3));
	addCoin(glm::vec3(1, 2, 5));
	addCoin(glm::vec3(-5, 2, 10));
}
