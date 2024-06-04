#include <iostream>
#include <random>
#include <windows.h>

const char STAGE_DATA[] = "\
**********\n\
**********\n\
**********\n\
**********\n\
**********\n\
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
void Draw(const Object *stage, int row, int col, const bool *isVisible, const bool *isFlag);
bool FlagPlaced(const Object *stage, int row, int col, int playerRow, int playerCol, bool *isFlag, char flag);
void Visible(const Object *stage, int row, int col, int playerRow, int playerCol, bool *isVisible, bool *isFlag);
bool IsClear(const Object *stage, int row, int col, const bool *isVisible);
void CountMines(Object *stage, int row, int col);

int main()
{
	Object *stage = new Object[STAGE_ROW * STAGE_COL];
	if (!stage)
	{
		std::cout << "memeory leak in main()" << std::endl;
		return -1;
	}

	GenerateRandomMineLocation(stage, STAGE_ROW, STAGE_COL, N_MINE);
	CountMines(stage, STAGE_ROW, STAGE_COL);

	bool isVisible[STAGE_ROW * STAGE_COL]{};
	bool isFlag[STAGE_ROW * STAGE_COL]{};

	while (true)
	{
		system("cls");
		
		Draw(stage, STAGE_ROW, STAGE_COL, isVisible, isFlag);

		int playerRow, playerCol;
		char flag;
		std::cout << "Enter your move(row and column) and f = flag, c = click : ";
		std::cin >> playerRow >> playerCol >> flag;

		if ((playerRow < 0 || playerRow > STAGE_ROW - 1 ) || (playerCol < 0 || playerCol > STAGE_COL - 1) ||
			((flag != 'f' && flag != 'F') && (flag != 'c' && flag != 'C')))
		{
			std::cout << "Input correct row and col and f, c" << std::endl;
			Sleep(1000);
			continue;
		}
		
		if (FlagPlaced(stage, STAGE_ROW, STAGE_COL, playerRow, playerCol, isFlag, flag))
		{
			continue;
		}

		Visible(stage, STAGE_ROW, STAGE_COL, playerRow, playerCol, isVisible, isFlag);

		if (stage[playerRow * STAGE_COL + playerCol] == OBJ_MINE)
		{
			system("cls");
			Draw(stage, STAGE_ROW, STAGE_COL, isVisible, isFlag);
			std::cout << "Game Over!" << std::endl;
			break;
		}

		if (IsClear(stage, STAGE_ROW, STAGE_COL, isVisible))
		{
			system("cls");
			Draw(stage, STAGE_ROW, STAGE_COL, isVisible, isFlag);
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
		
		stage[random] = OBJ_MINE;
		setMine[random] = true;
		
	}
}


void Draw(const Object *stage, int row, int col, const bool *isVisible, const bool *isFlag)
{
	static const char font[]{ 'X', '*', ' ', '1', '2', '3', '4', '5', '6', '7', '8'};

	std::cout << "                ";
	for (int i = 0; i < row; ++i)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;

	std::cout << "                ";
	for (int i = 0; i < row; ++i)
	{
		std::cout << "- ";
	}
	std::cout << std::endl;

	for (int y = 0; y < row; y++)
	{
		std::cout << "            ";
		std::cout << y << " | ";
		for (int x = 0; x < col; x++)
		{
			Object o = stage[y * col + x];
			
			if (isVisible[col * y + x])
			{
				std::cout << font[o] << " ";
			}
			else if (isFlag[y * col + x])
			{
				std::cout << "F ";
			}
			else
			{
				std::cout << "* ";
			}
		}

		std::cout << std::endl;
	}
}

bool FlagPlaced(const Object *stage, int row, int col, int playerRow, int playerCol, bool *isFlag, char flag)
{
	if ((flag == 'c' || flag == 'C') && flag != 'f' && flag != 'F')
	{
		return false;
	}

	if (isFlag[playerRow * col + playerCol])
	{
		isFlag[playerRow * col + playerCol] = false;
		return true;
	}

	isFlag[playerRow * col + playerCol] = true;

	return true;
}

void Visible(const Object *stage, int row, int col, int playerRow, int playerCol, bool *isVisible, bool *isFlag)
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
					Visible(stage, row, col, playerRow + dy, playerCol + dx,  isVisible, isFlag);
				}
			}
		}
	}
	else if (stage[playerRow * col + playerCol] == OBJ_MINE) 
	{
		return;
	}
}

bool IsClear(const Object *stage, int row, int col, const bool *IsVisible)
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