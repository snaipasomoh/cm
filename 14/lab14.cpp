#include <iostream>
#include <cmath>

double H1 = -0.1;
double H2 = -0.01;

double f(double x, double y){
	return (-y * x + x * (x * x + 1)) / (x * x + 1);
}

void foo(double x0, double x1, double y0, double h = H1){
	double y = y0;
	std::cout << "H = " << h << std::endl;
	std::cout << "x\ty" << std::endl;
	for (double x = x0; x >= x1 + h / 2; x += h){
		std::cout << x << "\t" << y << std::endl;
		double k1 = h * f(x, y);
		double k2 = h * f(x + h / 2, y + k1 / 2);
		double k3 = h * f(x + h / 2, y + k2 / 2);
		double k4 = h * f(x + h, y + k3);
		y = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
	}
}

int main(int argc, char **argv){
	foo(0, -1, 1);
	std::cout << std::endl;
	foo(0, -1, 1, H2);
}