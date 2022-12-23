#pragma once

///////////////////////////////////НОМЕР МИССИИ//////////////////////////////////
int getCurrentMission(int x)//ф-ция номера миссия, которая меняет номер миссии, в зависимости от координаты игрока Х (сюда будем передавать эту координату)
{
	int mission = 0;
	if ((x > 0) && (x < 400)) { mission = 0; } //знакомим игрока с игрой
	//if ((x > 400) && (x < 700)) { mission = 1; } //игрок на первой миссии
	//if ((x > 700) && (x < 1300)) { mission = 2; }//2ой
	//if ((x > 1300) && (x < 1920)) { mission = 3; }//и тд

	return mission;//ф-ция возвращает номер миссии
}



/////////////////////////////////////ТЕКСТ МИССИИ/////////////////////////////////
std::string getTextMission(int currentMission) {

	std::string missionText = "";//текст миссии и его инициализация

	switch (currentMission)//принимается номер миссии и в зависимости от него переменной missionText присваивается различный текст
	{
	case 0: missionText = "\n   Initial stage and \n   instructions for the game: \n   WASD or UpLeftDownRight - \n   Movement  \n   Space - Shooting" ; break;
	//case 1: missionText = "\n   Mission 1\n\n   Here's your first one\n   mission, on\n   this level \n   тебе стоит опасаться\n   ... бла-бла-бла ..."; break;
	//case 2: missionText = "\n   Mission 2\n   Необходимо решить\n   логическую задачку,\n   чтобы пройти дальше "; break;
	//case 3: missionText = "\n   И так далее \n   и тому подобное....."; break;
	}

	return missionText;//ф-ция возвращает текст
};