#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <dirent.h> // Linux Only

using namespace std;

namespace vital {
    string fileRead(const string& path){
        const char* char_path = path.c_str();
        ifstream ifs(char_path);
        string buffer;
        string doc;

        // 全部読み込んでみる
        while (getline(ifs, buffer) ) {
            doc += buffer;
            doc += "\n";
        }

        ifs.close();

        return doc;
    }


    vector<string> split(const string &str, char delim){
        vector<string> res;
        size_t current = 0, found;
        while((found = str.find_first_of(delim, current)) != string::npos){
            res.push_back(string(str, current, found - current));
            current = found + 1;
        }
        res.push_back(string(str, current, str.size() - current));
        return res;

    }

    vector<string> GetFileList(string directory_path){
        vector<string> file_list;
        const char* char_path = directory_path.c_str();

        DIR *dp = opendir(char_path);
        if (!dp) {
            // Error process
        }

        struct dirent *dir;
        while ((dir = readdir(dp)) != NULL) {
            string file_name = dir->d_name;

            if (file_name == "." || file_name == "..") {
                continue;
            }

            file_list.push_back(file_name);
        }

        closedir(dp);

        return file_list;
    }
} // namespace vital
