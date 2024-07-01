#include <iostream>
#include <random>

enum AIMode
{
	AIEASY = 1,
	AIHARD,
};

enum CountStar
{
	LN_H1,
	LN_H2,
	LN_H3,
	LN_H4,
	LN_H5,
	LN_V1,
	LN_V2,
	LN_V3,
	LN_V4,
	LN_V5,
	LN_RT,
	LN_LT,
};

void Print(const int num[], int row, int col)
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			if (num[i * 5 + j] != 0)
			{
				std::cout << num[i * row + j] << "\t";
			}
			else
			{
				std::cout << "*\t";
			}
		}
		std::cout << std::endl;
	}
}

void CreateRandomNumbers(int outArray[], int maxArrayIndex)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, maxArrayIndex);

	for (int i = 0; i < 100; ++i)
	{
		int randNum = dist(mt);
		int randNum2 = dist(mt);
		std::swap(outArray[randNum], outArray[randNum2]);
	}
}

void AIEasyMode(int outPlayerArray[], int outAIArray[], int maxNumber, bool setStar[])
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(1, maxNumber);

	int aiStar;
	do
	{
		aiStar = dist(mt);
	} while (setStar[aiStar - 1]);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (outPlayerArray[i * 5 + j] == aiStar)
			{
				outPlayerArray[i * 5 + j] = 0;
			}
			if (outAIArray[i * 5 + j] == aiStar)
			{
				outAIArray[i * 5 + j] = 0;
				setStar[aiStar - 1] = true;
			}
		}
	}
}

CountStar CountingLargeStarLine(const int AIArray[])
{
	int rowStar{}, colStar{}, rightDiagonalStar{}, leftDiagonalStar{}, maxStar{}, line{};
	for (int i = 0; i < 5; ++i)
	{
		rowStar = colStar = 0;
		for (int j = 0; j < 5; ++j)
		{
			if (AIArray[i * 5 + j] == 0)
				++rowStar;
			if (AIArray[j * 5 + i] == 0)
				++colStar;
			if (j == i && AIArray[i * 6] == 0)
				++rightDiagonalStar;
			if (j == 4 - i && AIArray[i * 4 + 4] == 0)
				++leftDiagonalStar;

			if (colStar < 5 && colStar > maxStar)
			{
				line = j + 5;
				maxStar = j;
			}
		}

		if (rowStar < 5 && rowStar > maxStar)
		{
			line = i;
			maxStar = rowStar;
		}

	}
	if (rightDiagonalStar < 5 && rightDiagonalStar > maxStar)
	{
		line = LN_RT;
		maxStar = rightDiagonalStar;
	}
	if (leftDiagonalStar < 5 && leftDiagonalStar > maxStar)
	{
		line = LN_LT;
		maxStar = leftDiagonalStar;
	}

	return (CountStar)line;
}

void AIHardMode(int outPlayerArray[], int outAIArray[], bool setStar[])
{
	CountStar maxStar = CountingLargeStarLine(outAIArray);

	int aiSelectNum;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (maxStar > LN_H5 && maxStar <= LN_V5 && outAIArray[maxStar * 5 + j] != 0)
			{
				aiSelectNum = outAIArray[maxStar * 5 + j];
				outAIArray[maxStar * 5 + j] = 0;
				setStar[maxStar * 5 + j] = true;
			}
			if (maxStar == LN_RT && j == i && outAIArray[i * 6] != 0)
			{
				aiSelectNum = outAIArray[i * 6];
				outAIArray[i * 6] = 0;
				setStar[i * 6] = true;
			}
			if (maxStar == LN_LT && j == 4 - i && outAIArray[i * 4 + 4] == 0)
			{
				aiSelectNum = outAIArray[i * 4 + 4];
				outAIArray[i * 4 + 4] = 0;
				setStar[i * 4 + 4] = true;
			}
		}

		if (maxStar <= LN_H5 && outAIArray[maxStar * 5 + i] != 0)
		{
			aiSelectNum = outAIArray[maxStar * 5 + i];
			outAIArray[maxStar * 5 + i] = 0;
			setStar[maxStar * 5 + i] = true;
			break;
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (outPlayerArray[i * 5 + j] == aiSelectNum)
			{
				outPlayerArray[i * 5 + j] = 0;
			}
		}
	}
}

void CountBingoLine(const int outArray[], int &outBingoLine)
{
	int rowStar{}, colStar{}, rightDiagonal{}, leftDiagonal{};
	for (int i = 0; i < 5; ++i)
	{
		rowStar = colStar = 0;
		for (int j = 0; j < 5; ++j)
		{
			if (outArray[i * 5 + j] == 0)
				++rowStar;
			if (outArray[j * 5 + i] == 0)
				++colStar;
			if (j == i && outArray[i * 6] == 0)
				++rightDiagonal;
			if (j == 4 - i && outArray[i * 4 + 4] == 0)
				++leftDiagonal;
		}

		if (rowStar == 5)
			++outBingoLine;
		if (colStar == 5)
			++outBingoLine;
	}
	if (rightDiagonal == 5)
		++outBingoLine;
	if (leftDiagonal == 5)
		++outBingoLine;
}

int main()
{
	int num[25];
	int AINum[25];

	for (int i = 0; i < 25; ++i)
	{
		num[i] = i + 1;
		AINum[i] = i + 1;
	}

	CreateRandomNumbers(num, 24);
	CreateRandomNumbers(AINum, 24);

	int ai;
	while (true)
	{
		system("cls");

		std::cout << "Choose AI Mode 1.Easy 2.Hard : ";
		std::cin >> ai;
		
		if (ai < 1 || ai > 2)
		{
			std::cout << "Choose correct number." << std::endl;
			system("pause");
		}
		else
			break;
	}
	
	int pNum, bingo{}, AIBingo{};
	bool setStar[25]{};
	while (true)
	{
		system("cls");

		std::cout << "-------------------- Player --------------------" << std::endl;
		Print(num, 5, 5);
		std::cout << "Number of Player Bingo " << bingo << std::endl << std::endl;

		switch (ai)
		{
		case AIEASY:
			std::cout << "-------------------- AI Easy --------------------" << std::endl;
			break;
		case AIHARD:
			std::cout << "-------------------- AI Hard --------------------" << std::endl;
			break;
		}		
		Print(AINum, 5, 5);
		std::cout << "Number of AI Bingo " << AIBingo << std::endl << std::endl;

		if (bingo >= 5)
		{
			std::cout << "Congratulatinos! You win AI Easy Mode! " << std::endl;
			break;
		}
		else if (AIBingo >= 5)
		{
			std::cout << "AI Win. But Cheer up! You will be Win next time";
			break;
		}
		
		std::cout << "1 ~ 25까지 숫자를 입력하세요" << std::endl;
		std::cin >> pNum;

		if (pNum < 1 || pNum > 25)
		{
			std::cout << "올바른 숫자를 입력하세요 " << std::endl;
			system("pause");
			continue;
		}
		
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				if (num[i * 5 + j] == pNum)
				{
					num[i * 5 + j] = 0;
				}
				if (AINum[i * 5 + j] == pNum)
				{
					AINum[i * 5 + j] = 0;
					setStar[pNum - 1] = true;
				}
			}
		}

		switch (ai)
		{
		case AIEASY:
			AIEasyMode(num, AINum, 25, setStar);
			break;
		case AIHARD:
			AIHardMode(num, AINum, setStar);
			break;
		default:
			break;
		}

		bingo = AIBingo = 0;
		CountBingoLine(num, bingo);
		CountBingoLine(AINum, AIBingo);
		
	}
}
