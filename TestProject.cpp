#include <SFML/Graphics.hpp>
#include <iostream>
#include "view.h"
#include <SFML/Audio.hpp>
#include "GameStart.h"
#include <sstream>
#include "mission.h"
#include "level.h"
#include "vector"
#include "list"
///////////////////// Основной класс \\\\\\\\\\\\\\\\\\\\\\\\\

class Entity
{
public:
	std::vector<Object> obj;
	float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
	int w, h, health;
	bool life, isMove, onGround, isShoot;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::String name;//враги могут быть разные, мы не будем делать другой класс для врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени
	Entity(sf::Image& image, float X, float Y, int W, int H, sf::String Name) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	sf::FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return sf::FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
	virtual void update(float time) = 0;

};


///////////////////// Класс Игрока \\\\\\\\\\\\\\\\\\\\\\\\

class Player : public Entity
{ 
public:
	int PlayerScore;
	float CurrentFrame = 0; //хранение текущего кадра
	enum stateObject { left, right, up, down, jump, stay } state;//добавляем тип перечисления - состояние объекта

	Player(sf::Image& image, Level& lev, float X, float Y, float W, float H, sf::String Name) : Entity(image, X, Y, W, H, Name)
	{
		PlayerScore = 0;  state = stay; obj = lev.GetAllObjects();
		if (name == "Player1")
		{
			sprite.setTextureRect(sf::IntRect(0, 0, w, h));
		}
	}
	void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		control(time);
		switch (state)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
		{
		case right: dx = speed; break;//состояние идти вправо
		case left: dx = -speed; break;//состояние идти влево
		case up: break;//будет состояние поднятия наверх (например по лестнице)
		case down: dx = 0; break;//будет состояние во время спуска персонажа (например по лестнице)
		case jump: break;//здесь может быть вызов анимации
		case stay: break;//и здесь тоже		

		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
		y += dy * time;
		checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y

		if (health <= 0)
		{
			life = false;
		}
		dy = dy + 0.0015 * time;
		checkCollisionWithMap(0, dy); //делаем притяжение к земле
		speed = 0;//зануляем скорость, чтобы персонаж остановился.
		sprite.setPosition(x + w / 2, y + h / 2);//выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
		if (life) getPlayerCoordinateForView(x, y);
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
		

	}


	void control(float time)
	{
		if (life == true)
		{
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
			{
				state = left;
				speed = 0.15;
				CurrentFrame += 0.006 * time;
				if (CurrentFrame > 4) CurrentFrame -= 4;
				sprite.setTextureRect(sf::IntRect(108 * int(CurrentFrame) + 106, 0, -100, 133));
				

			}
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
			{
				state = right;
				speed = 0.15;
				CurrentFrame += 0.006 * time;
				if (CurrentFrame > 4) CurrentFrame -= 4;
				sprite.setTextureRect(sf::IntRect(108 * int(CurrentFrame), 0, 100, 133));
			}
			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W))) && (onGround))
			{
				state = jump;
				dy = -0.8;
				onGround = false;
				isShoot = false;
			}
			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S))))
			{
				state = down;
				speed = 0.15;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				isShoot = true;
			}
		}


	}
	float getPlayerCoordinateX() {	//этим методом будем забирать координату Х	
		return x;
	}
	float getPlayerCoordinateY() {	//этим методом будем забирать координату Y 	
		return y;
	}


//////////////////////////// Класс Врага \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

class Enemy : public Entity {
	public:
		Enemy(sf::Image& image, Level& lvl, float X, float Y, int W, int H, sf::String Name) :Entity(image, X, Y, W, H, Name) {
			if (name == "EasyEnemy") {
				obj = lvl.GetObjects("solid");
				sprite.setTextureRect(sf::IntRect(0, 0, w, h));
				dx = 0.1;//даем скорость.этот объект всегда двигается
			}
		}

		void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
		{
			
			for (int i = 0; i < obj.size(); i++)//проходимся по объектам
				if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
				{
					if (obj[i].name == "solid")//если встретили препятствие
					{
						if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
						if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
						if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
						if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
					}
				}
		}

		void update(float time)
		{
			if (name == "EasyEnemy") {//для персонажа с таким именем логика будет такой

				moveTimer += time; if (moveTimer > 3000) { dx *= -1; moveTimer = 0; sprite.scale(-1, 1);	}//меняет направление и инвертирует спрайт примерно каждые 3 сек

				checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
				x += dx * time;
				sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
				if (health <= 0) { life = false; }
				
			}
		}
		float getEnemyCoordinateX() {	//этим методом будем забирать координату Х	
			return x;
		}
		float getEnemyCoordinateY() {	//этим методом будем забирать координату Y 	
			return y;
		}
	};


};

//////////////////////// Класс Пули \\\\\\\\\\\\\\\\\\\\\\\\\\

