#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "simsym.hpp"

struct eqVectors{
	std::vector<double> a;
	std::vector<double> b;
	std::vector<double> c;
	std::vector<double> d;
};

std::vector<double> thomasMethod (eqVectors const &vec){
	std::vector<double> res;
	std::vector<double> nd {vec.d[0] / vec.b[0]};
	std::vector<double> nc {vec.c[0] / vec.b[0]};
	size_t len = vec.d.size();

	for (size_t i = 1; i < len; i++){
		nd.push_back((vec.d[i] - vec.a[i] * nd[i - 1]) /
											(vec.b[i] - vec.a[i] * nc[i - 1]));
		if (i == len - 1){
			continue;
		}
		nc.push_back(vec.c[i] / (vec.b[i] - vec.a[i] * nc[i - 1]));
	}

	res.push_back(nd[len - 1]);
	for (size_t i = 1; i < len; i++){
		res.push_back(nd[len - i - 1] - nc[len - i - 1] * res[i - 1]);
	}
	std::reverse(res.begin(), res.end());
	return res;
}

eqVectors vecForThomas (std::vector<double> const &x,
						std::vector<double> const &y){
	eqVectors vec;
	size_t len = x.size();
	std::vector<double> h;
	for (size_t i = 1; i < len; i++){
		h.push_back(x[i] - x[i - 1]);
	}

	for (size_t i = 1; i < len - 1; i++){
		double a, b, c, d;

		if (i == 1){
			a = 0;
		}
		else{
			a = h[i - 1];
		}

		if (i == len - 1){
			c = 0;
		}
		else{
			c = h[i];
		}

		b = 2 * (h[i - 1] + h[i]);

		d = 3 * (((y[i + 1] - y[i]) / h[i]) - (y[i] - y[i - 1]) / h[i - 1]);

		vec.a.push_back(a);
		vec.b.push_back(b);
		vec.c.push_back(c);
		vec.d.push_back(d);
	}
	return vec;
}

eqVectors vecForSpline (std::vector<double> const &x,
						std::vector<double> const &y,
						std::vector<double> const &c){
	eqVectors res;
	size_t len = x.size();

	for (size_t i = 0; i < len; i++){
		if (i == 0 || i == len - 1){
			res.c.push_back(0);
		}
		else{
			res.c.push_back(c[i - 1]);
		}
		if (i == 0){
			res.b.push_back(0);
			res.d.push_back(0);
		}
		else{
			res.b.push_back((y[i] - y[i - 1]) / (x[i] - x[i - 1]) +
						(2 * res.c[i] + res.c[i - 1]) / 3 * (x[i] - x[i - 1]));
			res.d.push_back((res.c[i] - res.c[i - 1]) / 3 / (x[i] - x[i - 1]));
		}
		res.a.push_back(y[i]);
	}

	return res;
}

std::vector<simsym> makeSplines (eqVectors const &vec,
								std::vector<double> const &x){
	std::vector<simsym> res;
	simsym xVar({0, 1});
	for (size_t i = 1; i < x.size(); i++){
		res.push_back(vec.a[i] +
					  vec.b[i] * (xVar - x[i]) +
					  vec.c[i] * (xVar - x[i]) * (xVar - x[i]) +
					  vec.d[i] * (xVar - x[i]) * (xVar - x[i]) * (xVar - x[i]));
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
	std::vector<double> xVal {0, 1, 2, 3, 4};
	std::vector<double> yVal {1, 0.86603, 0.5, 0, -0.5};

	eqVectors vec = vecForSpline(xVal, yVal, thomasMethod(vecForThomas(xVal, yVal)));
	std::vector<simsym> splines = makeSplines(vec, xVal);
	std::cout << getValInX(splines[1], 1.5) << std::endl;
	// for (auto i : thomasMethod(vec)){
	// 	std::cout << i << std::endl;
	// }
	// for (auto i : vec.a){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;
	// for (auto i : vec.b){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;
	// for (auto i : vec.c){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;
	// for (auto i : vec.d){
	// 	std::cout << i << " ";
	// }
	// std::cout << std::endl;
}
