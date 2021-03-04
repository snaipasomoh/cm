// g++ lab21.cpp -o lab21 -lboost_iostreams -lboost_system -lboost_filesystem
#include <iostream>
#include <cmath>
#include <vector>
#include <gnuplot-iostream.h>

typedef std::vector<std::vector<double>> Matrix;

int main (int argc, char **argv){
	double x1 = 0;
	double x2 = M_PI_2;
	double y1 = 0;
	double y2 = M_PI_2;
	size_t N = 10;
	size_t K = 10;
	double eps = 0.001;

	double h1 = (x2 - x1) / N;
	double h2 = (y2 - y1) / K;

	auto left = [](double y){
		return exp(-y) * cos(y);
	};
	auto right = [](double y){
		return 0;
	};
	auto upper = [](double x){
		return 0;
	};
	auto lower = [](double x){
		return cos(x);
	};

	Matrix res(K + 1, std::vector<double>(N + 1, 1));
	for (size_t i = 0; i <= K; i++){
		res[i][0] = left(i * h2);
		res[i][N] = right(i * h2);
	}
	for (size_t i = 0; i <= N; i++){
		res[0][i] = lower(i * h1);
		res[K][i] = upper(i * h1);
	}

	double delta = eps * 2;
	while (delta >= eps){
		double maxD = 0;
		for (size_t i = 1; i < K; i++){
			for (size_t j = 1; j < N; j++){
				double temp = res[i][j];

				res[i][j] = (h1 * h1 * (res[i][j + 1] + res[i][j - 1]) + 
				h2 * h2 * (res[i + 1][j] + res[i - 1][j]) + 
				h1 * h1 * h2 * (res[i + 1][j] - res[i - 1][j])) / 
				(2 * h1 * h1 + 2 * h2 * h2 - 3 * h1 * h1 * h2 * h2);

				double td = fabs(res[i][j] - temp);
				std::cout << temp << " " << res[i][j] << " " << td << std::endl;
				maxD = (maxD < td) ? td : maxD;
			}
		}
		delta = maxD;
		std::cout << delta << std::endl;
	}

	Gnuplot gp;
	gp << "set ticslevel 0" << std::endl;
	gp << "set xrange[0:1.57079632679489661923]" << std::endl;
	gp << "set yrange[0:1.57079632679489661923]" << std::endl;
	gp << "set xlabel \"X\"" << std::endl;
	gp << "set ylabel \"Y\"" << std::endl;
	gp << "set hidden3d" << std::endl;

	gp << "splot '-' with lines title \"U(x, y)\"" << std::endl;

	for (size_t i = 0; i <= K; i++){
		for (size_t j = 0; j <= N; j++){
			gp << (N - j) * h1 + x1 << " " << i * h2 + y1 << " " << 
			res[i][N - j] << std::endl;
		}
		gp << std::endl;
	}
	gp << "e" << std::endl;
	std::cin.get();

}	