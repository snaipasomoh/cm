#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>
#include "simsym.hpp"
#include <eigen3/Eigen/Dense>

Eigen::VectorXd solveLS (Eigen::MatrixXd const &A, Eigen::VectorXd const &b){
	return A.colPivHouseholderQr().solve(b);
}

std::vector<double> ssm (std::vector<double> const &x,
						 std::vector<double> const &y, size_t k){
	std::vector<double> xPows (2 * k + 1, 0);
	std::vector<double> yxPows (k + 1, 0);
	size_t len = x.size();

	for (size_t i = 0; i < len; i++){
		for (size_t j = 0; j < 2 * k + 1; j++){
			xPows[j] += std::pow(x[i], j);
		}
		for (size_t j = 0; j < k + 1; j++){
			yxPows[j] += y[i] * std::pow(x[i], j);
		}
	}
	// std::cout << "hello" << std::endl;
	Eigen::MatrixXd A(k + 1, k + 1);
	Eigen::VectorXd b(k + 1);
	for (size_t i = 0; i < k + 1; i++){
		for (size_t j = 0; j < k + 1; j++){
			A(i, j) = xPows[i + j];
		}
		b(i) = yxPows[i];
	}
	auto sol = solveLS(A, b);
	std::vector<double> res;
	for (size_t i = 0; i < k + 1; i++){
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



	FILE *pipe = popen("gnuplot -persist", "w");
	if (pipe){
		size_t nDots = 100;
		double step = (xVal[xVal.size() - 1] - xVal[0] + 2) / nDots;
		std::string f1, f2, f3;
		for(size_t i = 0; i <= nDots; i++){
			double x = xVal[0] - 1 + i * step;
			f1 += std::to_string(x) + " " +
									std::to_string(getValInX(F1, x)) + "\n";
			f2 += std::to_string(x) + " " +
									std::to_string(getValInX(F2, x)) + "\n";
			f3 += std::to_string(x) + " " +
									std::to_string(getValInX(F3, x)) + "\n";
		}


		fprintf(pipe, "set title \"Lab 5\"\n");
		fprintf(pipe, "set xrange [%f:%f]\n", xVal[0], xVal[xVal.size() - 1]);
		fprintf(pipe, "set autoscale y\n");
		fprintf(pipe, "plot '-' title \"y=F1(x)\" w l lc 1 lw 10,\
							'' title \"y=F2(x)\" w l lc 2 lw 10,\
							'' title \"y=F3(x)\" w l lc 4 lw 10\n");
		fprintf(pipe, "%se\n%se\n%se\n", f1.c_str(), f2.c_str(), f3.c_str());
		fflush(pipe);

		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();

		pclose(pipe);
	}
}
