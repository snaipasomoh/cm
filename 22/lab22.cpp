// g++ lab22.cpp -o lab22 -lboost_iostreams -lboost_system -lboost_filesystem
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <gnuplot-iostream.h>

double const X1 = 0;
double const X2 = M_PI_2;
double const Y1 = 0;
double const Y2 = M_PI;
double const t1 = 0;
double const t2 = 1;
double const a = 1;
double const b = 1;
double const m = 1;

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
	return 0;
}

double right (double y, double t){
	return sin(y) * sin(m * t);
}

double lower (double x, double t){
	return 0;
}

double upperDer (double x, double t){
	return -sin(x) * sin(m * t);
}

double func (double x, double y, double t){
	return sin(x) * sin(y) * (m * cos(m * t) + (a + b) * sin(m * t));
}

Matrix makeHalfLayer (Matrix const &currLayer, size_t currLayerLevel){
	Matrix res(K + 1, std::vector<double>(N + 1, 0));
	double t = currLayerLevel * tau + tau / 2;

	for (size_t i = 0; i <= K; i++){
		res[i][0] = left(i * h2 + Y1, t);
		res[i][N] = right(i * h2 + Y1, t);
	}

	for (size_t j = 1; j < N; j++){
		eqVectors vec;

		vec.a.push_back(0);
		vec.b.push_back(2 * b / h2 / h2 + 2 / tau);
		vec.c.push_back(-b / h2 / h2);
		vec.d.push_back((a / h1 / h1) * currLayer[1][j + 1] +
		(2 / tau - 2 * a / h1 / h1) * currLayer[1][j] +
		(a / h1 / h1) * currLayer[1][j - 1] +
		func(j * h1 + X1, 1 * h2 + Y1, t));

		for (size_t i = 2; i < K - 1; i++){
			vec.a.push_back(-b / h2 / h2);
			vec.b.push_back(2 * b / h2 / h2 + 2 / tau);
			vec.c.push_back(-b / h2 / h2);
			vec.d.push_back((a / h1 / h1) * currLayer[i][j + 1] +
			(2 / tau - 2 * a / h1 / h1) * currLayer[i][j] +
			(a / h1 / h1) * currLayer[i][j - 1] +
			func(j * h1 + X1, i * h2 + Y1, t));
		}

		vec.a.push_back(-b / h2 / h2);
		vec.b.push_back(b / h2 / h2 + 2 / tau);
		vec.c.push_back(0);
		vec.d.push_back((a / h1 / h1) * currLayer[K - 1][j + 1] +
		(2 / tau - 2 * a / h1 / h1) * currLayer[K - 1][j] +
		(a / h1 / h1) * currLayer[K - 1][j - 1] +
		func(j * h1 + X1, (K - 1) * h2 + Y1, t) +
		h2 * upperDer(j * h1 + X1, t));

		std::vector<double> col = thomasMethod(vec);
		res[0][j] = lower(j * h1 + X1, t);
		for (size_t i = 1; i < K; i++){
			res[i][j] = col[i - 1];
		}
		res[K][j] = col[K - 2] + h2 * upperDer(j * h1 + X1, t);
	}
	return res;
}

Matrix makeFullLayer (Matrix const &halfLayer, size_t currLayerLevel){
	Matrix res(K + 1, std::vector<double>(N + 1, 0));
	double t = (currLayerLevel + 1) * tau;

	for (size_t i = 1; i < K; i++){
		eqVectors vec;

		vec.a.push_back(0);
		vec.b.push_back(2 * a / h1 / h1 + 2 / tau);
		vec.c.push_back(-a / h1 / h1);
		vec.d.push_back((b / h2 / h2) * halfLayer[i + 1][1] +
		(2 / tau - 2 * b / h2 / h2) * halfLayer[i][1] +
		(b / h2 / h2) * halfLayer[i - 1][1] +
		func(1 * h1 + X1, i * h2 + Y1, currLayerLevel * tau + tau / 2));

		for (size_t j = 2; j < N - 1; j++){
			vec.a.push_back(-a / h1 / h1);
			vec.b.push_back(2 * a / h1 / h1 + 2 / tau);
			vec.c.push_back(-a / h1 / h1);
			vec.d.push_back((b / h2 / h2) * halfLayer[i + 1][j] +
			(2 / tau - 2 * b / h2 / h2) * halfLayer[i][j] +
			(b / h2 / h2) * halfLayer[i - 1][j] +
			func(j * h1 + X1, i * h2 + Y1, currLayerLevel * tau + tau / 2));
		}

		vec.a.push_back(-a / h1 / h1);
		vec.b.push_back(2 * a / h1 / h1 + 2 / tau);
		vec.c.push_back(0);
		vec.d.push_back((b / h2 / h2) * halfLayer[i + 1][1] +
		(2 / tau - 2 * b / h2 / h2) * halfLayer[i][1] +
		(b / h2 / h2) * halfLayer[i - 1][1] +
		func(1 * h1 + X1, i * h2 + Y1, currLayerLevel * tau + tau / 2) +
		(a / h1 / h1) * right(i * h2 + Y1, t));

		std::vector<double> row = thomasMethod(vec);
		res[i][0] = left(i * h2 + Y1, t);
		for (size_t j = 1; j < N; j++){
			res[i][j] = row[j - 1];
		}
		res[i][N] = right(i * h2 + Y1, t);
	}

	for (size_t j = 0; j <= N; j++){
		res[0][j] = lower(j * h1 + X1, t);
		res[K][j] = res[K - 1][j] + h2 * upperDer(j * h1 + X1, t);
	}
	return res;
}

int main (int argc, char **argv){
	Matrix currLayer(K + 1, std::vector<double>(N + 1, 0));
	saveLayer(currLayer, 0);
	
	for (size_t i = 0; i < T; i++){
		Matrix halfLayer = makeHalfLayer(currLayer, i);
		currLayer = makeFullLayer(halfLayer, i);
		saveLayer(currLayer, i + 1);
	}


	Gnuplot gp;
	gp << "set ticslevel 0" << std::endl;
	gp << "set xrange[0:1.5708]" << std::endl;
	gp << "set yrange[0:3.1416]" << std::endl;
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

	// gp << "splot 'layer_0.dat' w l title \"foo\", 'layer_1.dat' w l title \"bar\", 'layer_2.dat' w l title \"baz\"" << std::endl;
	
	std::cin.get();
}