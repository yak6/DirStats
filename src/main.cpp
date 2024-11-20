#include <iostream>
#include <string>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <cctype>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "error: Not enough arguments." << endl;
        return -1;
    }
  
    double range = 100.00;  /* this var specifies the amount of numbers after the decimal point when showing the percentage. By increasing this variable, the range will increase. */
    fs::path directory = argv[1]; /* first argument is the directory path. */
    bool show_chars = false; /* show the total number of characters in file */

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[0], "--help") == 0) { /* help operation */
        const char *arg0 = argv[0];
        cout << "General:" << endl 
        << "\t" << arg0 << " <directory path> - check specified directory.\n"
        << "\t" << arg0 << " -h, --help - shows this message.\n"
        << "Arguments:" << endl 
        << "Remember, these arguments can be used after specifing the directory path as the first argument!\n"
        << "\t" << arg0 << " -r, --range <range> - define range of numbers after the decimal point (range must be a natural number).\n"
        << "\t" << arg0 << " -c, --chars - show total characters next to the percentage.\n";
        return 0;
    }

    if (!fs::exists(directory)) {
        cerr << "error: " << directory << " doesn't exist." << endl;
        return -1;
    } else if (!fs::is_directory(directory)) {
        cerr << "error: " << directory << " is not a directory." << endl;
        return -1;
    }
    
    for (int i = 2; i < argc; i++) { /* parsing arguments */
        if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--range") == 0) {
            if (argv[i+1] == NULL) {
                cerr << "error: Missing range." << endl;
                return -1;
            }
            range = pow(10, stoi(argv[i+1])); 
            continue;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--characters") == 0) {
            show_chars = true;
            continue;
        }

    }
  
    unordered_map<string, unsigned long long> chars_dict; /* file extension : total characters */

    unsigned long long total_chars = 0; /* total number of characters in all files*/

    for (const auto& dirEntry : fs::recursive_directory_iterator(directory)) {
        if (fs::is_regular_file(dirEntry)) { /* checking only regular files*/
            string extension = dirEntry.path().extension().string();
            if (extension.empty()) {
                continue;
            }
            ifstream fin(dirEntry.path()); 
            unsigned long long tmp_total_chars = 0; /* store current file total characters */
            string line;
            while (std::getline(fin, line)) {
                tmp_total_chars += line.size();
            }
            chars_dict[extension] += tmp_total_chars;
            total_chars += tmp_total_chars; 
        } else {
            continue; 
        }
    } for (const auto& [extension, chars] : chars_dict) {
        if (show_chars) {
            cout << "[" << extension << "] - " << (round((((double)chars/(double)total_chars) * 100.00) * range) / range) << "%, " << chars << " characters" << endl;
        } else {
            cout << "[" << extension << "] - " << (round((((double)chars/(double)total_chars) * 100.00) * range) / range) << "%" << endl;
        }
    } 
    return 0;
}
