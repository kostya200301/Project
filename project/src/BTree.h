#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <lmdb.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>

using namespace std;


class Btree {
private:
	MDB_env *env;
	MDB_dbi dbi;
	MDB_val key, data;
	MDB_txn *txn;
	MDB_cursor *cursor;
	const char* path;
	int kBuf;
	int bufS;
public:
	Btree();
	Btree(const char* path, int bufferSize);

	bool AddData(const string& key, const string& value, bool com=false);

	bool Close();

	void SetKBuf(int bufs);

	map<string, string> GetData(const string& start, const string& end);

	void PrintAll();
};


#endif
