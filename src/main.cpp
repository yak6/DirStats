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
    bool sort = false;   /* sort the output */
    int sort_type = 1;   /* 1 - decreasing, 0 - increasing */
    bool show_chars = false; /* show the total number of characters from extension */
    bool show_total_chars = false;
    double dp_range = 100.00; /* range after decimal point */
    vector<string> ignored_extensions = {
        ".log", ".bak", ".tmp",    
        ".swp", ".sublime-workspace", 
        ".DS_Store", ".thumbs", ".git",      
        ".gitignore", ".idea", ".vscode", ".exe",     
        ".dll", ".o", ".a", ".pdb", ".class", ".pyc",      
        ".jar", ".war", ".tar", ".zip", ".gz",      
        ".rar", ".7z", ".bak", ".aux", ".lof",      
        ".toc", ".idx", ".bbl", ".fdb_latexmk" 
    }; /* ignored file extensions */

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) { /* help operation */
        const char *arg0 = argv[0];
        cout << "General:" << endl 
        << "\t" << arg0 << " <directory path> - check specified directory.\n"
        << "\t" << arg0 << " -h, --help - shows this message.\n"
        << "Arguments:" << endl 
        << "\t" << arg0 << " <path> -r, --range <range> - define range of numbers after the decimal point (range must be a natural number).\n"
        << "\t" << arg0 << " <path> -c, --chars - show total characters next to the percentage.\n"
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
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--characters") == 0) {
            show_chars = true;
            continue;
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--total") == 0 ) {
            show_total_chars = true;
            continue;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--sort") == 0) {
            if (argv[i+1] == NULL) {
                cerr << "error: Missing sorting type." << endl;
                return -1;
            }
            else if (strcmp(argv[i+1], "i") == 0 || strcmp(argv[i+1], "increasing") == 0) {
                sort = true;
                sort_type = 0;
                continue;
            } else if (strcmp(argv[i+1], "d") == 0 || strcmp(argv[i+1], "decreasing") == 0) {
                sort = true;
                sort_type = 1;
                continue;
            } else {
                cerr << "error: Unknown sorting type: '" << argv[i+1] << "'." << endl;
                return -1;
            }
        } 
    }

    unsigned long long total_chars = 0; /* total characters*/
    unordered_map<string, unsigned long long> chars_dict; /* store the file extensions total characters */

    for (const auto& dirEntry : fs::recursive_directory_iterator(directory)) {
        if (fs::is_regular_file(dirEntry)) { /* checking only regular files*/
            bool skip = false;
            string extension = dirEntry.path().extension().string();
            for (const auto& ignored_extension : ignored_extensions) {
                if (ignored_extension == extension) {
                    skip = true;
                }
            } if (skip) {continue;}
            
            else if (extension.empty()) {
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
    }

    std::vector<std::pair<std::string, unsigned long long>> chars_vec(chars_dict.begin(), chars_dict.end());

    if (sort) { /* sorting */
        if (sort_type == 1) { /* decreasing */
            std::sort(chars_vec.begin(), chars_vec.end(), [](const auto& a, const auto& b) {
                return a.second > b.second;  
            });
        } else if (sort_type == 0) {
            std::sort(chars_vec.begin(), chars_vec.end(), [](const auto& a, const auto& b) {
                return a.second < b.second;  
            });
        }
    }

    if (show_total_chars) {cout << "total characters - " << total_chars << endl;}
    
    for (const auto& pair : chars_vec) {
        string extension = pair.first;
        unsigned long long chars = pair.second;
        double percentage = round((((double)chars/(double)total_chars) * 100.00) * dp_range) / dp_range;
        if (show_chars) {
            cout << extension << " - " << percentage << "% | characters - " << chars << endl;
        } else {
            cout << extension << " - " << percentage << "%" << endl; 
        }
    } 
    return 0;
}
