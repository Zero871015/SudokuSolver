// Name: B10615032
// Date: 2018/05/04
// Last Update: 2018/05/05
// Problem statement: Sudoku Solver

#include <iostream>
#include <string>
#include <set>
#include <fstream>
#define N 9

const std::string FILE_NAME = "Question.txt";

void print(char a[N][N]);
bool load();
bool checker(char a[N][N]);
void iniIsUsed(bool u[N]);
bool solver(char a[N][N], int h, int w);
void findRegionNumbers(char a[N][N], int row, int column);
std::string findCeilNumbers(int row, int column);

std::string rowN[N];
std::string columnN[N];
std::string boxN[N];

//sudoku map, 'n' is non-number.
char a[N][N] = { 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n' };

//judge whether the ceil is initial tip
bool isComfirmed[N][N];

int main()
{
	//if can't read the file
	if (!load())
	{
		std::cout << "file loading error!" << std::endl;
	}
	else
	{
		findRegionNumbers(a, 0, 0);
		//find the answer
		if (solver(a, 0, 0))
		{
			print(a);
		}
		//no solution
		else
		{
			std::cout << "can't solver!" << std::endl;		
		}
	}
	system("pause");
	return 0;
}

// print the result
void print(char a[N][N])
{
	for (int i = 0; i < N; i++)
	{
		printf(" ---------  ---------  ---------\n");
		if (i == 3 || i == 6)
			printf(" ---------  ---------  ---------\n");

		for (int j = 0; j < N; j++)
		{
			if (j == 3 || j == 6)
				printf("¡U");
			printf("¡U%c", a[i][j]);
		}
		printf("¡U\n");
	}
	printf(" ---------  ---------  ---------\n");
}

// read the file to set map
bool load()
{
	std::fstream fin;
	fin.open(FILE_NAME, std::ios::in);

	//reading error
	if (!fin)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				fin >> a[i][j];
				if (a[i][j] != 'n')
					isComfirmed[i][j] = true;
				else
					isComfirmed[i][j] = false;
			}
		}
		fin.close();
		return true;
	}
}

// checking whether the sudoku is OK now
bool checker(char a[N][N])
{
	bool isUsed[N];
	//row check
	for (int i = 0; i < N; i++)
	{
		iniIsUsed(isUsed);
		for (int j = 0; j < N; j++)
		{
			if (a[i][j] != 'n')
			{
				if (isUsed[a[i][j] - '0' - 1])
					return false;
				else
					isUsed[a[i][j] - '0' - 1] = true;
			}
		}
	}
	//colume check
	for (int i = 0; i < N; i++)
	{
		iniIsUsed(isUsed);
		for (int j = 0; j < N; j++)
		{
			if (a[j][i] != 'n')
			{
				if (isUsed[a[j][i] - '0' - 1])
					return false;
				else
					isUsed[a[j][i] - '0' - 1] = true;
			}
		}
	}
	//box check
	for (int k = 0; k < N; k++)
	{
		iniIsUsed(isUsed);
		for (int i = 0; i < N/3; i++)
		{
			for (int j = 0; j < N/3; j++)
			{
				if (a[i + (k / 3) * 3][j + (k % 3) * 3] != 'n')
				{
					if (isUsed[a[i + (k / 3) * 3][j + (k % 3) * 3] - '0' - 1])
						return false;
					else
						isUsed[a[i + (k / 3) * 3][j + (k % 3) * 3] - '0' - 1] = true;
				}
			}
		}
	}
	return true;
}

// initialize the bool array which is used to record whether the number already appear.
void iniIsUsed(bool u[N])
{
	for (int i = 0; i < N; i++)
	{
		u[i] = false;
	}
}

bool solver(char a[N][N], int h, int w)
{
	system("CLS");
	print(a);
	//get the next height
	int nexth = (w == N - 1) ? h + 1 : h;

	//get the next width
	int nextw = (w + 1) % N;

	//if now height bigger than N, the sudoku is finished
	if (h == N)return true;

	//if the ceil is setted by question, skip this ceil
	if (isComfirmed[h][w])
		return solver(a, nexth, nextw);

	std::string temp = findCeilNumbers(h, w);
	if (temp.length() == 0)
		return false;

	//try 1~9 when the ceil is 'n'
	for (int i=0;i<(int)temp.length();i++)
	{
		char value = temp[i];
		a[h][w] = value;

		int indexOfBox = (h / 3) * 3 + w / 3;
		rowN[h] += value;
		columnN[w] += value;
		boxN[indexOfBox] += value;


		//if the map is OK now, try next ceil.
		if (solver(a, nexth, nextw))
			return true;
		//if the map is not OK, set it to 'n' and try next ceil.
		a[h][w] = 'n';
		
		int pos = rowN[h].find(value);
		rowN[h].erase(pos);
		pos = columnN[w].find(value);
		columnN[w].erase(pos);
		pos = boxN[indexOfBox].find(value);
		boxN[indexOfBox].erase(pos);
	}

	return false;
}

void findRegionNumbers(char a[N][N],int row, int column)
{
	//get the next row
	int nextr = (column == N - 1) ? row + 1 : row;

	//get the next column
	int nextc = (column + 1) % N;

	if (row != N)
	{
		if (a[row][column] >= '1'&&a[row][column] <= '9')
		{
			int indexOfBox = (row / 3) * 3 + column / 3;
			rowN[row] += a[row][column];
			columnN[column] += a[row][column];
			boxN[indexOfBox] += a[row][column];
		}
		findRegionNumbers(a, nextr, nextc);
	}

}

std::string findCeilNumbers(int row, int column)
{
	std::string ans;
	bool r[N] = { true,true,true,true,true,true,true,true,true };
	bool c[N] = { true,true,true,true,true,true,true,true,true };
	bool b[N] = { true,true,true,true,true,true,true,true,true };
	int indexOfBox = (row / 3) * 3 + column / 3;
	for (int i = 0; i < (int)rowN[row].size(); i++)
	{
		r[rowN[row][i]-'1'] = 0;
	}
	for (int i = 0; i < (int)columnN[column].size(); i++)
	{
		c[columnN[column][i] - '1'] = 0;
	}
	for (int i = 0; i < (int)boxN[indexOfBox].size(); i++)
	{
		b[boxN[indexOfBox][i] - '1'] = 0;
	}
	for (int i = 0; i < N; i++)
	{
		if (r[i] && c[i] && b[i])
			ans += i + '1';
	}
	return ans;
}

