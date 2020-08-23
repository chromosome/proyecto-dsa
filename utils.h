#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>

using namespace std;

namespace dsa {

using point_t = tuple<double, double>;
using data_t = pair<point_t, unsigned>;
using dist_t = pair<double, double>;
using quad_t = pair<point_t, dist_t>;

data_t make_entry(vector<string> record) {
	stringstream geopoint(record[7]);
	string latitude , longitude;
	getline(geopoint, latitude , ',');
	getline(geopoint, longitude, ',');

	return	make_pair<point_t, int>
	(
		{
			stold(latitude ),
			stold(longitude)
		},
			stoul(record[4])
	);
}

vector<data_t> read_data(string filename) {
	vector<data_t> data;

	ifstream input(filename);
	string line;

	getline(input, line);
	while (getline(input, line)) {
		stringstream line_stream(line);
		string field;

		vector<string> record;
		while (getline(line_stream, field, ';'))
			record.push_back(field);

		data.push_back(make_entry(record));
	}

	return data;
}

} // namespace dsa

double profile (function<void(void)> f) {
	auto start = chrono::steady_clock::now();
	f();
	auto end = chrono::steady_clock::now();

	return chrono::duration_cast<chrono::nanoseconds>(end - start).count()/1e6;
		 // "ms"
}

template<typename T, unsigned N, unsigned Last>
struct tuple_printer {
	static void print (ostream& out, const T& value) {
		out << get<N>(value) << ", ";
		tuple_printer<T, N+1, Last>::print(out, value);
	}
};

template<typename T, unsigned N>
struct tuple_printer <T, N, N> {
	static void print (ostream& out, const T& value) {
		out << get<N>(value);
	}
};

template<typename... Args>
ostream& operator << (ostream& out, const tuple<Args...>& value) {
	out << "(";
	tuple_printer<tuple<Args...>, 0, sizeof...(Args)-1>::print(out, value);
	out << ")";
	return out;
}

template<typename F, typename S>
ostream& operator << (ostream& out, const pair<F,S>& value) {
	out << "(" << value.first << ", " << value.second << ")";
	return out;
}

#endif
