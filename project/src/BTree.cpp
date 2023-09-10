


#include "BTree.h"

Btree::Btree() {
        this->path = "./tree";
        this->kBuf = 0;
        this->bufS = 1;
        mdb_env_create(&env);
        size_t new_mapsize = 1024 * 1024 * 1024; // 1 GB
        if (mdb_env_set_mapsize(env, new_mapsize) != MDB_SUCCESS) {
                fprintf(stderr, "Ошибка при установке нового размера базы данных\n");
                throw "error";
        }
        mdb_env_open(env, path, MDB_CREATE | MDB_WRITEMAP, 0664);
        mdb_txn_begin(env, nullptr, 0, &txn);
        mdb_dbi_open(txn, nullptr, 0, &dbi);
        mdb_cursor_open(txn, dbi, &cursor);
}


Btree::Btree(const char* path_, int bufferSize) {
	this->path = path_;
	this->kBuf = 0;
	this->bufS = bufferSize;
	mdb_env_create(&env);
	size_t new_mapsize = 1024 * 1024 * 1024; // 1 GB
    	if (mdb_env_set_mapsize(env, new_mapsize) != MDB_SUCCESS) {
        	fprintf(stderr, "Ошибка при установке нового размера базы данных\n");
        	throw "error";
    	}
	mdb_env_open(env, path, MDB_CREATE | MDB_WRITEMAP, 0664);
	mdb_txn_begin(env, nullptr, 0, &txn);
	mdb_dbi_open(txn, nullptr, 0, &dbi);
	mdb_cursor_open(txn, dbi, &cursor);
}

bool Btree::AddData(const string& key, const string& value, bool com) {
	this->key.mv_size = key.size();
	this->key.mv_data = (void *)key.c_str();
	this->data.mv_size = value.size();
	this->data.mv_data = (void *)value.c_str();
	mdb_put(this->txn, this->dbi, &this->key, &this->data, 0);
	kBuf++;
	if ( (kBuf >= this->bufS) || com) {
		mdb_txn_commit(txn);
		mdb_txn_begin(env, NULL, 0, &txn);
		kBuf = 0;
	}
	return true;
}

void Btree::SetKBuf(int bufs) {
	this->bufS = bufs;
}

void Btree::PrintAll() {
	MDB_val key, value;
	long long k = 0;
	while (mdb_cursor_get(cursor, &key, &value, MDB_NEXT) == MDB_SUCCESS) {
		k++;
	        printf("Ключ: %.*s, Значение: %.*s\n", (int)key.mv_size, (char *)key.mv_data, (int)value.mv_size, (char *)value.mv_data);
		std::cout << "els: " << k << endl;
	}
}

bool Btree::Close() {
	mdb_dbi_close(this->env, this->dbi);
	mdb_env_close(this->env);
}


map<string, string> Btree::GetData(const string& start, const string& end) {
	map<string, string> answer;

	MDB_val mdbKey, mdbValue;
	// Находим первую запись, которая больше или равна нижней границе
	mdbKey.mv_size = start.size();
	mdbKey.mv_data = const_cast<char*>(start.c_str());
	if (mdb_cursor_get(cursor, &mdbKey, &mdbValue, MDB_SET_RANGE) != MDB_SUCCESS) {
		return map<string, string>();
	} else {
		// Перебираем записи до тех пор, пока ключ меньше или равен верхней границе
		while (std::string(static_cast<char*>(mdbKey.mv_data), mdbKey.mv_size) <= end) {
			std::string keyStr(static_cast<char*>(mdbKey.mv_data), mdbKey.mv_size);
			std::string valueStr(static_cast<char*>(mdbValue.mv_data), mdbValue.mv_size);
			answer[keyStr] = valueStr;
			if (mdb_cursor_get(cursor, &mdbKey, &mdbValue, MDB_NEXT) != MDB_SUCCESS) {
				break;
			}
		}
	}
	return answer;

}


