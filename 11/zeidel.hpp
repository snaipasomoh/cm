#ifndef ZEIDEL
#define ZEIDEL
#include <vector>
#include <utility>
#include <cmath>

typedef std::vector<std::vector<double>> Matrix;

double vecNorm (std::vector<double> const &x){
	double max = std::abs(x[0]);
	for (double i : x){
		if (abs(i) > max){
			max = abs(i);
		}
	}
	return max;
}

std::vector<double> subtract (std::vector<double> const &a,
	                          std::vector<double> const &b){
	std::vector<double> res;
	for (size_t i = 0; i < a.size(); i++){
		res.push_back(a[i] - b[i]);
	}
	return res;
}

std::vector<double> sum (std::vector<double> const &a,
	                     std::vector<double> const &b){
	std::vector<double> res;
	for (size_t i = 0; i < a.size(); i++){
		res.push_back(a[i] + b[i]);
	}
	return res;
}

std::vector<double> mult (Matrix const &matrix, std::vector<double> const &x){
	size_t len = x.size();
	std::vector<double> res;
	for (size_t i = 0; i < len; i++){
		double tempSum = 0;
		for (size_t j = 0; j < len; j++){
			tempSum += matrix[i][j] * x[j];
		}
		res.push_back(tempSum);
	}
	return res;
}

std::pair<Matrix, std::vector<double>> prepare (Matrix const &matrix,
	                                       std::vector<double> const &freePart){
	;
	Matrix resMatrix;
	std::vector<double> resFreePart;
	size_t len = freePart.size();

	for (size_t i = 0; i < len; i++){
		resFreePart.push_back(freePart[i] / matrix[i][i]);
	}

	for (size_t i = 0; i < len; i++){
		std::vector<double> temp;
		for (size_t j = 0; j < len; j++){
			temp.push_back(i == j ? 0 : -matrix[i][j] / matrix[i][i]);
		}
		resMatrix.push_back(temp);
	}
	return std::make_pair(resMatrix, resFreePart);
}

std::vector<double> zeidSum (Matrix const &matrix,
	                         std::vector<double> const &freePart,
	                         std::vector<double> const &x){
	std::vector<double> newX = x;
	size_t len = x.size();

	for (size_t i = 0; i < len; i++){
		double tempSum = freePart[i];
		for (size_t j = 0; j < len; j++){
			tempSum += newX[j] * matrix[i][j];
		}
		newX[i] = tempSum;
	}
	return newX;
}

std::vector<double> zeidel (Matrix const &matrix,
	                        std::vector<double> const &freePart,
	                        double eps = 0.01){
	auto foo = prepare(matrix, freePart);
	Matrix newMatrix = foo.first;
	std::vector<double> newFreePart = foo.second;
	std::vector<double> x = newFreePart;

	while (vecNorm(subtract(mult(matrix, x), freePart)) > eps){
		x = zeidSum(newMatrix, newFreePart, x);
	}
	return x;
}

#endif
