#include <iostream>
#include <cmath>

double func (double x){
	return std::log(x + 1) / std::log(M_E) - 2 * x * x + 1;
}

double supFunc (double x){
	return std::sqrt((std::log(x + 1) / std::log(M_E) + 1) / 2);
}

double binSearch (double a, double b, double y(double), double eps = 0.001){
	while (b - a > eps){
		double c = (a + b) / 2;
		if (y(a) * y (c) < 0){
			b = c;
		}
		else{
			a = c;
		}
	}
	return (a + b) / 2;
}

double simIter (double a, double b, double y(double), double sup(double),
	            double eps = 0.001){
	double x = (a + b) / 2;
	while (std::abs(y(x)) > eps){
		x = sup(x);
	}
	return x;
}

double newton (double a, double b, double y(double), double eps = 0.001){
	double x = (a + b) / 2;
	while (std::abs(y(x)) > eps){
		double dy = (y(x + eps) - y(x)) / eps;
		x = x - y(x) / dy;
	}
	return x;
}

double sec (double a, double b, double y(double), double eps = 0.001){
	double x = (a + b) / 2;
	double dy = (y(x + eps) - y(x)) / eps;
	double prevX = x = x - y(x) / dy;
	dy = (y(x + eps) - y(x)) / eps;
	x = x - y(x) / dy;
	while (std::abs(y(x)) > eps){
		double t = x;
		x = x - y(x) * (x - prevX) / (y(x) - y(prevX));
		prevX = t;
	}
	return x;
}

int main (int argc, char **argv){
	std::cout << binSearch(0, 1, func) << std::endl;
	std::cout << simIter(0, 1, func, supFunc) << std::endl;
	std::cout << newton(0, 1, func) << std::endl;
	std::cout << sec(0, 1, func) << std::endl;
}
