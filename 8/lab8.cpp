#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

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

	std::cout << "Q: ";
	for (auto i : nd){
		std::cout << i << " ";
	}
	std::cout << std::endl << "P: ";
	for (auto i : nc){
		std::cout << -i << " ";
	}
	std::cout << 0 << std::endl;

	std::reverse(res.begin(), res.end());
	return res;
}

int main (int argc, char **argv){
	eqVectors foo;
	foo.a = {0, -7, -7, 4, -6};
	foo.b = {-12, -11, 21, -13, 14};
	foo.c = {-7, -3, -8, 5, 0};
	foo.d = {-102, -92, -65, 38, -12};

	std::vector<double> res = thomasMethod(foo);
	std::cout << "X: ";
	for (auto i : res){
		std::cout << i << " ";
	}
	std::cout << std::endl;
}
