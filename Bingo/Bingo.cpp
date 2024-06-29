#include <iostream>
#include <random>

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

void SetAIStar(int outPlayerArray[], int outAIArray[], int maxNumber, bool setStar[])
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

enum AIMode
{
	AIEASY = 1,
	AIHARD,
};

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
			SetAIStar(num, AINum, 25, setStar);
			break;
		case AIHARD:
			break;
		default:
			break;
		}

		bingo = AIBingo = 0;
		CountBingoLine(num, bingo);
		CountBingoLine(AINum, AIBingo);
		
	}
}
