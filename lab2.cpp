#include <iostream>
#include <cmath>
#include <vector>

double diff (double y1, double y0, double h){
	if (h == 0){
		std::cout << "Zero division" << std::endl;
		exit(0);
	}
	return (y1 - y0) / h;
}



int main (int argc, char **argv){
	std::vector<double> x{1.0, 1.5, 2.0, 2.5, 3.0};
	std::vector<double> y{0, 0.40547, 0.69315, 0.91629, 1.0986};
	std::cout << "F'(2):" << std::endl;
	std::cout << "\tleft: " << diff(y[2], y[1], x[2] - x[1]) << std::endl;
	std::cout << "\tright: " << diff(y[3], y[2], x[3] - x[2]) << std::endl;
	std::cout << "\tcenter: " << diff(y[3], y[1], x[3] - x[1]) << std::endl;
	std::cout << "F\"(2):" << std::endl;
	std::cout << "\tright: " << (y[3] - 2*y[2] + y[1]) /
	 											(x[3] - x[2]) / (x[2] - x[1]);
	std::cout << std::endl;
}
