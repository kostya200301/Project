#include <iostream>
#include <fstream>
#include <chrono>
#include "Action.h"
#include "QueActions.h"
#include "Baket.h"


#include <rocksdb/db.h>
#include <rocksdb/merge_operator.h>
#include <rocksdb/slice.h>
class MyMergeOperator : public rocksdb::MergeOperator {
public:
    virtual ~MyMergeOperator() {}
    // Метод для выполнения полного объединения (Full Merge)
    virtual bool FullMerge(const rocksdb::Slice& key,
                           const rocksdb::Slice* existing_value,
                           const std::deque<std::string>& operand_list,
                           std::string* new_value,
                           rocksdb::Logger* logger) const override {
        // Реализуйте вашу логику объединения данных (Full Merge) здесь
        // operand_list содержит последовательность операций объединения, front() первый операнд

        // Пример: конкатенация всех операндов
        for (const std::string& operand : operand_list) {
            *new_value += operand + " ";
        }

        // Если существующее значение существует, добавьте его к результату
        if (existing_value) {
            *new_value += existing_value->ToString();
        }

        return true;
    }
    // Метод для выполнения частичного объединения (Partial Merge)
    virtual bool PartialMerge(const rocksdb::Slice& key,
                              const rocksdb::Slice& left_operand,
                              const rocksdb::Slice& right_operand,
                              std::string* new_value,
                              rocksdb::Logger* logger) const override {
        // Реализуйте вашу логику частичного объединения данных (Partial Merge) здесь

        // Пример: конкатенация левого и правого операндов
        *new_value = left_operand.ToString() + " " + right_operand.ToString();

        return true;
    }

    // Возвращаем имя оператора объединения
    virtual const char* Name() const override {
        return "MyMergeOperator";
    }
};





void MakeQueue(QueActions<Action>& que) {
	std::ifstream inputFile("topic.txt"); // Замените "example.txt" на путь к вашему файлу

    if (!inputFile.is_open()) {
        std::cerr << "Не удалось открыть файл!" << std::endl;
        return;
    }

    std::string line;
	int i = 0;
    while (std::getline(inputFile, line)) {
	if  (i % 100 == 49) {
		break;
	}
	Action act = Action(line);
	que.Add(act);
	i++;
    }

    inputFile.close();

}

void printRocksDB(const std::string& db_path) {
	MyMergeOperator merge_operator;

    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
	options.merge_operator.reset(new MyMergeOperator);
    rocksdb::Status status = rocksdb::DB::Open(options, db_path, &db);

    if (!status.ok()) {
        std::cerr << "Ошибка при открытии базы данных: " << status.ToString() << std::endl;
        return;
    }

    rocksdb::ReadOptions read_options;
    rocksdb::Iterator* it = db->NewIterator(read_options);

    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        std::string key = it->key().ToString();
        std::string value = it->value().ToString();
        std::cout << "Ключ: " << key << ", Значение: " << value << std::endl;
    }

    if (!it->status().ok()) {
        std::cerr << "Ошибка при обходе базы данных: " << it->status().ToString() << std::endl;
    }
	std::cout << "END\n";
    delete it;
    delete db;
}


int main() {
	//printRocksDB("./data/index/121/201/12120147738807884196/BI/10/37/10373397421197347548");

	//exit(0);
	QueActions<Action> queue;
	MakeQueue(queue);
	 auto currentTime = std::chrono::system_clock::now();
	    uint64_t currentTimeInSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count();

	Baket b(currentTimeInSeconds, 25000);
	int u = 0;
	while (queue.GetSize() != 0) {
		cout << u << endl;
		Action a = queue.Get();
		b.Add(a, 1);
	}
}
