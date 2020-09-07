#include <iostream>
#include <cstdio>
#include <vector>
#include <iomanip>
/*
	y' = z1
	y'' = z1' = z2
	...
*/

typedef std::vector<std::pair<std::string, std::vector<double>>> plotData;

void plot (std::vector<double> const &x, plotData &data){
	FILE *pipe = popen("gnuplot -persist", "w");
	if (pipe){
		std::vector<std::string> textData (data.size(), "");
		for (size_t i = 0; i < x.size(); i++){
			for (size_t j = 0; j < data.size(); j++){
				textData[j] += std::to_string(x[i]) + " " +
				               std::to_string(data[j].second[i]) + "\n";
			}
		}
		fprintf(pipe, "set xrange [%lf:%lf]\n", x[0], x[x.size() - 1]);
		fprintf(pipe, "set autoscale y\n");
		std::string req = "plot";
		std::string foo = " '-' ";
		for (size_t i = 0; i < data.size(); i++){
			req += foo + "title \"" + data[i].first + "\" w l lw 3, ";
			foo = " '' ";
		}
		req.pop_back();
		req += "\n";
		fprintf(pipe, "%s", req.c_str());
		for (size_t i = 0; i < data.size(); i++){
			fprintf(pipe, "%se\n", textData[i].c_str());
		}
		fflush(pipe);
	
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();
	
		pclose(pipe);
	}
}

double H1 = 0.1;
double H2 = 0.01;

double f (double x, double y, double z1){
	return - (2 * x * x * x * z1 + y) / x / x / x / x;
}

void solve (double x0, double x1, double y0, double z0, double h = H1,
            bool plotting = false){
	double z1 = z0;
	double y = y0;
	std::cout << "H = " << h << std::endl;
	std::cout << "x\ty\ty'" << std::setprecision(4) << std::endl;
	std::vector<double> xv;
	std::vector<double> yv;

	for (double x = x0; x <= x1 + h / 2; x += h){
		std::cout << x << "\t" << y << "\t" << z1 << std::endl;
		if (plotting){
			xv.push_back(x);
			yv.push_back(y);
		}
		double z1t = z1 + h * f(x, y, z1);
		double yt = y + h * z1t;
		z1 += h / 2 * (f(x, y, z1) + f(x + h, y, z1t));
		y += h / 2 * (z1t + z1);
	}
	if (plotting){
		plotData foo = {std::make_pair(std::string("y(x)"), yv)};
		plot(xv, foo);
	}
}

int main (int argc, char **argv){
	solve(1, 2, 1, 2, H2, 1);
	std::cout << std::endl;

}