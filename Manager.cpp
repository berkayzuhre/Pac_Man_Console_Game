#include "Manager.h"


Manager::Manager()
{
}

Manager::~Manager()
{
}


bool Manager::OnUserCreate() {

	ChampX = 81;
	ChampY = 35;

	MX[0] = 153;
	MY[0] = 95;

	MX[1] = 9;
	MY[1] = 20;

	MX[2] = 153;
	MY[2] = 20;

	MX[3] = 9;
	MY[3] = 95;

	nLevelWidth = 20;
	nLevelHeight = 20;

	nTileWidth = ScreenWidth() / nLevelWidth;
	nTileHeight = ScreenHeight() / nLevelHeight;

	sLevel += L"#........#........#|";
	sLevel += L"#.##.###.#.###.##.#|";
	sLevel += L"#.##.###.#.###.##.#|";
	sLevel += L"#.................#|";
	sLevel += L"#.##.#.#####.#.##.#|";
	sLevel += L"#....#...#...#....#|";
	sLevel += L"####.###.#.###.####|";
	sLevel += L"|||#.#.......#.#||||";
	sLevel += L"####.#.#####.#.####|";
	sLevel += L".......#|||#.......|";
	sLevel += L"####.#.#####.#.####|";
	sLevel += L"|||#.#.......#.#||||";
	sLevel += L"####.#.#####.#.####|";
	sLevel += L"#....#...#...#....#|";
	sLevel += L"#.##.###.#.###.##.#|";
	sLevel += L"#..#...........#..#|";
	sLevel += L"##.#.#.#####.#.#.##|";
	sLevel += L"#....#...#...#....#|";
	sLevel += L"#.######.#.######.#|";
	sLevel += L"#.................#|";

	RightOpen += L"**#####**";
	RightOpen += L"*#######*";
	RightOpen += L"####*****";
	RightOpen += L"*#######*";
	RightOpen += L"**#####**";

	LeftOpen += L"**#####**";
	LeftOpen += L"*#######*";
	LeftOpen += L"*****####";
	LeftOpen += L"*#######*";
	LeftOpen += L"**#####**"; 



	DownOpen += L"**#####**";
	DownOpen += L"*#######*";
	DownOpen += L"####*####";
	DownOpen += L"*###*###*";
	DownOpen += L"**##*##**";

	UpOpen += L"**##*##**";
	UpOpen += L"*###*###*";
	UpOpen += L"####*####";
	UpOpen += L"*#######*";
	UpOpen += L"**#####**";

	Closed += L"**#####**";
	Closed += L"*#######*";
	Closed += L"#########";
	Closed += L"*#######*";
	Closed += L"**#####**";

	Monster += L"**#####**";
	Monster += L"*#######*";
	Monster += L"*#*###*#*";
	Monster += L"*#######*";
	Monster += L"#*#*#*#*#";

	ChampTimer = 0;
	MonsterTimer = 0;

	GOD = TRUE;

	Dog = TRUE;


	return true;
}

bool Manager::OnUserUpdate(float fElapsedTime) {


	if (GOD) {
		DrawBoard();


		//Move and Draw Champ

		ChampMove(fElapsedTime);

		DrawChamp(fElapsedTime);

		//Move and Draw Monsters

		for (int i = 0; i < 4; i++) {

			if ((int)MX[i] % 9 == 0 && (int)MY[i] % 5 == 0)
				SetDirection((int)MX[i], (int)MY[i], MonsterDirection[i], fElapsedTime, i);

			MonsterMove(MX[i], MY[i], MonsterDirection[i], fElapsedTime);


			//for (int k = MX[i]; k < (int)MX[i] + 9; k++)
			//	for (int l = MY[i]; l < (int)MY[i] + 5; l++) {
			//		if(Mo)
			//		Draw(k, l, PIXEL_SOLID, FG_GREEN+i);
			//	}



			for (int k = MX[i]; k < (int)MX[i] + nTileWidth; k++)
				for (int l = MY[i]; l < (int)MY[i] + nTileHeight; l++) {

					switch (Monster[(l - (int)MY[i]) * nTileWidth + (k - (int)MX[i])])
					{
					case L'#':
						Draw(k, l, PIXEL_SOLID, FG_RED + i);
						break;
					case L'*':
						Draw(k, l, PIXEL_SOLID, BG_BLACK);
						break;
					}

				}

		}



	}



	return true;
}

