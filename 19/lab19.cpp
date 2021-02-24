// g++ lab19.cpp -o lab19 -lboost_iostreams -lboost_system -lboost_filesystem

#include <iostream>
#include <vector>
#include <cmath>
#include <gnuplot-iostream.h>

typedef std::vector<std::vector<double>> Matrix;

int main (int argc, char **argv){
	double a = 1;
	double b = 1;
	double lx = 0;
	double rx = M_PI;
	size_t N = 10;
	size_t K = 1000;
	double T = 10;
	double h = (rx - lx) / N;
	double tau = T / K;

	auto left = [](double t, double a, double b){
		return exp(-a * t) * (-cos(b * t) + sin(b * t));
		// return cos(b * t);
	};
	auto right = [](double t, double a, double b){
		return -exp(-a * t) * (cos(b * t) + sin(b * t));
		// return cos(b * t - M_PI);
	};
	Matrix res(1, std::vector<double>());

	for (size_t i = 0; i <= N; i++){
		res[0].push_back(cos(i * h));
	}

	for (size_t i = 1; i <= K; i++){
		std::vector<double> newRow;
		newRow.push_back(0);
		for (size_t j = 1; j < N; j++){
			newRow.push_back(
			tau * 
			(a * (res[i - 1][j + 1] - 2 * res[i - 1][j] + res[i - 1][j - 1])
			/ h / h - b * (res[i - 1][j + 1] - res[i - 1][j - 1]) / 2 / h) + 
			res[i - 1][j]);
		}
		newRow[0] = (newRow[1] - h * left(i * tau, a, b)) / (h + 1);
		newRow.push_back((newRow[N - 1] + h * right(i * tau, a, b)) / (1 - h));

		res.push_back(newRow);
	}

	Gnuplot gp;
	gp << "set ticslevel 0" << std::endl;
	gp << "set xrange[0:3.14159265358979323846]" << std::endl;
	gp << "set yrange[0:10]" << std::endl;
	gp << "set xlabel \"X\"" << std::endl;
	gp << "set ylabel \"Temperature\"" << std::endl;
	gp << "set dgrid3d 21,11" << std::endl;
	gp << "set hidden3d" << std::endl;

	gp << "splot '-' with lines title \"U(x, t)\"" << std::endl;

	for (size_t i = 0; i <= K; i += (K / 20)){
		for (size_t j = 0; j <= N; j++){
			gp << j * h + lx << " " << i * tau << " " << res[i][j] << std::endl;
		}
	}
	gp << "e" << std::endl;
	std::cin.get();
}