// Name: zero871015
// Date: 2018/05/04
// Last Update: 2018/05/10
// Problem statement: Sudoku Solver

#include <iostream>
#include <string>
#include <fstream>
#define N 9

const std::string FILE_NAME = "Question.txt";

void print(char a[N][N]);
bool load();
bool findMoreTips();
bool solver(char a[N][N], int row, int column);
void findCeilNumbers(int row, int column);
bool fillInNumber(int row, int column);
bool checker(char a[N][N]);
void iniIsUsed(bool u[N]);

//a table to record all of the ceil can fill in what number
std::string number[N*N];

//sudoku table, 'n' is non-number.
char a[N][N] = { 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n',
				 'n','n','n','n','n','n','n','n','n' };

//judge whether the ceil is sure answer
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
		//check whether the sudoku is legal
		if (checker(a))
		{
			//fill up number by hidden single and naked single
			do
			{
				for (int i = 0; i < N*N; i++)
				{
					findCeilNumbers(i / N, i%N);
				}
			} while (findMoreTips());
			//brute force to find the answer
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
		//illegal question
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
				//the number provided by file must be correct
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

//recursive to solve the sudoku
bool solver(char a[N][N], int row, int column)
{
	//get the next row
	int nextr = (column == N - 1) ? row + 1 : row;

	//get the next column
	int nextc = (column + 1) % N;

	//if now row bigger than N, the sudoku is finished
	if (row == N)return true;

	//if the ceil is setted by file or already calculate it, skip this ceil
	if (isComfirmed[row][column])
		return solver(a, nextr, nextc);

	//get the numbers can fill in now ceil
	std::string temp = number[row*N + column];

	//try possible numbers when the ceil is 'n'
	for (int i = 0; i < (int)temp.length(); i++)
	{
		char value = temp[i];
		a[row][column] = value;

		//if the map is OK now, try next ceil
		//through multiple judgments to prune
		if (fillInNumber(row, column) && solver(a, nextr, nextc))
			return true;
		//if the number is false, prune this branch to speed up
		a[row][column] = 'n';
		fillInNumber(row, column);
	}
	return false;
}

//update the numbers which can fill in this ceil
void findCeilNumbers(int row, int column)
{
	//if this ceil already have number, it can't fill any number.
	if (a[row][column] != 'n')
	{
		number[row*N + column].clear();
	}
	//if this ceil is 'n', find what number can fill in.
	else
	{
		std::string ans;
		bool r[N] = { true,true,true,true,true,true,true,true,true };
		bool c[N] = { true,true,true,true,true,true,true,true,true };
		bool b[N] = { true,true,true,true,true,true,true,true,true };
		int indexOfBox = (row / 3) * 3 + column / 3;	//the ceil in which box
		//row check
		for (int i = 0; i < N; i++)
		{
			if (a[i][column] != 'n')
				r[a[i][column] - '1'] = 0;
		}
		//column check
		for (int i = 0; i < N; i++)
		{
			if (a[row][i] != 'n')
				c[a[row][i] - '1'] = 0;
		}
		//box check
		for (int i = 0; i < N; i++)
		{
			if (a[(indexOfBox / 3) * 3 + i / 3][(indexOfBox % 3) * 3 + i % 3] != 'n')
				b[a[(indexOfBox / 3) * 3 + i / 3][(indexOfBox % 3) * 3 + i % 3] - '1'] = 0;
		}
		for (int i = 0; i < N; i++)
		{
			//if conform row, column and box at same time, add the number in
			if (r[i] && c[i] && b[i])
				ans += i + '1';
		}
		number[row*N + column] = ans;
	}
}

//when you fill a number in a ceil,
//updata all the ceils which is affected
//don't updata all ceils to save time
bool fillInNumber(int row, int column)
{
	int indexOfBox = (row / 3) * 3 + column / 3;	//the ceil in which box
	for (int i = 0; i < N; i++)
	{
		//row part
		findCeilNumbers(row, i);
		//when the ceil updata, check it have number to use when it is 'n'
		if (a[row][i] == 'n' && !number[row*N + i].length())
			return false;	//if there is no number can fill in, prune this branch

		//column part
		findCeilNumbers(i, column);
		//when the ceil updata, check it have number to use when it is 'n'
		if (a[i][column] == 'n' && !number[i*N + column].length())
			return false;	//if there is no number can fill in, prune this branch

		//box part
		findCeilNumbers((indexOfBox / 3) * 3 + i / 3, (indexOfBox % 3) * 3 + i % 3);
		//when the ceil updata, check it have number to use when it is 'n'
		if (a[(indexOfBox / 3) * 3 + i / 3][(indexOfBox % 3) * 3 + i % 3] == 'n' && !number[(indexOfBox / 3)*N * 3 + (indexOfBox % 3) * 3 + i % 3 + (i / 3)*N].length())
			return false;	//if there is no number can fill in, prune this branch
	}
	return true;
}

//through the numbers file provided to calculate other definite number.
bool findMoreTips()
{
	bool ans = false;
	//row part
	for (int i = 0; i < N; i++)	//each row
	{
		for (char c = '1'; c < '9'; c++)	//each number
		{
			int count = 0;
			for (int j = 0; j < N; j++)	//each ceil in row
			{
				if (a[i][j] == c)	//already have this number
				{
					count = 0;
					break;
				}
				else
				{
					//there is a ceil can fill in the number
					if ((int)number[i*N + j].find(c) != -1)
						count++;
				}
			}
			//if only one ceil can fill in the number
			if (count == 1)
			{
				for (int j = 0; j < N; j++)
				{
					//find which ceil
					if ((int)number[i*N + j].find(c) != -1)
					{
						a[i][j] = c;
						isComfirmed[i][j] = true;
						ans = true;
					}
				}
			}
		}
	}
	//column part
	for (int j = 0; j < N; j++)	//each column
	{
		for (char c = '1'; c < '9'; c++)	//each number
		{
			int count = 0;
			for (int i = 0; i < N; i++)	//each ceil in this colume
			{
				if (a[i][j] == c)	//already have this number
				{
					count = 0;
					break;
				}
				else
				{
					//there is a ceil can fill in the number
					if ((int)number[i*N + j].find(c) != -1)
						count++;
				}
			}
			//if only one ceil can fill in the number
			if (count == 1)
			{
				for (int i = 0; i < N; i++)
				{
					//find which ceil
					if ((int)number[i*N + j].find(c) != -1)
					{
						a[i][j] = c;
						isComfirmed[i][j] = true;
						ans = true;
					}
				}
			}
		}
	}
	//box part
	for (int i = 0; i < N; i++)	//each box
	{
		for (char c = '1'; c <= '9'; c++)	//each number
		{
			int count = 0;
			for (int j = 0; j < N; j++)	//each ceil in this box
			{
				if (a[(i / 3) * 3 + j / 3][(i % 3) * 3 + j % 3] == c)	//already have this number
				{
					count = 0;
					break;
				}
				else
				{
					//there is a ceil can fill in the number
					if ((int)number[(i % 3) * 3 + (i / 3)*N * 3 + j % 3 + (j / 3)*N].find(c) != -1)
					{
						count++;
					}
				}
			}
			//if only one ceil can fill in the number
			if (count == 1)
			{
				for (int j = 0; j < N; j++)
				{
					//find which ceil
					if ((int)number[(i % 3) * 3 + (i / 3)*N * 3 + j % 3 + (j / 3)*N].find(c) != -1)
					{
						a[(i / 3) * 3 + j / 3][(i % 3) * 3 + j % 3] = c;
						isComfirmed[(i / 3) * 3 + j / 3][(i % 3) * 3 + j % 3] = true;
						ans = true;
					}
				}
			}


		}
	}
	//return whether find a new ceil
	return ans;
}

//check the question is legal
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
		for (int i = 0; i < N / 3; i++)
		{
			for (int j = 0; j < N / 3; j++)
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
