#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct edges {
	double y0Left;
	double y1Left;
	double sumLeft;
	double y0Right;
	double y1Right;
	double sumRight;
	double xLeft;
	double xRight;
	double h;
};

struct eqVectors{
	std::vector<double> a;
	std::vector<double> b;
	std::vector<double> c;
	std::vector<double> d;
};

double const H1 = 0.1;
double const H2 = 0.01;


// original: p(x)y'' + q(x)y' + r(x)y = f(x)
// func = p(x)y'' + q(x)y' + r(x)y - f(x)
// y -> y0
// y' -> y1
// y'' -> y2
double func (double x, double y0, double y1, double y2){
	return x * (x * x + 6) * y2 - 4 * (x * x + 3) * y1 + 6 * x * y0;
}

eqVectors makeVec (edges const &edg, double f(double, double, double, double)){
	eqVectors res;
	res.a.push_back(0);
	res.b.push_back(edg.y0Left * edg.h - edg.y1Left);
	res.c.push_back(edg.y1Left);
	res.d.push_back(edg.sumLeft * edg.h);
	for (size_t i = 1; i < (edg.xRight - edg.xLeft) / edg.h; i++){
		double x = edg.xLeft + edg.h * i;
		double p = f(x, 0, 0, 1) - f(x, 0, 0, 0);
		double q = f(x, 0, 1, 0) - f(x, 0, 0, 0);
		double r = f(x, 1, 0, 0) - f(x, 0, 0, 0);
		res.a.push_back(2 * p - q * edg.h);
		res.b.push_back(2 * edg.h * edg.h * r - 4 * p);
		res.c.push_back(2 * p + q * edg.h);
		res.d.push_back(-2 * f(x, 0, 0, 0) * edg.h * edg.h);
	}
	res.a.push_back(-edg.y1Right);
	res.b.push_back(edg.y0Right * edg.h + edg.y1Right);
	res.c.push_back(0);
	res.d.push_back(edg.sumRight * edg.h);
	return res;
}

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

int main (int argc, char **argv){
	edges foo = {0, 1, 0, 1, -1, 26, 0, 4, H1};
	auto bar = thomasMethod(makeVec(foo, func));
	for (auto i : bar){
		std::cout << i << std::endl;
	}
}