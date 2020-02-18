#include <iostream>
#include <cmath>
#include <vector>

double mFunc (double x){
	return x / std::pow((3 * x + 4), 2);
}

double rectMethod (std::vector<double> x, double dx, double y (double)){
	double res = 0;
	for (double i : x){
		res += y(i) * dx;
	}
	return res;
}

double leftRect (double a, double b, double dx, double y (double)){
	std::vector<double> x;
	while (a < b){
		x.push_back(a);
		a += dx;
	}
	return rectMethod(x, dx, y);
}

double rightRect (double a, double b, double dx, double y (double)){
	std::vector<double> x;
	while (a < b){
		x.push_back(a + dx);
		a += dx;
	}
	return rectMethod(x, dx, y);
}

double middleRect (double a, double b, double dx, double y (double)){
	std::vector<double> x;
	while (a < b){
		x.push_back(a + dx / 2);
		a += dx;
	}
	return rectMethod(x, dx, y);
}

double trapezMethod (double a, double b, double dx, double y (double)){
	double res = 0;
	while (a < b){
		res += (y(a) + y(a + dx)) * dx / 2;
		a += dx;
	}
	return res;
}

double simpsonsMethod (double a, double b, double dx, double y (double)){
	double res = 0;
	while (a < b){
		res += (y(a) + 4 * y(a + dx / 2) + y(a + dx)) * dx / 6;
		a += dx;
	}
	return res;
}

int main (int argc, char **argv){
	std::cout << "Left rectangle: " << leftRect(0, 4, 1, mFunc) << std::endl;
	std::cout << "Right rectangle: " << rightRect(0, 4, 1, mFunc) << std::endl;
	std::cout << "Middle rectangle: " << middleRect(0, 4, 1, mFunc) << std::endl;
	std::cout << "Trapezii: " << trapezMethod(0, 4, 1, mFunc) << std::endl;
	std::cout << "Simpson: " << simpsonsMethod(0, 4, 1, mFunc) << std::endl;

	std::cout << "Middle rectangle h/2: " << middleRect(0, 4, 0.5, mFunc) +
							std::abs(middleRect(0, 4, 0.5, mFunc) -
							middleRect(0, 4, 1, mFunc)) / 3 << std::endl;
	std::cout << "Trapezii h/2: " << trapezMethod(0, 4, 0.5, mFunc) +
							std::abs(trapezMethod(0, 4, 0.5, mFunc) -
							trapezMethod(0, 4, 1, mFunc)) / 3 << std::endl;

	std::cout << "Simpson h/2: " << simpsonsMethod(0, 4, 0.5, mFunc) +
							std::abs(simpsonsMethod(0, 4, 0.5, mFunc) -
							simpsonsMethod(0, 4, 1, mFunc)) / 15 << std::endl;


							// std::cout << "Trapezii h/2: " << trapezMethod(0, 4, 0.5, mFunc) << " +- " <<
							// 						std::abs(trapezMethod(0, 4, 0.5, mFunc) -
							// 						trapezMethod(0, 4, 1, mFunc)) / 3 << std::endl;
							//
							// std::cout << "Simpson h/2: " << simpsonsMethod(0, 4, 0.5, mFunc) << " +- " <<
							// 						std::abs(simpsonsMethod(0, 4, 0.5, mFunc) -
							// 						simpsonsMethod(0, 4, 1, mFunc)) / 15 << std::endl;
}
