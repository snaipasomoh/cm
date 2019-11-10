#include <iostream>
#include <cmath>

double uCos (double x, double eps = 0.0001){
	double num = 1;
	double denom = 1;
	double res = 0;
	double curr = 1;
	int n = 0;
	while (fabs(curr) > eps){
		res += curr;
		num *= -x * x;
		n++;
		denom *= 4 * n * n - 2 * n;
		curr = num / denom;
	}
	return res;
}

double uExp (double x, double eps = 0.0001){
	double num = 1;
	double denom = 1;
	double res = 0;
	double curr = 1;
	int n = 0;
	while (fabs(curr) > eps){
		res += curr;
		num *= x;
		n++;
		denom *= n;
		curr = num / denom;
	}
	return res;
}

double uLg (double x, double eps = 0.0001){
	if (x <= 0){
		std::cout << "Wrong arg for Lg" << std::endl;
		exit(0);
	}
	int mult = 0;
	while (fabs(x) > 1){
		x /= 10;
		mult++;
	}
	x -= 1;
	double num = x;
	double denom = 1;
	double res = 0;
	double curr = x;
	int n = 1;
	while (fabs(curr) > eps){
		res += curr;
		num *= -x;
		denom++;
		curr = num / denom;
	}
	return res / log(10) + mult;
}

void end (){
	std::cout << "Usage:" << std::endl;
	std::cout << "[function](e/c/l)" << std::endl;
	exit(0);
}

int main (int argc, char **argv){
	if (argc != 2){
		end();
	}
	double x;
	double res = 42;
	std::cin >> x;

	if (*argv[1] == 'e'){
		res = uExp(x);
	}
	else if (*argv[1] == 'c'){
		x = fmod(x, 2*M_PI);
		res = uCos(x);
	}
	else if (*argv[1] == 'l'){
		res = uLg(x);
	}

	std::cout << res << std::endl;
}
