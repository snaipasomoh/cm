#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
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

std::pair<size_t, size_t> findPivot (Matrix const &matrix){
	size_t x = 1;
	size_t y = 0;
	double max = std::abs(matrix[0][1]);
	size_t len = matrix.size();

	for (size_t i = 0; i < len; i++){
		for (size_t j = 0; j < len; j++){
			if (i == j){
				continue;
			}
			if (std::abs(matrix[i][j]) > max){
				max = std::abs(matrix[i][j]);
				y = i;
				x = j;
			}
		}
	}
	return std::make_pair(y, x);
}

double nonDiagSqSum (Matrix const &matrix){
	size_t len = matrix.size();
	double sqSum = 0;
	for (size_t i = 0; i < len; i++){
		for (size_t j = i + 1; j < len; j++){
			sqSum += matrix[i][j] * matrix[i][j];
		}
	}
	return std::sqrt(sqSum);
}

Matrix mult (Matrix const &a, Matrix const &b){
	size_t height = a.size();
	size_t width = b[0].size();
	size_t len = b.size();
	Matrix res (height, std::vector<double> (width, 0));

	for (size_t i = 0; i < height; i++){
		for (size_t j = 0; j < width; j++){
			for (size_t k = 0; k < len; k++){
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return res;
}

Matrix makeRotMatrix (Matrix const &matrix, std::pair<size_t, size_t> pivot){
	double theta = M_PI / 4;
	size_t y = pivot.first;
	size_t x = pivot.second;
	if (matrix[y][y] != matrix[x][x]){
		theta = std::atan(2 * matrix[y][x] / (matrix[y][y] - matrix[x][x])) / 2;
	}
	size_t len = matrix.size();
	Matrix res (len, std::vector<double> (len, 0));
	for (size_t i = 0; i < len; i++){
		res[i][i] = 1;
	}
	res[y][x] = -std::sin(theta);
	res[x][y] = std::sin(theta);
	res[x][x] = res[y][y] = std::cos(theta);
	return res;
}

Matrix transpose (Matrix const &matrix){
	size_t len = matrix.size();
	Matrix res = matrix;
	for (size_t i = 0; i < len; i++){
		for (size_t j = 0; j < len; j++){
			res[i][j] = matrix[j][i];
		}
	}
	return res;
}

std::vector<double> jacobi (Matrix matrix, double eps = 0.01){
	while (nonDiagSqSum(matrix) > eps){
		Matrix rotMatrix = makeRotMatrix(matrix, findPivot(matrix));
		matrix = mult(transpose(rotMatrix), matrix);
		matrix = mult(matrix, rotMatrix);
		// std::cout << nonDiagSqSum(matrix) << std::endl;
		// std::cout << std::endl << "iter " << count << std::endl;
		// printMatrix(matrix);
		// std::cout << std::endl;
		// printMatrix(rotMatrix);
		// std::cout << std::endl;
		// count++;
	}
	std::vector<double> res;
	for (size_t i = 0; i < matrix.size(); i++){
		res.push_back(matrix[i][i]);
	}
	return res;
}

Matrix forwStep (Matrix const &matrix, double eps = 0.0001){
	size_t len = matrix.size();
	Matrix m = matrix;
	for (size_t i = 0; i < len; i++){
		double max = std::abs(m[i][i]);
		size_t maxInd = i;
		for (size_t j = i + 1; j < len; j++){
			if (std::abs(m[j][i]) < eps){
				m[j][i] = 0;
			}
			if (std::abs(m[j][i]) > max){
				max = std::abs(m[j][i]);
				maxInd = j;
			}
		}
		if (max < eps){
			continue;
		}

		if (maxInd != i){
			std::swap(m[maxInd], m[i]);
		}

		for (size_t j = 0; j < len; j++){
			if (j == i){
				continue;
			}
			double mult = m[j][i] / m[i][i];
			for (size_t k = i; k < len; k++){
				m[j][k] -= m[i][k] * mult;
				if (std::abs(m[j][k]) < eps){
					m[j][k] = 0;
				}
			}
		}

		double div = m[i][i];
		for (size_t j = i; j < len; j++){
			m[i][j] /= div;
			if (std::abs(m[i][j]) < eps){
				m[i][j] = 0;
			}
		}
	}
	return m;
}

std::vector<double> backStep (Matrix matrix, std::vector<double> freePart){
	size_t len = matrix.size();
	std::vector<double> res (len, 0);
	freePart[len - 1] = 1;
	for (size_t i = 0; i < len; i++){
		res[len - 1 - i] = freePart[len - 1 - i];
		for (size_t j = 0; j < len - 1 - i; j++){
			freePart[j] -= matrix[j][len - 1 - i] * res[len - 1 - i];
		}
	}
	return res;
}

int main (int argc, char **argv){
	Matrix matrix = {{-4,  1, -7},
	                 { 1,  9,  1},
	                 {-7,  1,  7}};

	std::vector<double> eigenValues = jacobi(matrix);

	for (size_t i = 0; i < eigenValues.size(); i++){
		std::cout << "Eigenvalue " << i << ": " << eigenValues[i] << std::endl;
		Matrix m = matrix;
		for (size_t j = 0; j < m.size(); j++){
			m[j][j] -= eigenValues[i];
		}
		m = forwStep(m);
		std::vector<double> eigenVector = backStep(m, {0, 0, 0});
		std::cout << "Eigenvector " << i << ": ";
		for (double j : eigenVector){
			std::cout << j << " ";
		}
		std::cout << std::endl << std::endl;
	}
}
