#include <iostream>
#include <vector>
#include <cmath>

double H1 = 0.1;
double H2 = 0.01;

double f (double x, double y){
	return (y * (2 * y * log(x) - 1) - 1) / x;
}

void adams (double x0, double x1, double y0, double h = H1){
	std::vector<double> y;
	std::vector<double> x;
	double xt = x0;
	double yt = yt;
	std::cout << "H = " << h << std::endl;
	std::cout << "x\ty" << std::endl;
	for (size_t i = 0; i < 4; i++){
		std::cout << xt << "\t" << yt << std::endl;
		y.push_back(yt);
		x.push_back(xt);
		double k1 = h * f(xt, yt);
		double k2 = h * f(xt + h / 2, yt + k1 / 2);
		double k3 = h * f(xt + h / 2, yt + k2 / 2);
		double k4 = h * f(xt + h, yt + k3);
		yt = yt + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
		xt = xt + h;
	}
	for (; xt <= x1 + h / 2; xt += h){
		yt = y[3] + h / 24 * (55 * f(x[3], y[3]) - 59 * f(x[2], y[2]) +
		                      37 * f(x[1], y[1]) - 9 * f(x[0], y[0]));
		std::cout << xt << "\t" << yt << std::endl;
		y.erase(y.begin());
		x.erase(x.begin());
		y.push_back(yt);
		x.push_back(xt);
	}
}

int main (int argc, char ** argv){
	adams(1, 2, 0.3333333);
	std::cout << std::endl;
	adams(1, 2, 0.3333333, H2);
}