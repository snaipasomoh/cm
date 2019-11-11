#ifndef SIMSYM
#define SIMSYM
#include <vector>

class simsym{
	std::vector<double> data;
public:
	simsym (double n) : data(std::vector<double> {n}) {}

	simsym (std::vector<double> const &n) : data(n) { this->cutZeros(); }

	size_t size() const {
		return data.size();
	}

	double operator[] (size_t index) const {
		return data[index];
	}

	double &operator[] (size_t index) {
		return data[index];
	}

	std::vector<double> getCoefVec () const {
		return data;
	}

	simsym &cutZeros () {
		std::vector<double>::iterator firstZero = data.end();
		for (auto iter = data.end() - 1; *iter == 0; firstZero = iter--){}
		data.erase(firstZero, data.end());
		return *this;
	}
};

simsym operator+ (simsym const &a, simsym const &b) {
	if (a.size() > b.size()){
		std::vector<double> newData (a.getCoefVec());
		for (size_t i = 0; i < b.size(); i++){
			newData[i] += b[i];
		}
		return simsym(newData).cutZeros();
	}
	std::vector<double> newData (b.getCoefVec());
	for (size_t i = 0; i < a.size(); i++){
		newData[i] += a[i];
	}
	return simsym(newData).cutZeros();
}

simsym operator- (simsym const &a, simsym const &b) {
	auto coefs = b.getCoefVec();
	for (double &i : coefs){
		i *= -1;
	}
	return a + coefs;
}

simsym operator* (simsym const &a, simsym const &b) {
	size_t len = a.size() + b.size();
	std::vector<double> newData (len, 0);
	for (size_t i = 0; i < a.size(); i++){
		for (size_t j = 0; j < b.size(); j++){
			newData[i + j] += a[i] * b[j];
		}
	}
	return simsym(newData).cutZeros();
}

#endif
