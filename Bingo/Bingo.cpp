// Bingo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
int main()
{
	int num[25];

	for (int i = 0; i < 25; ++i)
	{
		num[i] = i + 1;
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 24);

	for (int i = 0; i < 100; ++i)
	{
		int randNum = dist(mt);
		int randNum2 = dist(mt);
		std::swap(num[randNum], num[randNum2]);
	}

	int pNum, bingo{};
	while (true)
	{
		system("cls");

		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				if (num[i * 5 + j] != 0)
				{
					std::cout << num[i * 5 + j] << "\t";
				}
				else
				{
					std::cout << "*\t";
				}

			}
			std::cout << std::endl;
		}

		if (bingo == 5)
		{
			std::cout << "Congratulatinos!" << std::endl;
			break;
		}

		std::cout << "Number of Bingo " << bingo << std::endl;
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
			}
		}

		bingo = 0;
		int rowStar{}, colStar{}, rightDiagonal{}, leftDiagonal{};
		for (int i = 0; i < 5; ++i)
		{
			rowStar = colStar = 0;
			for (int j = 0; j < 5; ++j)
			{
				if (num[i * 5 + j] == 0)
					++rowStar;
				if (num[j * 5 + i] == 0)
					++colStar;
				if (j == i && num[i * 6] == 0)
					++rightDiagonal;
				if (j == 4 - i && num[i * 4 + 4] == 0)
					++leftDiagonal;
			}

			if (rowStar == 5)
				++bingo;
			if (colStar == 5)
				++bingo;
		}
		if (rightDiagonal == 5)
			++bingo;
		if (leftDiagonal == 5)
			++bingo;
			
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
