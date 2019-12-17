#include <iostream>
#include <vector>
#include <cmath>
#include <simsym>
// #include <eigen3/Eigen/Dense>

Eigen::VectorXd solveLS (Eigen::MatrixXd const &A, Eigen::VectorXd const &b){
	return A.colPivHouseholderQr().solve(b);
}

std::vector<double> ssm (std::vector<double> const &x,
						 std::vector<double> const &y, size_t k){
	std::vector<double> xPows (0, k * k + 1);
	std::vector<double> yxPows (0, k + 1);
	size_t len = x.size();
	for (size_t i = 0; i < len; i++){
		for (size_t j = 0; j < k * k + 1; j++){
			xPows[j] += std::pow(x[i], j);
		}
		for (size_t j = 0; j < k + 1; j++){
			yxPows[j] += y[i] * std::pow(x[i], j);
		}
	}
	Eigen::MatrixXd A(k, k);
	Eigen::VectorXd b(k);
	for (size_t i = 0; i < k; i++){
		for (size_t j = 0; j < k; j++){
			A(i, j) = xPows[i + j];
		}
		b(i) = yxPows[i];
	}
	auto sol = solveLS(A, b);
	std::vector<double> res;
	for (size_t i = 0; i < k; i++){
		res.push_back(sol(i));
	}
	return res;
}

double getValInX (simsym const &pol, double x){
	double res = 0;
	for (size_t i = 0; i < pol.size(); i++){
		res += pol[i] * std::pow(x, i);
	}
	return res;
}

int main (int argc, char **argv){
	std::vector<double> xVal {-1, 0, 1, 2, 3, 4};
	std::vector<double> yVal {0.86603, 1, 0.86603, 0.5, 0, -0.5};
	std::vector<double> F1 = ssm(xVal, yVal, 1);
	std::vector<double> F2 = ssm(xVal, yVal, 2);
	std::vector<double> F3 = ssm(xVal, yVal, 3);
}
