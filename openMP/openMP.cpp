#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

vector<vector<int>> CreateMatrix(int, int);
void DisplayMatrix(vector<vector<int>>);
vector<vector<int>> MultiplyMatrices(vector<vector<int>>, vector<vector<int>>);

const int LIMIT = 100;

int main()
{	
	srand(static_cast<int>(time(nullptr)));
	try
	{
		auto firstMatrix = CreateMatrix(10, 10);
		auto secondMatrix = CreateMatrix(10, 10);
		auto resultMatrix = MultiplyMatrices(firstMatrix, secondMatrix);

		cout << "First Matrix : " << endl;
		DisplayMatrix(firstMatrix);
		cout << endl << endl;

		cout << "Second Matrix : " << endl;
		DisplayMatrix(secondMatrix);
		cout << endl << endl;
		
		cout << "Result Matrix : " << endl;
		DisplayMatrix(resultMatrix);
	}
	catch(invalid_argument &exception)
	{
		cout << exception.what();
	}

	getchar();
	return 0;
}

vector<vector<int>> CreateMatrix(int numberOfRows, int numberOfColumns)
{
	if( numberOfRows == 0 || numberOfColumns == 0 )
		throw invalid_argument("The number of rows or columns is equal with 0 !");
	
	vector<vector<int>> matrix(numberOfRows, vector<int>(numberOfColumns, 0));
	for (int row = 0; row < numberOfRows; ++row)
		for (int col = 0; col < numberOfColumns; ++col)
			matrix[row][col] = rand() % LIMIT;
	return matrix;
}

vector<vector<int>> MultiplyMatrices(vector<vector<int>> firstMatrix, vector<vector<int>> secondMatrix)
{
	if (firstMatrix[0].size() != secondMatrix.size())
		throw invalid_argument("You can not multiply this matices !");

	auto numberOfRows = firstMatrix.size();
	auto numberOfColumns = secondMatrix[0].size();

	vector<vector<int>> resultMatrix(numberOfRows, vector<int>(numberOfColumns, 0));
	
	int row = 0;

	omp_set_num_threads(4);
#pragma omp parallel for default(none) shared( firstMatrix, secondMatrix, resultMatrix ) //private(row)

	for ( row = 0; row < numberOfRows; ++row)
	{
		//cout << "Row : " << row << " - Thread ID : " << omp_get_thread_num() << endl;
		for (int col = 0; col < numberOfColumns; ++col)
			for (int k = 0; k < numberOfColumns; ++k)
				resultMatrix[row][col] += firstMatrix[row][k] * secondMatrix[k][col];
	}
		
	return resultMatrix;
}

void DisplayMatrix(vector<vector<int>> resultMatrix)
{
	for (int row = 0; row < resultMatrix.size(); ++row)
	{
		for (int col = 0; col < resultMatrix[row].size(); ++col)
			cout << resultMatrix[row][col] << " ";
		cout << endl;
	}
}
