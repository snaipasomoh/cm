#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
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

Matrix rotateMatrix (Matrix const &matrix,
	                       std::pair<size_t, size_t> const &coords){
	size_t len = matrix.size();
	Matrix res = matrix;
	size_t x = coords.second;
	size_t y = coords.first;
	double theta;
	if (matrix[x][x] == matrix[y][y]){
		theta = M_PI / 4;
	}
	else{
		theta = std::atan(2 * matrix[y][x] / (matrix[x][x] - matrix[y][y])) / 2;
	}
	double c = std::cos(theta);
	double s = std::sin(theta);

	for (size_t i = 0; i < len; i++){
		for (size_t j = i; j < len; j++){
			if (i == y && j == y){
				res[i][j] = c * c * matrix[y][y] + s * s * matrix[x][x] -
				            2 * s * c * matrix[y][x];
			}
			else if (i == x && j == x){
				res[i][j] = s * s * matrix[y][y] + c * c * matrix[x][x] +
				            2 * s * c * matrix[y][x];
			}
			else if (i == y && j == x){
				res[i][j] = res[j][i] = (c * c - s * s) * matrix[y][x] +
				                        s * c * (matrix[y][y] - matrix[x][x]);
			}
			else if (i == y){
				res[i][j] = res[j][i] = c * matrix[y][j] - s * matrix[x][j];
			}
			else if (i == x){
				res[i][j] = res[j][i] = s * matrix[y][j] + c * matrix[x][j];
			}
		}
	}
	return res;
}

Matrix jacobi (Matrix const &matrix, double eps = 0.001){
	Matrix m = matrix;
	std::pair<size_t, size_t> coords = findPivot(matrix);
	double pivot = std::abs(m[coords.first][coords.second]);
	while (pivot > eps){
		m = rotateMatrix(m, coords);
		coords = findPivot(m);
		pivot = std::abs(m[coords.first][coords.second]);
	}
	return m;
}

int main (int argc, char **argv){
	Matrix matrix = {{ 2, -1},
	                 {-1, -3}};

	matrix = jacobi(matrix);
	printMatrix(matrix);
}
