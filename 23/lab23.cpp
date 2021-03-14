// g++ lab23.cpp -o lab23 -lboost_iostreams -lboost_system -lboost_filesystem
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <gnuplot-iostream.h>

double const X1 = 0;
double const X2 = M_PI_4;
double const Y1 = 0;
double const Y2 = log(2);
double const t1 = 0;
double const t2 = 1;
double const a = 1;

size_t const N = 20;
size_t const K = 20;
size_t const T = 10;

double const h1 = (X2 - X1) / N;
double const h2 = (Y2 - Y1) / K;
double const tau = (t2 - t1) / T;

typedef std::vector<std::vector<double>> Matrix;

void saveLayer(Matrix const &data, size_t layerNum){
	std::string name = "layer_" + std::to_string(layerNum) + ".dat";
	std::ofstream file(name);
	for (size_t i = 0; i <= K; i++){
		for (size_t j = 0; j <= N; j++){
			file << (N - j) * h1 + X1 << " " << i * h2 + Y1 << " " << 
			data[i][N - j] << std::endl;
		}
		file << std::endl;
	}
	file.close();
}

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

double left (double y, double t){
	return cosh(y) * exp(-3 * a * t);
}

double right (double y, double t){
	return 0;
}

double lower (double x, double t){
	return cos(2 * x) * exp(-3 * a * t);
}

double upper (double x, double t){
	return 5 / 4 * cos(2 * x) * exp(-3 * a * t);
}

Matrix makeHalfLayer (Matrix const &currLayer, size_t currLayerLevel){
	Matrix res(K + 1, std::vector<double>(N + 1, 0));
	double t = currLayerLevel * tau;

	for (size_t i = 0; i <= K; i++){
		res[i][0] = left(i * h2 + Y1, t);
		res[i][N] = right(i * h2 + Y1, t);
	}

	for (size_t j = 1; j < N; j++){
		eqVectors vec;

		vec.a.push_back(0);
		vec.b.push_back(-2 * a / h2 / h2 - 1 / tau);
		vec.c.push_back(a / h2 / h2);
		vec.d.push_back(-1 / tau * currLayer[1][j] -
		a / h2 / h2 * lower(j * h1 + X1, t));

		for (size_t i = 2; i < K - 1; i++){
			vec.a.push_back(a / h2 / h2);
			vec.b.push_back(-2 * a / h2 / h2 - 1 / tau);
			vec.c.push_back(a / h2 / h2);
			vec.d.push_back(-1 / tau * currLayer[i][j]);
		}

		vec.a.push_back(a / h2 / h2);
		vec.b.push_back(-2 * a / h2 / h2 - 1 / tau);
		vec.c.push_back(0);
		vec.d.push_back(-1 / tau * currLayer[K][j] -
		a / h2 / h2 * upper(j * h1 + X1, t));

		std::vector<double> col = thomasMethod(vec);
		res[0][j] = lower(j * h1 + X1, t);
		for (size_t i = 1; i < K; i++){
			res[i][j] = col[i - 1];
		}
		res[K][j] = upper(j * h1 + X1, t);
	}
	return res;
}

Matrix makeFullLayer (Matrix const &halfLayer, size_t currLayerLevel){
	Matrix res(K + 1, std::vector<double>(N + 1, 0));
	double t = (currLayerLevel + 1) * tau;

	for (size_t j = 0; j <= N; j++){
		res[0][j] = lower(j * h1 + X1, t);
		res[K][j] = upper(j * h1 + X1, t);
	}

	for (size_t i = 1; i < K; i++){
		eqVectors vec;

		vec.a.push_back(0);
		vec.b.push_back(-2 * a / h2 / h2 - 1 / tau);
		vec.c.push_back(a / h2 / h2);
		vec.d.push_back(-1 / tau * halfLayer[i][1] -
		a / h2 / h2 * left(i * h2 + Y1, t));

		for (size_t j = 2; j < N - 1; j++){
			vec.a.push_back(a / h2 / h2);
			vec.b.push_back(-2 * a / h2 / h2 - 1 / tau);
			vec.c.push_back(a / h2 / h2);
			vec.d.push_back(-1 / tau * halfLayer[i][j]);
		}

		vec.a.push_back(a / h2 / h2);
		vec.b.push_back(-2 * a / h2 / h2 - 1 / tau);
		vec.c.push_back(0);
		vec.d.push_back(-1 / tau * halfLayer[i][N] -
		a / h2 / h2 * right(i * h2 + Y1, t));

		std::vector<double> row = thomasMethod(vec);
		res[i][0] = left(i * h2 + Y1, t);
		for (size_t j = 1; j < N; j++){
			res[i][j] = row[j - 1];
		}
		res[i][N] = right(i * h2 + Y1, t);
	}
	return res;
}

int main (int argc, char **argv){
	Matrix currLayer(K + 1, std::vector<double>(N + 1, 0));
	for (size_t i = 0; i <= K; i++){
		for (size_t j = 0; j <= N; j++){
			double x = j * h1 + X1;
			double y = i * h2 + Y1;
			currLayer[i][j] = cos(2 * x) * cosh(y);
		}
	}
	saveLayer(currLayer, 0);
	
	for (size_t i = 0; i < T; i++){
		Matrix halfLayer = makeHalfLayer(currLayer, i);
		currLayer = makeFullLayer(halfLayer, i);
		saveLayer(currLayer, i + 1);
	}


	Gnuplot gp;
	gp << "set ticslevel 0" << std::endl;
	gp << "set xrange[0:0.7854]" << std::endl;
	gp << "set yrange[0:0.6932]" << std::endl;
	gp << "set xlabel \"X\"" << std::endl;
	gp << "set ylabel \"Y\"" << std::endl;
	// gp << "set hidden3d" << std::endl;

	gp << "splot";
	
	for (size_t i = 0; i <= T; i++){
		std::string name = " 'layer_" + std::to_string(i) + ".dat'";
		gp << name << " w l lw 1 title \"" << i << "\"";
		if (i != T){
			gp << ",";
		}
	}
	gp << std::endl;
	
	std::cin.get();
}