void Manager::DrawBoard() {

	for (int x = 0; x < nLevelWidth; x++) {
		for (int y = 0; y < nLevelHeight; y++) {

			wchar_t sTileID = sLevel[y * nLevelWidth + x];

			switch (sTileID) {

			case L'.':
				//Draw active black path
				Fill(x * nTileWidth, y * nTileHeight, (x + 1) * nTileWidth, (y + 1) * nTileHeight, PIXEL_SOLID, FG_BLACK);

				//Draw white baits
				Fill(x * nTileWidth + nTileWidth / 2, y * nTileHeight + nTileHeight / 2, x * nTileWidth + nTileWidth / 2 + 2, y * nTileHeight + nTileHeight / 2 + 1, PIXEL_SOLID, FG_WHITE);

				break;
			case L'#':

				//Draw walls
				Fill(x * nTileWidth, y * nTileHeight, (x + 1) * nTileWidth, (y + 1) * nTileHeight, PIXEL_SOLID, FG_DARK_BLUE);

				break;
			case L'|':

				//Draw passive black path
				Fill(x * nTileWidth, y * nTileHeight, (x + 1) * nTileWidth, (y + 1) * nTileHeight, PIXEL_SOLID, FG_BLACK);
				break;
			case L'+':

				//Draw passive black path
				Fill(x * nTileWidth, y * nTileHeight, (x + 1) * nTileWidth, (y + 1) * nTileHeight, PIXEL_SOLID, FG_RED);
				break;
			default:
				//Check for errors, there shouldnt be any yellow on the board
				Fill(x * nTileWidth, y * nTileHeight, (x + 1) * nTileWidth, (y + 1) * nTileHeight, PIXEL_SOLID, FG_YELLOW);
				break;
			}
		}
	}

	ResetMap2();

}

void Manager::ChampMove(float fElapsedTime) {

	char c1, c2;
	//In the map array, walls are represented with -1. Moveble paths are represented with 0. 

	int Speed = 30;

	auto GetTile = [=](int x, int y)
	{
		if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
			return sLevel[y * nLevelWidth + x];
		else
			return L' ';
	};



	ChampDirection = 'N';

	if (m_keys[VK_LEFT].bHeld)
	{

		c1 = GetTile((((int)ChampX + nTileWidth - 1) / nTileWidth) - 1, (int)ChampY / nTileHeight);

		c2 = GetTile((((int)ChampX + nTileWidth - 1) / nTileWidth) - 1, ((int)ChampY + nTileHeight - 1) / nTileHeight);

		//if (check1&&check2)
		if (c1 != '#' && c2 != '#') {
			ChampX -= (fElapsedTime*Speed);
			ChampDirection = 'L';

		}
		if (ChampX < -4)
			ChampX = 170;


	}

	if (m_keys[VK_RIGHT].bHeld)
	{

		c1 = GetTile(((int)ChampX / nTileWidth) + 1, (int)ChampY / nTileHeight);
		c2 = GetTile(((int)ChampX / nTileWidth) + 1, ((int)ChampY + nTileHeight - 1) / nTileHeight);

		if (c1 != '#' && c2 != '#') {
			ChampX += (fElapsedTime*Speed);
			ChampDirection = 'R';
		}

		if (ChampX > 170)
			ChampX = 0;

	}

	if (m_keys[VK_UP].bHeld)
	{

		c1 = GetTile(((int)ChampX / nTileWidth), (((int)ChampY + nTileHeight - 1) / nTileHeight) - 1);
		c2 = GetTile((((int)ChampX + nTileWidth - 1) / nTileWidth), (((int)ChampY + nTileHeight - 1) / nTileHeight) - 1);

		if (c1 != '#' && c2 != '#' && ChampY > 0) {
			ChampY -= (fElapsedTime*Speed);
			ChampDirection = 'U';
		}

	}

	if (m_keys[VK_DOWN].bHeld)
	{

		c1 = GetTile(((int)ChampX / nTileWidth), ((int)ChampY / nTileHeight) + 1);
		c2 = GetTile((((int)ChampX + nTileWidth - 1) / nTileWidth), ((int)ChampY / nTileHeight) + 1);

		if (c1 != '#' && c2 != '#' && ChampY < 95) {
			ChampY += (fElapsedTime*Speed);
			ChampDirection = 'D';
		}

	}


	//Eat bait

	if (sLevel[(((int)ChampY+3) / nTileHeight) * nLevelWidth + (((int)ChampX+4) / nTileWidth)] == '.') {
		sLevel[(((int)ChampY + 3) / nTileHeight) * nLevelWidth + (((int)ChampX + 4) / nTileWidth)] = '|';
	}


}

