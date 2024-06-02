#include <iostream>
#include <random>
#include <Windows.h>

const char STAGE_DATA[] = "\
****\n\
****\n\
****\n\
****";

const int STAGE_ROW{ 4 };
const int STAGE_COL{ 4 };

enum Object
{
	OBJ_STAR,
	OBJ_A,
	OBJ_B,
	OBJ_C,
	OBJ_D,
	OBJ_E,
	OBJ_F,
	OBJ_G,
	OBJ_H,

	OBJ_MAX
};

void GenerateRandomAlphabet(Object *stage, int row, int col)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, row * col - 1);

	bool *setAlphabet = new bool[row * col] {};
	for (int i = 0; i < row * col; ++i)
	{
		int random;
		do
		{
			random = dist(mt);
		} while (setAlphabet[random]);

		stage[random] = Object(OBJ_A + i / 2);
		setAlphabet[random] = true;
	}

	delete[] setAlphabet;
}

void Draw(const Object *stage, int row, int col, const bool isMatch[], bool isVisible[])
{
	static const char font[]{ '*', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

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

	for (int y = 0; y < row; ++y)
	{
		std::cout << "            ";
		std::cout << y << " | ";
		for (int x = 0; x < col; ++x)
		{
			Object o = stage[y * col + x];

			if (isMatch[col * y + x])
			{
				std::cout << font[o] << " ";
			}
			else if(isVisible[col * y + x])
			{
				isVisible[col * y + x] = false;
				std::cout << font[o] << " ";
			}
			else
			{
				std::cout << "* ";
			}
		}

		std::cout << std::endl;
	}
}

bool IsMatch(const Object *stage, int col, int firstRow, int firstCol, int secondRow, int secondCol)
{
	if (stage[firstRow * col + firstCol] == stage[secondRow * col + secondCol])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsClear(int row, int col, bool isMatch[])
{
	for (int y = 0; y < row; ++y)
	{
		for (int x = 0; x < col; ++x)
		{
			if (isMatch[y * col + x] == false)
			{
				return false;
			}
		}
	}
	return true;
}

int main()
{
	Object *stage = new Object[STAGE_ROW * STAGE_COL];
	if (!stage)
	{
		std::cout << "memeory leak in main()" << std::endl;
		return -1;
	}

	GenerateRandomAlphabet(stage, STAGE_ROW, STAGE_COL);

	bool isMatch[STAGE_ROW * STAGE_COL]{};
	bool isVisible[STAGE_ROW * STAGE_COL]{};
	bool showMoment{};

	while (true)
	{
		system("cls");

		Draw(stage, STAGE_ROW, STAGE_COL, isMatch, isVisible);

		if (showMoment)
		{
			std::cout << "No match. Try again";
			showMoment = false;
			Sleep(1000);
			continue;
		}

		int firstRow, firstCol, secondRow, secondCol;
		std::cout << "Enter coordinates of fist card(row colum) : ";
		std::cin >> firstRow >> firstCol;
		std::cout << "Enter coordinates of second card(row colum) : ";
		std::cin >> secondRow >> secondCol;

		if ((firstRow < 0 && firstRow > STAGE_ROW - 1) || (firstCol < 0 && firstCol > STAGE_COL - 1))
		{
			std::cout << std::endl << "Invalid Input" << std::endl;
			Sleep(1000);
			continue;
		}

		if (IsMatch(stage, STAGE_COL, firstRow, firstCol, secondRow, secondCol))
		{
			isMatch[firstRow * STAGE_COL + firstCol] = true;
			isMatch[secondRow * STAGE_COL + secondCol] = true;
		}
		else
		{
			isVisible[firstRow * STAGE_COL + firstCol] = true;
			isVisible[secondRow * STAGE_COL + secondCol] = true;
			showMoment = true;
		}

		if(IsClear(STAGE_ROW, STAGE_COL, isMatch))
		{
			system("cls");
			Draw(stage, STAGE_ROW, STAGE_COL, isMatch, isVisible);
			break;
		}
	}

	std::cout << "Congratulation" << std::endl;
}