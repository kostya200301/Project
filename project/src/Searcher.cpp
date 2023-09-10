#include <filesystem>
#include <string>

#include "Searcher.h"

namespace fs = std::filesystem;

void listFiles(const fs::path& dirPath) {
    for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
        if (fs::is_regular_file(entry)) {
            std::cout << entry.path() << std::endl;
        }
    }
}

Searcher::Searcher(){}

Searcher::Searcher(Request& r){
	cout << "in SerCr\n";
	this->treeBakets = Btree("./testdb", 1);
	this->req = r;
}

map<string, string> Searcher::GetBakets() {
	return this->treeBakets.GetData(this->req.start_time, this->req.end_time);
}

vector<string> GetUUIDtoActionsByRequest() {

}

void Searcher::PrintAll(const std::string& dir) {

	for (auto el : GetBakets()) {
		string keyWord = this->req.keyWords[0];
		//Baket b = Baket();

		cout << el.first << " " << el.second << endl;
	}


//    fs::path folderPath = dir; // Замените на путь к вашей папке
//    if (fs::is_directory(folderPath)) {
//        listFiles(folderPath);
//    } else {
//        std::cerr << "Invalid folder path." << std::endl;
//    }


}
