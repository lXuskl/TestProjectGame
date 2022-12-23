#pragma once
void menu(sf::RenderWindow& window) {
	sf::Texture menuTexture1, menuTexture2, menuTexture3, menuTexture4, aboutTexture, menuBackground, instructionTexture;
	menuTexture1.loadFromFile("Texture/Menu/111.png");
	menuTexture2.loadFromFile("Texture/Menu/222.png");
	menuTexture3.loadFromFile("Texture/Menu/444.png");
	menuTexture4.loadFromFile("Texture/Menu/333.png");
	aboutTexture.loadFromFile("Texture/Menu/about.png");
	menuBackground.loadFromFile("Texture/Menu/Fon.png");
	instructionTexture.loadFromFile("Texture/Menu/Instruction.png");
	sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3),menu4(menuTexture4), about(aboutTexture), menuBg(menuBackground),instruction(instructionTexture);
	bool isMenu = 1;
	int menuNum = 0;
	menuBg.setPosition(0, 0);
	menu1.setPosition(1500, 30);
	menu2.setPosition(1500, 90);
	menu3.setPosition(1500, 150);
	menu4.setPosition(1500, 210);
	

	////////////////////////////// Меню \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	while (isMenu)
	{
		menu1.setColor(sf::Color::White);
		menu2.setColor(sf::Color::White);
		menu3.setColor(sf::Color::White);
		menu4.setColor(sf::Color::White);
		menuNum = 0;
		

		if (sf::IntRect(1500, 30, 300, 50).contains(sf::Mouse::getPosition(window))) { menu1.setColor(sf::Color::Blue); menuNum = 1; }
		if (sf::IntRect(1500, 90, 300, 50).contains(sf::Mouse::getPosition(window))) { menu2.setColor(sf::Color::Blue); menuNum = 2; }
		if (sf::IntRect(1500, 150, 300, 50).contains(sf::Mouse::getPosition(window))) { menu3.setColor(sf::Color::Blue); menuNum = 3; }
		if (sf::IntRect(1500, 210, 300, 50).contains(sf::Mouse::getPosition(window))) { menu4.setColor(sf::Color::Blue); menuNum = 4; }

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) { window.draw(about); window.display(); while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)); }
			if (menuNum == 3) { window.draw(instruction); window.display(); while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));}
			if (menuNum == 4) { window.close(); isMenu = false; }

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.draw(menu4);

		window.display();
	}
	
	
}