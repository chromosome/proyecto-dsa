#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>

using namespace std;

using point_t = tuple<double, double>;
using data_t = pair<point_t, int>;
using dist_t = tuple<double, double>;
using quad_t = pair<point_t, dist_t>;

// namespace dsa {

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
			stoi (record[4])
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

// } // namespace dsa

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

#endif
