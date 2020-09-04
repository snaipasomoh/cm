#include <iostream>
#include <iomanip>
/*
	y' = z1
	y'' = z1' = z2
	...
*/

double H1 = 0.1;
double H2 = 0.01;

double f (double x, double y, double z1){
	return - (2 * x * x * x * z1 + y) / x / x / x / x;
}

void solveEuler (double x0, double x1, double y0, double z0, double h = H1){
	double z2 = f(x0, y0, z0);
	double z1 = z0;
	double y = y0;
	double z2t;
	double z1t;
	double yt;
	std::cout << "H = " << h << std::endl;
	std::cout << "x\ty\ty'" << std::setprecision(4) << std::endl;
	for (double x = x0; x <= x1 + h / 2; x += h){
		std::cout << x << "\t" << y << "\t" << z1 << std::endl;
		auto z2f = [=](double x, double y, double z1){
			return z2 + h * f(x, y, z1);
		};
		auto z1f = [=](double x, double y, double z1){
			return z1 + h * z2f(x, y, z1);
		};
		auto yf = [=](double x, double y, double z1){
			return y + h * z1f(x, y, z1);
		};

		z2t = z2f(x, y, z1);
		z1t = z1f(x, y, z1);
		yt = yf(x, y, z1);

		// z2 = z2 + h / 2 * (f(x, y, z1) + f(x + h, yt, z1t));
		// z1 = z1t;
		// y = yt;

		z2 = z2 + h / 2 * (z2t + z2f(x + h, yt, z1t));
		z1 = z1 + h / 2 * (z1t + z1f(x + h, yt, z1t));
		y = y + h / 2 * (yt + yf(x + h, yt, z1t));
	}
}



int main (int argc, char **argv){
	solveEuler(1, 2, 1, 2, H2);
	// std::cout << std::endl;
	// solve(1, 2, 1, 2, H2);
}