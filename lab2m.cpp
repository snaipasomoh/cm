#include <iostream>
#include <cmath>
#include <vector>

std::vector<double> oneDirDiff (std::vector<double> const &x,
								std::vector<double> const &y){
	std::vector<double> res;
	for (size_t i = 1; i < x.size(); i++){
		res.push_back((y[i] - y[i - 1]) / (x[i] - x[i - 1]));
	}
	return res;
}

std::vector<double> twoDirDiff (std::vector<double> const &x,
								std::vector<double> const &y){
	std::vector<double> res;
	for (size_t i = 1; i < x.size() - 1; i++){
		res.push_back((y[i + 1] - y[i - 1]) / (x[i + 1] - x[i - 1]));
	}
	return res;
}

int main (int argc, char **argv){
	std::vector<double> x{1.0, 1.5, 2.0, 2.5, 3.0};
	std::vector<double> y{0, 0.40547, 0.69315, 0.91629, 1.0986};
	std::cout << oneDirDiff(x, y)[1] << std::endl;
	std::cout << oneDirDiff(x, y)[2] << std::endl;
	std::cout << twoDirDiff(x, y)[1] << std::endl;
	std::cout << twoDirDiff(std::vector<double> ({x[1], x[2], x[3]}), twoDirDiff(x, y))[0] << std::endl;
}
