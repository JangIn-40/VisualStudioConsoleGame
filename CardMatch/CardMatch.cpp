#include <iostream>
#include <random>
#include <chrono>
#include <thread>

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

void Draw(const Object *stage, int row, int col, const bool isMatch[], bool isVisible[], bool player1Turn, int playerScores[])
{
	static const char font[]{ '*', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
	
	std::cout << "    ";
	for (int i = 0; i < row; ++i)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;

	std::cout << "    ";
	for (int i = 0; i < row; ++i)
	{
		std::cout << "- ";
	}
	std::cout << std::endl;

	for (int y = 0; y < row; ++y)
	{
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

	std::cout << "Player 1 Score : " << playerScores[0] << std::endl;
	std::cout << "Player 2 Score : " << playerScores[1] << std::endl;

	if (player1Turn)
	{
		std::cout << std::endl << "Player 1 Turn" << std::endl;
	}
	else
	{
		std::cout << std::endl << "Player 2 Turn" << std::endl;
	}
}

bool IsMatch(const Object *stage, int col, int firstRow, int firstCol, int secondRow, int secondCol)
{
	return stage[firstRow * col + firstCol] == stage[secondRow * col + secondCol];
}

bool IsClear(int row, int col, bool isMatch[])
{
	for (int y = 0; y < row; ++y)
	{
		for (int x = 0; x < col; ++x)
		{
			if (!isMatch[y * col + x])
			{
				return false;
			}
		}
	}
	return true;
}

// 0번 인덱스 1번플레이어 점수,
// 1번 인덱스 2번플레이어 점수
void CountScore(int playerScores[], bool player1Turn)
{
	playerScores[player1Turn ? 0 : 1]++;
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

	int playerScores[2]{};
	bool isMatch[STAGE_ROW * STAGE_COL]{};
	bool isVisible[STAGE_ROW * STAGE_COL]{};
	bool showMoment{};
	bool player1Turn{ true };

	while (true)
	{
		system("cls");

		Draw(stage, STAGE_ROW, STAGE_COL, isMatch, isVisible, player1Turn, playerScores);

		if (showMoment)
		{
			std::cout << "No match. Try again";
			showMoment = false;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			continue;
		}

		int firstRow, firstCol, secondRow, secondCol;
		std::cout << "Enter coordinates of fist card(row colum) : ";
		std::cin >> firstRow >> firstCol;

		system("cls");
		isVisible[firstRow * STAGE_COL + firstCol] = true;
		Draw(stage, STAGE_ROW, STAGE_COL, isMatch, isVisible, player1Turn, playerScores);

		std::cout << "Enter coordinates of second card(row colum) : ";
		std::cin >> secondRow >> secondCol;

		if ((firstRow < 0 || firstRow > STAGE_ROW - 1) || (firstCol < 0 || firstCol > STAGE_COL - 1) ||
			(secondRow < 0 || secondRow > STAGE_ROW - 1) || (secondCol < 0 || secondCol > STAGE_COL - 1))
		{
			std::cout << std::endl << "Invalid Input" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			continue;
		}

		if (IsMatch(stage, STAGE_COL, firstRow, firstCol, secondRow, secondCol))
		{
			isMatch[firstRow * STAGE_COL + firstCol] = true;
			isMatch[secondRow * STAGE_COL + secondCol] = true;
			CountScore(playerScores, player1Turn);
		}
		else
		{
			isVisible[firstRow * STAGE_COL + firstCol] = true;
			isVisible[secondRow * STAGE_COL + secondCol] = true;
			showMoment = true;
			player1Turn = !player1Turn;
		}

		if(IsClear(STAGE_ROW, STAGE_COL, isMatch))
		{
			system("cls");
			Draw(stage, STAGE_ROW, STAGE_COL, isMatch, isVisible, player1Turn, playerScores);
			break;
		}
	}

	delete[] stage;
}