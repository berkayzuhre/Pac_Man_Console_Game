#pragma once
#include "olcConsoleGameEngine.h"


class Manager: public olcConsoleGameEngine
{
public:
	Manager();
	~Manager();

	virtual bool OnUserUpdate(float fElapsedTime);

	virtual bool OnUserCreate();

	void ChampMove(float);

	void DrawChamp(float);

	void MonsterMove(float&,float&,char,float);

	void SetDirection(int, int,char&,float,int);

	void DrawBoard();

	void ResetMap2();
	

private:

	wstring sLevel;

	wstring Monster;

	wstring RightOpen;

	wstring LeftOpen;

	wstring DownOpen;

	wstring UpOpen;

	wstring Closed;

	int nLevelWidth;
	int nLevelHeight;

	int nTileWidth; 
	int nTileHeight; 

	float ChampX;
	float ChampY;

	float ChampTimer;
	float MonsterTimer;

	float MX[4];
	float MY[4];


	char ChampDirection;

	char MonsterDirection[4];


	int map2[20][20];

	bool GOD;

	bool Dog;
};

