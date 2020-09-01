#include <iostream>
#include <cmath>
#include <fstream>

double H1 = 0.1;
double H2 = 0.01;

double f(double x, double y){
	return -y / 2 / x + x * x;
}

double first(double x0, double y0, double h = H1){
	return y0 + h * f(x0, y0);
}

double second(double x0, double y0, double h = H1){
	double x1 = x0 + h;
	return y0 + h / 2 * (f(x0, y0) + f(x1, first(x0, y0)));
}

int main(int argc, char **argv){
	std::cout << "h = " << H1 << std::endl;
	std::cout << "x\ty0\ty1" << std::endl;
	double y0 = 1;
	double y1 = 1;
	std::ofstream f1 ("gr1.txt");
	std::ofstream f2 ("gr2.txt");
	for (double i = 1; i <= 2 + H1; i += H1){
		std::cout << i << "\t" << y0 << "\t" << y1 << std::endl;
		f1 << i << " " << y0 << std::endl;
		f2 << i << " " << y1 << std::endl;
		y0 = first(i, y0);
		y1 = second(i, y1);
	}
	f1.close();
	f2.close();
	std::cout << std::endl;
	std::cout << "h = " << H2 << std::endl;
	std::cout << "x\ty0\ty1" << std::endl;
	y0 = 1;
	y1 = 1;
	f1.open("gr3.txt");
	f2.open("gr4.txt");
	for (double i = 1; i <= 2 + H2; i += H2){
		std::cout << i << "\t" << y0 << "\t" << y1 << std::endl;
		f1 << i << " " << y0 << std::endl;
		f2 << i << " " << y1 << std::endl;
		y0 = first(i, y0, H2);
		y1 = second(i, y1, H2);
	}
	f1.close();
	f2.close();
}