#include <iostream>
#include <string>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char **argv) {

    if (argc < 2) {
        cerr << "error: Not enough arguments." << endl;
        return -1;
    }
    fs::path directory = argv[1]; 

    /* settings */
    int mode = 1; /* 1 - characters, 0 - lines  */
    bool count_all = false; /* count characters, lines */
    bool sort_bool = false;   /* sort the output */
    int sort_type = 1;   /* 1 - decreasing, 0 - increasing */
    bool show_count = false; /* show the total number of characters from extension */
    bool show_total_count = false;
    double dp_range = 100.00; /* range after decimal point */
    vector<string> ignored_directories = {};
    vector<string> ignored_extensions = {}; /* ignored file extensions */

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) { /* help operation */
        const char *arg0 = argv[0];
        cout << "General:" << endl 
        << "\t" << arg0 << " <directory path> - check specified directory.\n"
        << "\t" << arg0 << " -h, --help - shows this message.\n"
        << "Arguments:" << endl 
        << "\t" << arg0 << " <path> -m, --mode <l/c>/<lines/characters> - choose between two modes, counting by lines or characters.\n"
        << "\t" << arg0 << " <path> -r, --range <range> - define range of numbers after the decimal point (range must be a natural number).\n"
        << "\t" << arg0 << " <path> -a, --amount - include the amount of lines/characters in the ouput.\n"
        << "\t" << arg0 << " <path> -s, --sort <increasing/decreasing>/<i/d> - sort the output increasing or decreasing (i - increasing, d - decreasing).\n"
        << "\t" << arg0 << " <path> -t, --total - show total lines.\n";
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
            dp_range = pow(10, stoi(argv[i+1])); 
            continue;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--amount") == 0) {
            show_count = true;
            continue;
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--total") == 0 ) {
            show_total_count = true;
            continue;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--sort") == 0) {
            if (argv[i+1] == NULL) {
                cerr << "error: Missing sorting type." << endl;
                return -1;
            }
            else if (strcmp(argv[i+1], "i") == 0 || strcmp(argv[i+1], "increasing") == 0) {
                sort_bool = true;
                sort_type = 0;
                continue;
            } else if (strcmp(argv[i+1], "d") == 0 || strcmp(argv[i+1], "decreasing") == 0) {
                sort_bool = true;
                sort_type = 1;
                continue;
            } else {
                cerr << "error: Unknown sorting type: '" << argv[i+1] << "'." << endl;
                return -1;
            }
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--mode") == 0) {
            if (argv[i+1] == NULL) {
                cerr << "error: Missing mode." << endl;
                return -1; 
            }
            if (strcmp(argv[i+1], "lines") == 0 || strcmp(argv[i+1], "l") == 0) {
                mode = 0;
                continue;
            } else if (strcmp(argv[i+1], "characters") == 0 || strcmp(argv[i+1], "c") == 0) {
                mode = 1;
                continue;
            } else {
                cerr << "error: Unknown mode: '" << argv[i+1] << "'." << endl; 
            }
        }
    }
    unsigned long long total_lines = 0;
    unsigned long long total_chars = 0; /* total characters*/
    unordered_map<string, unsigned long long> chars_dict; /* store the file extensions total characters */
    unordered_map<string, unsigned long long> lines_dict;

    for (const auto& dirEntry : fs::recursive_directory_iterator(directory)) {
        if (fs::is_regular_file(dirEntry)) { /* checking only regular files*/
            bool skip = false;
            string extension = dirEntry.path().extension().string();
            for (const auto& ignored_extension : ignored_extensions) {
                if (ignored_extension == extension) {
                    skip = true;
                }
            } 
            if (skip) {continue;}
            
            else if (extension.empty()) {
                continue;
            }
            ifstream fin(dirEntry.path()); 
            unsigned long long tmp_total_chars = 0; /* store current file total characters */
            unsigned long long tmp_total_lines = 0; 
            string line;
            while (getline(fin, line)) {
                tmp_total_chars += line.size();
                tmp_total_lines++;
            }
            chars_dict[extension] += tmp_total_chars;
            lines_dict[extension] += tmp_total_lines;
            total_chars += tmp_total_chars;
            total_lines += tmp_total_lines;  
        } else {
            continue; 
        }
    }

    vector<pair<string, unsigned long long>> chars_vec(chars_dict.begin(), chars_dict.end());
    vector<pair<string, unsigned long long>> lines_vec(lines_dict.begin(), lines_dict.end());

    if (sort_bool) { /* sorting */
        if (sort_type == 1) { /* decreasing */
            sort(chars_vec.begin(), chars_vec.end(), [](const auto& a, const auto& b) {
                return a.second > b.second;  
            });
            sort(lines_vec.begin(), lines_vec.end(), [](const auto& a, const auto& b) {
                return a.second > b.second;  
            });
        } else if (sort_type == 0) {
            sort(chars_vec.begin(), chars_vec.end(), [](const auto& a, const auto& b) {
                return a.second < b.second;  
            });
            sort(lines_vec.begin(), lines_vec.end(), [](const auto& a, const auto& b) {
                return a.second < b.second;  
            });
        }
    }
    if (mode == 1) { /* characters */
        for (const auto& pair : chars_vec) {
            string extension = pair.first;
            unsigned long long chars = pair.second;
            double percentage = round((((double)chars/(double)total_chars) * 100.00) * dp_range) / dp_range;
            cout << "[" << extension << "] - " << percentage << "%";
            if (show_count) {
                cout << ",  characters - " << chars;
            }
            cout << endl;
        }  
        if (show_total_count) {cout << "total characters - " << total_chars << endl;}
    } else if (mode == 0) { /* lines */
        for (const auto& pair : lines_vec) {
            string extension = pair.first;
            unsigned long long lines = pair.second;
            double percentage = round((((double)lines/(double)total_lines) * 100.00) * dp_range) / dp_range;
            cout << "[" << extension << "] - " << percentage << "%";
            if (show_count) {
                cout << ",  lines - " << lines;
            }
            cout << endl;
            
        }
        if (show_total_count) {cout << "total lines - " << total_lines << endl;}
    } else {
        cerr << "error: Unknown mode: " << mode << endl;    
        return -1;
    }
    return 0;
}
