#ifndef _CONF_HPP
#define _CONF_HPP

#include "node.h"

static const string CONFIG = "g04.conf";

static int ERROR = 0;

typedef bool(Node::*mf)(void);
typedef bool(Node::*int_setter)(int);
typedef bool(Node::*str_setter)(string);
typedef bool(Node::*boo_setter)(bool);

typedef map<string, mf> mf_map;

enum {
	INT_T,
	STR_T,
	BLN_T
};

static const tuple<int, string, mf> conf_map[] {
	make_tuple(INT_T, "neighborPort", 
		(mf)(&Node::set_neighborPort)),
	make_tuple(INT_T, "filePort", 
		(mf)(&Node::set_filePort)),
	make_tuple(INT_T, "NumberOfPeers", 
		(mf)(&Node::set_numberOfPeers)),
	make_tuple(INT_T, "TTL", 
		(mf)(&Node::set_TTL)),
	make_tuple(STR_T, "seedNodes", 
		(mf)(&Node::set_seedNodes)),
	make_tuple(BLN_T, "isSeedNode", 
		(mf)(&Node::set_isSeedNode)),
	make_tuple(STR_T, "localFilesDirectory", 
		(mf)(&Node::set_localFilesDirectory)),
	make_tuple(STR_T, "localFiles", 
		(mf)(&Node::set_localFiles))
};


int configure(Node &N);

vector<pair<string, string>> read_conf(const string &fname);

void configure(Node &N, const vector<pair<string, string>> &conf);

template <typename T>
void configureNodeValue(bool (Node::*setfunc)(T),
	Node &N, const string &key, const T &val);

template<typename T>
T split(const string &line, const char delim);

template <typename T> 
T read_value(const string &key, const string &val);

void value_err(const string &key, const string &val);

bool isTrue(string s);


int configure(Node &N) {
	vector<pair<string, string>> conf = read_conf(CONFIG);
	configure(N, conf);
	if (ERROR) return -1;
	
	return 0;
}

vector<pair<string, string>> read_conf(const string &fname) {
	vector<pair<string, string>> conf;
	ifstream ifs;
	string line;
	list<string> conf_pair;
	ifs.open(fname, ios::in);
	if (!ifs) {
		cout << "Failed to open " << fname << endl;
		exit(-1);
	}
	while (getline(ifs, line)) {
		conf_pair = split<list<string>>(line, '=');
		if (conf_pair.size() != 2) {
			cerr << fname << ": " << line << endl 
				<< "Invalid configuration\n";
			exit(-1);
		}
		conf.push_back(make_pair(conf_pair.front(), conf_pair.back()));
	}
	ifs.close();
	return conf;
}

void configure(Node &N, const vector<pair<string, string>> &conf) {
	for (auto & c : conf) {
		for (auto & m : conf_map) {
			if (!c.first.compare(get<1>(m))) {
				switch (get<0>(m)) {
					case INT_T:
						// ints
						configureNodeValue<int>((int_setter)get<2>(m), N, 
							c.first, read_value<int>(c.first, c.second));
						break;
					case BLN_T:
						// bools
						configureNodeValue<bool>((boo_setter)get<2>(m), N,
							c.first, isTrue(c.second));
						break;
					case STR_T:
						// strings
						configureNodeValue<string>((str_setter)get<2>(m), N,
							c.first, c.second);
						break;
					default:
						// no others to support
						cerr << "No type match for input configuation: " 
							<< c .first << endl;
						break;
				}
			}
		}
	}
}

template <typename T>
void configureNodeValue(bool (Node::*setfunc)(T),
	Node &N, const string &key, const T &val) {
	// T setval = read_value<T>(key, val);
	if (!(N.*setfunc)(val)) {
		cerr << "Bad value for " << key << ": "
			<< val << endl;
		ERROR = 1;
	}
}

template<typename T>
T split(const string &line, const char delim) {
	sstream ss(line);
    string item;
	T container;
    while (getline(ss, item, delim))
        container.push_back(item);
    return container;
}

template <typename T>
T read_value(const string &key, const string &val) {
	T n = 0;
	sstream ss(val);
	ss >> n;
	if (ss.fail()) value_err(key, val);
	return n;
}

void value_err(const string &key, const string &val) {
	cerr << "Value Error: " << 
		key << ": " << val << endl;
	ERROR = 1;
}

bool isTrue(string s) {
	string value[] = {
		"1", "True", "true", "T", "t",
		"Yes", "yes", "Y", "y"
	};
	for (auto & val: value)
		if (!s.compare(val))
			return true;
	return false;
}


#endif