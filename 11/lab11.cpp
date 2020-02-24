#include <iostream>
#include <cmath>
#include <vector>
#include "zeidel.hpp"

double f1 (double x1, double x2){
	return std::cos(x2) / 4;
}

double f2 (double x1, double x2){
	return std::pow(M_E, x1) / 4;
}

std::vector<double> simIter (double f1(double, double),
	                         double f2(double, double), double eps = 0.001){
	size_t len = 2;
	std::vector<double> x(len, 0);
	std::vector<double> prevX = x;
	x[0] = f1(prevX[0], prevX[1]);
	x[1] = f2(prevX[0], prevX[1]);

	while (vecNorm(subtract(x, prevX)) > eps){
		prevX = x;
		x[0] = f1(prevX[0], prevX[1]);
		x[1] = f2(prevX[0], prevX[1]);
	}
	return x;
}

std::vector<double> zeidel (double f1(double, double),
	                        double f2(double, double), double eps = 0.001){
	size_t len = 2;
	std::vector<double> x(len, 0);
	std::vector<double> prevX = x;
	x[0] = f1(prevX[0], prevX[1]);
	x[1] = f2(prevX[0], prevX[1]);

	while (vecNorm(subtract(x, prevX)) > eps){
		prevX = x;
		x[0] = f1(x[0], x[1]);
		x[1] = f2(x[0], x[1]);
	}
	return x;
}

std::vector<double> newton (double eps = 0.001){
	std::vector<std::vector<double(*)(double, double)>> jacobian(2, {nullptr,
		                                                             nullptr});
	jacobian[0][0] = [](double x1, double x2){return 4.0;};
	jacobian[0][1] = [](double x1, double x2){return std::sin(x2);};
	jacobian[1][0] = [](double x1, double x2){return -std::pow(M_E, x1);};
	jacobian[1][1] = [](double x1, double x2){return 4.0;};

	std::vector<double(*)(double, double)> mFuncs (2, nullptr);
	mFuncs[0] = [](double x1, double x2){return - 4 * x1 + std::cos(x2);};
	mFuncs[1] = [](double x1, double x2){return - 4 * x2 + std::pow(M_E, x1);};

	std::vector<double> x (2, 0);
	std::vector<double> prevX = x;

	std::vector<std::vector<double>> W (2, {0, 0});
	std::vector<double> mF (2, 0);
	for (size_t i = 0; i < 2; i++){
		for (size_t j = 0; j < 2; j++){
			W[i][j] = jacobian[i][j](prevX[0], prevX[1]);
		}
		mF[i] = mFuncs[i](prevX[0], prevX[1]);
	}

	std::vector<double> dx = zeidel(W, mF, eps);
	x = sum(prevX, dx);

	while (vecNorm(subtract(x, prevX)) > eps){
		prevX = x;
		for (size_t i = 0; i < 2; i++){
			for (size_t j = 0; j < 2; j++){
				W[i][j] = jacobian[i][j](prevX[0], prevX[1]);
			}
			mF[i] = mFuncs[i](prevX[0], prevX[1]);
		}
		dx = zeidel(W, mF, eps);
		x = sum(prevX, dx);
	}
	return x;
}

int main (int argc, char **argv){
	std::vector<double> res = simIter(f1, f2);
	for (auto i : res){
		std::cout << i << std::endl;
	}
	std::cout << std::endl;
	res = zeidel(f1, f2);
	for (auto i : res){
		std::cout << i << std::endl;
	}
	std::cout << std::endl;
	res = newton();
	for (auto i : res){
		std::cout << i << std::endl;
	}
}
