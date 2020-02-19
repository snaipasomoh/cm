#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>

typedef std::vector<std::vector<double>> Matrix;

void printMatrix (Matrix const &matrix){
	for (std::vector<double> i : matrix){
		for (double j : i){
			std::cout << std::setw(10) << j << " ";
		}
		std::cout << std::endl;
	}
}

void forwStep (Matrix &matrix, double eps){
	size_t len = matrix.size();
	for (size_t i = 0; i < len; i++){
		size_t maxInd = i;
		double max = std::abs(matrix[i][i]);
		for (size_t j = i; j < len; j++){
			if (std::abs(matrix[j][i]) > max){
				maxInd = j;
				max = std::abs(matrix[j][i]);
			}
		}
		if (maxInd != i){
			std::swap(matrix[i], matrix[maxInd]);
		}

		double div = matrix[i][i];
		for (size_t j = i; j < len + 1; j++){
			matrix[i][j] /= div;
		}

		for (size_t j = i + 1; j < len; j++){
			double mult = matrix[j][i] / matrix[i][i];
			for (size_t k = i; k < len + 1; k++){
				matrix[j][k] -= matrix[i][k] * mult;
				if (std::abs(matrix[j][k]) < eps){
					matrix[j][k] = 0;
				}
			}
		}
	}
}

std::vector<double> backStep (Matrix &matrix){
	size_t len = matrix.size();
	std::vector<double> res (len, 0);
	for (size_t i = 0; i < len; i++){
		res[len - 1 - i] = matrix[len - 1 - i][len];
		for (size_t j = 0; j < len - 1 - i; j++){
			matrix[j][len] -= res[len - 1 - i] * matrix[j][len - 1 - i];
		}
		printMatrix(matrix);
		std::cout << std::endl;
	}
	return res;
}

std::vector<double> gauss (Matrix matrix, double eps = 0.000001){
	forwStep(matrix, eps);
	return backStep(matrix);
}

int main (int argc, char **argv){
	std::vector<std::vector<double>> matrix = {{-2, -9, -3,  7, -26},
											   {-7,  2,  2,  5, -25},
											   {-6,  2,  0,  0, -16},
											   { 0, -3,  8, -3,  -5}};


	std::vector<double> res = gauss(matrix);
	for (auto i : res){
		std::cout << i << " ";
	}
	std::cout << std::endl;
}
