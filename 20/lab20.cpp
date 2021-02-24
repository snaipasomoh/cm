// g++ lab20.cpp -o lab20 -lboost_iostreams -lboost_system -lboost_filesystem
#include <iostream>
#include <cmath>
#include <vector>
#include <gnuplot-iostream.h>

typedef std::vector<std::vector<double>> Matrix;

int main (int argc, char **argv){
	double lx = 0;
	double rx = M_PI;
	size_t N = 10;
	size_t K = 1000;
	double T = 20;
	double h = (rx - lx) / N;
	double tau = T / K;

	auto left = [](double t){
		return exp(-t);
	};
	auto right = [](double t){
		return -exp(-t);
	};
	Matrix res(1, std::vector<double>());

	for (size_t i = 0; i <= N; i++){
		res[0].push_back(cos(i * h));
	}

	{
		std::vector<double> temp;
		temp.push_back(left(tau));
		for (size_t i = 1; i < N; i++){
			temp.push_back(res[0][i] - tau * cos(i * h));
		}
		temp.push_back(right(tau));
		res.push_back(temp);
	}

	for (size_t i = 2; i <= K; i++){
		std::vector<double> newRow;
		newRow.push_back(left(i * tau));
		for (size_t j = 1; j < N; j++){
			double t1 = 4 * res[i - 1][j] + (3 * tau - 2) * res[i - 2][j];
			double t2 = (res[i - 1][j + 1] - 2 * res[i - 1][j] +
			             res[i - 1][j + 1]) / h / h +
			            (res[i - 1][j + 1] - res[i - 1][j - 1]) / 2 / h -
			             res[i - 1][j] + sin(lx + j * h) * exp(-tau * (i - 1));
			newRow.push_back((t1 + 2 * tau * tau * t2) / (2 + 3 * tau));
		}
		newRow.push_back(right(i * tau));

		res.push_back(newRow);
	}
	
	Gnuplot gp;
	gp << "set ticslevel 0" << std::endl;
	gp << "set xrange[0:3.14159265358979323846]" << std::endl;
	gp << "set yrange[0:20]" << std::endl;
	gp << "set xlabel \"X\"" << std::endl;
	gp << "set ylabel \"Temperature\"" << std::endl;
	gp << "set hidden3d" << std::endl;

	gp << "splot '-' with lines title \"U(x, t)\"" << std::endl;

	for (size_t i = 0; i <= K; i += (K / 200)){
		for (size_t j = 0; j <= N; j++){
			gp << (N - j) * h + lx << " " << i * tau << " " << res[i][N - j]
			<< std::endl;
		}
		gp << std::endl;
	}
	gp << "e" << std::endl;
	std::cin.get();
}