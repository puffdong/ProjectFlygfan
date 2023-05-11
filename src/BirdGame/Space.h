#pragma once


#include "glm/glm.hpp"
#include <vector>

#include "Coin.h"
#include "Player.h"

class Space {
private:
	Player player;
	std::vector<Coin> coins;
	
public:
	Space();

	void tick(double delta);

};