#pragma once

#include <SFML/Graphics.hpp>

sf::View view;

void getPlayerCoordinateForView(float x, float y) 
{ //функци€ дл€ считывани€ координат игрока
	float tempX = x; float tempY = y;
	if (x < 380) tempX = 380;
	if (y > 810) tempY = 810;
	if (x > 1340) tempX = 1340;
	view.setCenter(tempX + 100, tempY);  //следим за игроком, передава€ его координаты камере.  
}
