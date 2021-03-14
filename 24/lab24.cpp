// g++ lab24.cpp -o lab24 -lboost_iostreams -lboost_system -lboost_filesystem -I /usr/include/eigen3
#include <iostream>
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include <gnuplot-iostream.h>

double f (double x){
	return 1 - x * (exp(x) - exp(-x));
}

double k (double x, double t){
	return x * x * exp(x * t);
}

int main (int argc, char **argv){
	double a = -1;
	double b = 1;
	size_t N = 20;
	double h = (b - a) / N;

	Eigen::MatrixXd A(N + 1, N + 1);
	Eigen::VectorXd B(N + 1);
	
	for (size_t i = 0; i <= N; i++){
		double x = i * h + a;
		A(i, 0) = -h / 2 * k(x, a);
		A(i, N) = -h / 2 * k(x, b);
		for (size_t j = 1; j < N; j++){
			double t = j * h + a;
			A(i, j) = -h * k(x, t);
		}
		A(i, i) += 1;
		B(i) = f(x);
	}

	Eigen::VectorXd res = A.colPivHouseholderQr().solve(B);

	Gnuplot gp;
	gp << "set ticslevel 0" << std::endl;
	gp << "set xrange[-1:1]" << std::endl;
	gp << "set xlabel \"X\"" << std::endl;
	gp << "set ylabel \"Y\"" << std::endl;
	gp << "plot '-' w l title \"U(x)\"" << std::endl;
	for (size_t i = 0; i <= N; i++){
		gp << i * h + a << " " << res(i) << std::endl;
		std::cout << i * h + a << " " << res(i) << std::endl;
	}
	gp << "e" << std::endl;
	std::cin.get();
}