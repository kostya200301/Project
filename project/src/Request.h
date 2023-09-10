#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <vector>

using namespace std;

class Request {
public:
	string start_time;
	string end_time;
	vector<string> keyWords;
	Request(string start_time_, string end_time_, vector<string> keyWords_) {
		this->start_time = start_time_;
		this->end_time = end_time_;
		this->keyWords = keyWords_;
	}
	Request(){}

};

#endif
