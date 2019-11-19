#include <iostream>
#include <cmath>
#include "simsym.hpp"

simsym getLagr (std::vector<double> const &x, std::vector<double> const &y){
	simsym res(0);
	simsym xVar({0, 1});
	size_t len = x.size();
	for (size_t i = 0; i < len; i++){
		simsym temp(y[i]);
		for (size_t j = 0; j < len; j++){
			if (i == j) { continue; }
			temp = temp * (xVar - x[j]) / (x[i] - x[j]);
		}
		res = res + temp;
	}
	return res;
}

simsym getNewt (std::vector<double> const &x, std::vector<double> const &y){
	std::vector<double> currLevelDivDiff = y;
	simsym res(y[0]);
	simsym xVar({0, 1});
	simsym mult(1);
	for (size_t i = 1; i < x.size(); i++){
		mult = mult * (xVar - x[i - 1]);
		std::vector<double> temp;
		{
			for (size_t j = 0; j < currLevelDivDiff.size() - 1; j++){
				temp.push_back((currLevelDivDiff[j] - currLevelDivDiff[j + 1])
								/ (x[j] - x[j + i]));
			}
		}
		currLevelDivDiff = temp;
		res = res + mult * currLevelDivDiff[0];
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
	std::vector<double> xVal {0, M_PI/6, 2*M_PI/6, 3*M_PI/6};
	std::vector<double> yVal;
	double X = M_PI/4;
	for (size_t i = 0; i < xVal.size(); i++){
		yVal.push_back(cos(xVal[i]));
	}
	simsym lagr = getLagr(xVal, yVal);
	std::cout << "Value of Lagrange polynomial in X = " << X << " : ";
	std::cout << getValInX(lagr, X) << std::endl;
	std::cout << std::endl;
	std::cout << "Value of Newton polynomial in X = " << X << " : ";
	simsym newt = getNewt(xVal, yVal);
	std::cout << getValInX(newt, X) << std::endl;
}