void Manager::DrawChamp(float fElapsedTime) {

	wstring temp=Closed;



	ChampTimer += fElapsedTime;
	switch (ChampDirection) {

	case 'R':
		if (ChampTimer > 0.5)
			temp = RightOpen;
		break;
	case 'L':
		if (ChampTimer > 0.5)
			temp = LeftOpen;
		break;
	case 'U':
		if (ChampTimer > 0.5)
			temp = UpOpen;
		break;
	case 'D':
		if (ChampTimer > 0.5)
			temp = DownOpen;
		break;
	}
	if (ChampTimer > 1) ChampTimer = 0;




	for (int k = ChampX; k < (int)ChampX + nTileWidth; k++)
		for (int l = ChampY; l < (int)ChampY + nTileHeight; l++) {

				switch (temp[(l - (int)ChampY) * nTileWidth + (k - (int)ChampX)])
				{
				case L'#':
					Draw(k, l, PIXEL_SOLID, FG_YELLOW);
					break;
				case L'*':
					Draw(k, l, PIXEL_SOLID, BG_BLACK);
					break;
				}

		}

}

void Manager::SetDirection(int MonsterX, int MonsterY,char& direction,float fElapsedTime,int id) {

		int value = 1;

		int goalX;
		int goalY;

		MonsterTimer += fElapsedTime;

		if (MonsterTimer < 5) {
			goalX = (int)ChampX / nTileWidth;
			goalY = (int)ChampY / nTileHeight;
			Dog = TRUE;
		}
		else {

			Dog = FALSE;

			if (id == 0) {
				goalX =17 ;
				goalY = 19;
			}
			else if (id == 1) {
				goalX = 1;
				goalY = 4;
			}
			else if(id==2){
				goalX = 17;
				goalY = 4;

			}
			else if (id == 3) {
				goalX = 1;
				goalY = 19;

			}
		}

		if (MonsterTimer > 10) MonsterTimer = 0;

		map2[goalX][goalY] = value;



		for (int k = 0; k < 50; k++) {
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++) {

					if (map2[i][j] == value) {
						if (map2[i + 1][j] == 0 && i < 19) map2[i + 1][j] = value + 1;
						if (map2[i - 1][j] == 0 && i > 0) map2[i - 1][j] = value + 1;
						if (map2[i][j + 1] == 0 && j < 19) map2[i][j + 1] = value + 1;
						if (map2[i][j - 1] == 0 && j > 0) map2[i][j - 1] = value + 1;

					}

				}

			}
			value++;
		}

		//find min
		int min = INT_MAX;

		int check;


		int MX = MonsterX / nTileWidth;
		int MY = MonsterY / nTileHeight;
		

		check = map2[(MonsterX / nTileWidth) + 1][MonsterY / nTileHeight];

		if (check < min && check > 0&&MonsterX<175) {
			min = check;
			direction = 'r';
		}

		//check going left

		check = map2[(MonsterX / nTileWidth) - 1][MonsterY / nTileHeight];

		if (check < min && check > 0&&MonsterX>0) {
			min = check;
			direction = 'l';
		}

		//check going down

		check = map2[MonsterX / nTileWidth][(MonsterY / nTileHeight) + 1];


		if (check < min && check > 0&&MonsterY<95) {
			min = check;
			direction = 'd';
		}
		//check going up

		check = map2[MonsterX / nTileWidth][(MonsterY / nTileHeight) - 1];

		if (check < min && check > 0&&MonsterY>0) {

			min = check;
			direction = 'u';

		}

		if ((int)ChampX / nTileWidth == MonsterX / nTileWidth && (int)ChampY / nTileHeight == MonsterY / nTileHeight) {
			direction = 'n';
			GOD = FALSE;
		}

		ResetMap2();
}







void Manager::MonsterMove(float& MonsterX,float& MonsterY,char direction,float fElapsedTime) {

	int Speed=20;

	switch (direction) {
	case 'r':
		MonsterX+=(fElapsedTime*Speed);
		break;
	case 'l':
		MonsterX-=(fElapsedTime*Speed);
		break;
	case 'u':
		MonsterY-=(fElapsedTime*Speed);
		break;
	case 'd':
		MonsterY+=(fElapsedTime*Speed);
		break;
	case 'n':
		break;

	}

}

void Manager::ResetMap2() {


	for (int i = 0; i < nLevelWidth; i++) {

		for (int j = 0; j < nLevelHeight; j++) {

			wchar_t sTileID = sLevel[j * nLevelWidth + i];

			switch (sTileID) {
			case L'.':
				map2[i][j] = 0;

				break;

			case L'|':
				map2[i][j] = 0;

				break;
			default:
				map2[i][j] = -1;

				break;
			}
		}
	}
	
	



}
