#pragma once

///////////////////////////////////����� ������//////////////////////////////////
int getCurrentMission(int x)//�-��� ������ ������, ������� ������ ����� ������, � ����������� �� ���������� ������ � (���� ����� ���������� ��� ����������)
{
	int mission = 0;
	if ((x > 0) && (x < 400)) { mission = 0; } //�������� ������ � �����
	//if ((x > 400) && (x < 700)) { mission = 1; } //����� �� ������ ������
	//if ((x > 700) && (x < 1300)) { mission = 2; }//2��
	//if ((x > 1300) && (x < 1920)) { mission = 3; }//� ��

	return mission;//�-��� ���������� ����� ������
}



/////////////////////////////////////����� ������/////////////////////////////////
std::string getTextMission(int currentMission) {

	std::string missionText = "";//����� ������ � ��� �������������

	switch (currentMission)//����������� ����� ������ � � ����������� �� ���� ���������� missionText ������������� ��������� �����
	{
	case 0: missionText = "\n   Initial stage and \n   instructions for the game: \n   WASD or UpLeftDownRight - \n   Movement  \n   Space - Shooting" ; break;
	//case 1: missionText = "\n   Mission 1\n\n   Here's your first one\n   mission, on\n   this level \n   ���� ����� ���������\n   ... ���-���-��� ..."; break;
	//case 2: missionText = "\n   Mission 2\n   ���������� ������\n   ���������� �������,\n   ����� ������ ������ "; break;
	//case 3: missionText = "\n   � ��� ����� \n   � ���� ��������....."; break;
	}

	return missionText;//�-��� ���������� �����
};