class Bullet :public Entity {
public:
	int direction;//направление пули

	Bullet(sf::Image& image, Level& lvl, float X, float Y, int W, int H, int dir, sf::String Name) :Entity(image, X, Y, W, H, Name) {//всё так же, только взяли в конце состояние игрока (int dir)
		obj = lvl.GetObjects("solid");//инициализируем .получаем нужные объекты для взаимодействия пули с картой
		x = X;
		y = Y;
		direction = dir;
		speed = 0.8;
		w = 20;
		h = 17;
		life = true;
		//выше инициализация в конструкторе
	}


	void update(float time)
	{
		switch (direction)
		{
		case 0: dx = -speed; dy = 0; sprite.setPosition(x + 5, y + 57);   break;//интовое значение state = left
		case 1: dx = speed; dy = 0; sprite.setPosition(x+100, y + 57);    break;//интовое значение state = right
		
		}

		x += dx * time;//само движение пули по х
		y += dy * time;//по у

		if (x <= 0) x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {//проход по объектам 
			if (getRect().intersects(obj[i].rect)) //если этот объект столкнулся с пулей,
			{
				life = false;// то пуля умирает
			}
		}

		//sprite.setPosition(x+15, y+57);//задается позицию пуле
	}
};


int main()


{

	
	////////////////////////////////////////// Экран \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game", sf::Style::Fullscreen  ); //создание окна
	sf::Music Menu;//создаем объект музыки
	Menu.openFromFile("Music/Menu.ogg");//загружаем файл
	Menu.play();//воспроизводим музыку
	Menu.setLoop(true);//зацикливаем музыку
	menu(window);
	Menu.stop();

	view.reset(sf::FloatRect(0, 0, 960, 540));//размер "вида" камеры при создании объекта вида камеры
	window.setVerticalSyncEnabled(true);


	////////////////////////////////////////// Шрифт \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	sf::Font font;//шрифт 
	font.loadFromFile("tektur-6.ttf");//передаем нашему шрифту файл шрифта
	sf::Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	// text.setColor(sf::Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	// text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый



	////////////////////////////////////////// Загрузка карты \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	
	Level lvl;//создали экземпляр класса уровень
	lvl.LoadFromFile("Map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

	////////////////////////////////////////// Звук \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	sf::SoundBuffer shootBuffer;//создаём буфер для звука
	shootBuffer.loadFromFile("Sound/shoot.ogg");//загружаем в него звук
	sf::Sound shoot(shootBuffer);//создаем звук и загружаем в него звук из буфера

	sf::SoundBuffer GoBuffer;
	GoBuffer.loadFromFile("Sound/Go.ogg");
	sf::Sound Go(GoBuffer);

	sf::Music music;//создаем объект музыки
	music.openFromFile("Music/Embient.ogg");//загружаем файл
	music.play();//воспроизводим музыку
	music.setLoop(true);//зацикливаем музыку
	
	////////////////////////////////////////// Загрузка Статуса \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	
	bool showMissionText = true;//логическая переменная, отвечающая за появление текста миссии на экране
	sf::Image quest_image;
	quest_image.loadFromFile("Texture/Status.png");
	quest_image.createMaskFromColor(sf::Color(0, 0, 0));
	sf::Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	sf::Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(sf::IntRect(0, 0, 600, 600));  //приведение типов, размеры картинки исходные
	s_quest.setScale(0.6f, 0.6f);//чуть уменьшили картинку, => размер стал меньше
    
	
	////////////////////////////////////////// Загрузка игровых персонажей \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	
	sf::Image heroImage;
	heroImage.loadFromFile("Texture/Герой/mech.png");

	sf::Image easyEnemyImage;
	easyEnemyImage.loadFromFile("Texture/Враги/ЛегкийВраг.png");

	sf::Image BulletImage;//изображение для пули
	BulletImage.loadFromFile("Texture/Карта/Пуля2.png");//загрузили картинку в объект изображения

	

	Object player = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	


	Player p(heroImage, lvl, player.rect.left, player.rect.top, 100,133, "Player1");

	
	
	
	/////////////////////// Списки чего угодно (враги, барьеры, усилители) \\\\\\\\\\\\\\\\\\\\\\\\

	std::list<Entity*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
	std::list<Entity*>  bullet;

	std::list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка
	std::list<Entity*>::iterator ib;
	
	std::vector<Object> e = lvl.GetObjects("easyEnemy");//все объекты врага на tmx карте хранятся в этом векторе



	for (int i = 0; i < e.size(); i++)//проходимся по элементам этого вектора(а именно по врагам)
		entities.push_back(new Player::Enemy(easyEnemyImage, lvl, e[i].rect.left, e[i].rect.top, 100, 133, "EasyEnemy"));//и закидываем в список всех наших врагов с карты
		
	

	////////////////////////////////////////// Объявление переменных времени и прочее  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	sf::Clock clock;
	sf::Clock gameTimeClock;
	int gameTime = 0;
	float CurrentFrame = 0; //хранение текущего кадра

	////////////////////////////////////////// Работа окна \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в секундах идёт вперед, пока жив игрок
		clock.restart(); //перезагружает время
		time = time / 800; //скорость игры
		//std::cout << time << "\n";//смотрим как живет время (перезагружается, как видим)
		
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					window.close();
				

			if (p.isShoot == true) { p.isShoot = false; entities.push_back(new Bullet (BulletImage, lvl, p.x, p.y, 20, 17, p.state, "Bullet")); shoot.play();}//если выстрелили, то появляется пуля. enum передаем как int 

			if (event.type == sf::Event::KeyPressed)//событие нажатия клавиши
				if ((event.key.code == sf::Keyboard::Tab)) {//если клавиша ТАБ


					switch (showMissionText) {//переключатель, реагирующий на логическую переменную showMissionText

					case true: {
						std::ostringstream playerHealthString;//строка здоровья игрока
						playerHealthString << p.health; //заносим в строку здоровье 
						std::ostringstream task;//строка текста миссии
						task << getTextMission(getCurrentMission(p.getPlayerCoordinateX()));//вызывается функция getTextMission (она возвращает текст миссии), которая принимает в качестве аргумента функцию getCurrentMission(возвращающую номер миссии), а уже эта ф-ция принимает в качестве аргумента функцию p.getplayercoordinateX() (эта ф-ция возвращает Икс координату игрока)
						text.setString( "\n   HP: " + playerHealthString.str() + "\n" + task.str());//задаем
						text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
						s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока
						showMissionText = false;//эта строка позволяет убрать все что мы вывели на экране
						break;//выходим , чтобы не выполнить условие "false" (которое ниже)
					}
					case false: {
						text.setString("");//если не нажата клавиша таб, то весь этот текст пустой
						showMissionText = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
						break;
					}
					}
				}
		}

		p.update(time);

		for (it = entities.begin(); it != entities.end();)//говорим что проходимся от начала до конца
		{
			Entity* b = *it;//для удобства, чтобы не писать (*it)->
			b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			if (b->life == false) { it = entities.erase(it); delete b; }// если этот объект мертв, то удаляем его
			else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}

		for (it = entities.begin(); it != entities.end(); it++)//проходимся по эл-там списка
		{
			if ((*it)->getRect().intersects(p.getRect()))//если прямоугольник спрайта объекта пересекается с игроком
			{
				if ((*it)->name == "EasyEnemy") {//и при этом имя объекта EasyEnemy,то..

					if ((p.dy > 0) && (p.onGround == false)) { (*it)->dx = 0; p.dy = -0.2; (*it)->health = 0; }//если прыгнули на врага,то даем врагу скорость 0,отпрыгиваем от него чуть вверх,даем ему здоровье 0
					else {
						p.health -= 5;	//иначе враг подошел к нам сбоку и нанес урон
					}
				}
			}
		}

		for (it = entities.begin(); it != entities.end(); it++)//проходимся по эл-там списка
		{
		     


			if ((*it)->getRect().intersects(p.getRect()))//если прямоугольник спрайта объекта пересекается с игроком
			{
				if ((*it)->name == "EasyEnemy") {//и при этом имя объекта EasyEnemy,то..
					if ((*it)->dx > 0)//если враг идет вправо
					{

						(*it)->x = p.x - (*it)->w; //отталкиваем его от игрока влево (впритык)
						(*it)->dx = 0;//останавливаем

						
					}
					if ((*it)->dx < 0)//если враг идет влево
					{
						(*it)->x = p.x + p.w; //аналогично - отталкиваем вправо
						(*it)->dx = 0;//останавливаем
					}
					if (p.dx < 0) { p.x = (*it)->x + (*it)->w; }//если столкнулись с врагом и игрок идет влево то выталкиваем игрока
					if (p.dx > 0) { p.x = (*it)->x - p.w; }//если столкнулись с врагом и игрок идет вправо то выталкиваем игрока
				}
			}
			//if ((*it)->getRect().inter

	

		}
		

		window.setView(view);
		window.clear();
		lvl.Draw(window);//рисуем новую карту
		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite); //рисуем entities объекты (сейчас это только враги)
		}

		
		
		
		window.draw(p.sprite);

		////////////////////////////// Cтатус \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

		if (!showMissionText) {
			text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока			
			window.draw(s_quest); window.draw(text); //рисуем спрайт свитка (фон для текста миссии). а затем и текст. все это завязано на логическую переменную, которая меняет свое состояние от нажатия клавиши ТАБ
		}
		window.display();
		
	}

	return 0;
}
