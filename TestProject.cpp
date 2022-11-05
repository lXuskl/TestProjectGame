#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1980, 1080), "Работай блин"/*, sf::Style::Fullscreen */ ); //создание окна
	
    sf::Image heroimage;
	heroimage.loadFromFile("Текстуры/Герой/hero.png");

	sf::Texture herotexture;
	herotexture.loadFromImage(heroimage);

	sf::Sprite herosprite;
	herosprite.setTexture(herotexture);
	herosprite.setTextureRect(IntRect());
	herosprite.setPosition(50, 25);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { herosprite.move(-0.1, 0); } //первая координата Х отрицательна =>идём влево
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { herosprite.move(0.1, 0); } //первая координата Х положительна =>идём вправо
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { herosprite.move(0, -0.1); } //вторая координата (У) отрицательна =>идём вверх (вспоминаем из предыдущих уроков почему именно вверх, а не вниз)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { herosprite.move(0, 0.1); } //вторая координата (У) положительна =>идём вниз (если не понятно почему именно вниз - смотрим предыдущие уроки)

		window.clear();
		window.draw(herosprite);
		window.display();
	}

	return 0;
}