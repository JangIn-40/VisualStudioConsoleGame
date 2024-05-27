﻿#include <iostream>
#include <random>
#include <windows.h>

const char STAGE_DATA[] = "\
**********\n\
**********\n\
**********\n\
**********\n\
**********";

const int STAGE_ROW{ 10 };
const int STAGE_COL{ 10 };
const int N_MINE{ 10 };

enum Object
{
	OBJ_MINE,
	OBJ_STAR,
	OBJ_EMPTY_SPACE,
	OBJ_AROUND_MINE_NUMBER1,
	OBJ_AROUND_MINE_NUMBER2,
	OBJ_AROUND_MINE_NUMBER3,
	OBJ_AROUND_MINE_NUMBER4,
	OBJ_AROUND_MINE_NUMBER5,
	OBJ_AROUND_MINE_NUMBER6,
	OBJ_AROUND_MINE_NUMBER7,
	OBJ_AROUND_MINE_NUMBER8,

	OBJ_MAX
};

void GenerateRandomMineLocation(Object * stage, int row, int col, int nMine);
void Initialize(Object *stage, int row, int col, const char *stageData);
void Draw(const Object *stage, int row, int col, bool *isVisible);
void Visible(Object *stage, int row, int col, int playerRow, int playerCol, bool *isVisible);
bool IsClear(const Object *stage, int row, int col, bool *isVisible);
void CountMines(Object *stage, int row, int col);

int main()
{
	Object *stage = new Object[STAGE_ROW * STAGE_COL];
	if (!stage)
	{
		std::cout << "memeory leak in main()" << std::endl;
		return -1;
	}

	Initialize(stage, STAGE_ROW, STAGE_COL, STAGE_DATA);
	GenerateRandomMineLocation(stage, STAGE_ROW, STAGE_COL, N_MINE);
	CountMines(stage, STAGE_ROW, STAGE_COL);

	bool isVisible[STAGE_ROW * STAGE_COL]{};

	while (true)
	{
		system("cls");
		Draw(stage, STAGE_ROW, STAGE_COL, isVisible);

		int playeRow, playerCol;
		std::cout << "Enter your move(row and column) : ";
		std::cin >> playeRow >> playerCol;

		if ((playeRow < 0 && playeRow > STAGE_ROW - 1 )|| (playerCol < 0 && playerCol > STAGE_COL - 1))
		{
			std::cout << "Input correct row and col" << std::endl;
			Sleep(1000);
			continue;
		}

		Visible(stage, STAGE_ROW, STAGE_COL, playeRow, playerCol, isVisible);

		if (stage[playeRow * STAGE_COL + playerCol] == OBJ_MINE)
		{
			system("cls");
			Draw(stage, STAGE_ROW, STAGE_COL, isVisible);
			std::cout << "Game Over!" << std::endl;
			break;
		}

		if (IsClear(stage, STAGE_ROW, STAGE_COL, isVisible))
		{
			system("cls");
			Draw(stage, STAGE_ROW, STAGE_COL, isVisible);
			std::cout << "Congratulations!" << std::endl;
			break;
		}
	}
	
	delete[] stage;
}

void GenerateRandomMineLocation(Object *stage, int row, int col, int nMine)
{
	
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(1, row * col - 1);
	
	bool setMine[STAGE_ROW * STAGE_COL]{};
	for (int i = 0; i < nMine; ++i)
	{
		int random;
		do
		{
			random = dist(mt);
		} while (setMine[random]);
		{
			stage[random] = OBJ_MINE;
			setMine[random] = true;
		}
	}
}

void Initialize(Object *stage, int row, int col, const char *stageData)
{
	const char *p = stageData;
	int x{}, y{};

	while (*p != '\0')
	{
		Object o;

		switch (*p)
		{
		case 'X':
			o = OBJ_MINE;
			break;
		case '*':
			o = OBJ_STAR;
			break;
		case 'O':
			o = OBJ_EMPTY_SPACE;
			break;
		case '1':
			o = OBJ_AROUND_MINE_NUMBER1;
			break;
		case '2':
			o = OBJ_AROUND_MINE_NUMBER2;
			break;
		case '3':
			o = OBJ_AROUND_MINE_NUMBER3;
			break;
		case '4':
			o = OBJ_AROUND_MINE_NUMBER4;
			break;
		case '5':
			o = OBJ_AROUND_MINE_NUMBER5;
			break;
		case '6':
			o = OBJ_AROUND_MINE_NUMBER6;
			break;
		case '7':
			o = OBJ_AROUND_MINE_NUMBER7;
			break;
		case '8':
			o = OBJ_AROUND_MINE_NUMBER8;
			break;
		case '\n':
			x = 0;
			y++;
			o = OBJ_MAX;
			break;
		default:
			o = OBJ_MAX;
			break;
		}
		++p;

		if (o != OBJ_MAX)
		{
			stage[y * col + x] = o;
			x++;
		}
	}
}

void Draw(const Object *stage, int row, int col, bool *isVisible)
{
	static const char font[]{ 'X', '*', 'O', '1', '2', '3', '4', '5', '6', '7', '8'};

	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < col; x++)
		{
			Object o = stage[y * col + x];

			if (isVisible[col * y + x])
			{
				std::cout << font[o];
			}
			else
			{
				std::cout << '*';
			}
		}

		std::cout << std::endl;
	}
}

void Visible(Object *stage, int row, int col, int playerRow, int playerCol, bool *isVisible)
{
	if (playerRow < 0 || playerRow >= row || playerCol < 0 || playerCol >= col || isVisible[playerRow * col + playerCol]) 
	{
		return;
	}

	isVisible[playerRow * col + playerCol] = true;

	if (stage[playerRow * col + playerCol] == OBJ_EMPTY_SPACE) 
	{
		for (int dy = -1; dy <= 1; ++dy) 
		{
			for (int dx = -1; dx <= 1; ++dx) 
			{
				if (dy != 0 || dx != 0) 
				{
					Visible(stage, row, col, playerRow + dy, playerCol + dx,  isVisible);
				}
			}
		}
	}
	else if (stage[playerRow * col + playerCol] == OBJ_MINE) 
	{
		return;
	}
}

bool IsClear(const Object *stage, int row, int col, bool *IsVisible)
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			if (!IsVisible[i * col + j] && stage[i * col + j] != OBJ_MINE)
			{
				return false;
			}
		}
	}

	return true;
}

void CountMines(Object *stage, int row, int col)
{
	//...?
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			if (stage[i * col + j] != OBJ_MINE)
			{
				int mine_count = 0;
				for (int dy = -1; dy <= 1; ++dy)
				{
					for (int dx = -1; dx <= 1; ++dx)
					{
						int tx = j + dx;
						int ty = i + dy;
						if (tx >= 0 && tx < col && ty >= 0 && ty < row && stage[ty * col + tx] == OBJ_MINE)
						{
							mine_count++;
						}
					}
				}

				if (mine_count == 0)
				{
					stage[i * col + j] = OBJ_EMPTY_SPACE;
				}
				else
				{
					stage[i * col + j] = static_cast<Object>(OBJ_AROUND_MINE_NUMBER1 + mine_count - 1);
				}
			}
		}
	}
}