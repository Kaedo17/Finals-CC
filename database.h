#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <ctime>
#include <algorithm>  // Added for  sort
#include <iomanip>    // Added for time formatting

using namespace std;

class ToDoDataBase {
private:
    // Format: (description, completed, priority, creation_time)
     vector< tuple< string, bool, int,  time_t>> toDoList;
     string filename = "tododatabase.txt";

    void saveToFile() {
         ofstream outFile(filename);
        for (const auto& task : toDoList) {
            outFile <<  get<0>(task) << "\n"
                    <<  get<1>(task) << "\n"
                    <<  get<2>(task) << "\n"
                    <<  get<3>(task) << "\n";
        }
    }

    void loadFromFile() {
         ifstream inFile(filename);
        if (!inFile) {
            createInitialData();
            return;
        }
        
        toDoList.clear();
         string task;
        bool completed;
        int priority;
         time_t created;
        
        while ( getline(inFile, task)) {
            inFile >> completed >> priority >> created;
            inFile.ignore();
            toDoList.emplace_back(task, completed, priority, created);
        }
    }

public:
    void createInitialData() {
        toDoList = {{"Cook an egg!", false, 2,  time(nullptr)}};
        saveToFile();
    }

    void addTask(const  string& desc, int priority = 2) {
        toDoList.emplace_back(desc, false, priority,  time(nullptr));
        saveToFile();
    }

    void sortByPriority() {
         sort(toDoList.begin(), toDoList.end(),
            [](const auto& a, const auto& b) {
                return  get<2>(a) <  get<2>(b);
            });
    }

    void sortAlphabetically() {
    sort(toDoList.begin(), toDoList.end(),
        [](const auto& a, const auto& b) {
            const string& strA = get<0>(a);
            const string& strB = get<0>(b);
            
            // Compare character by character case-insensitively
            return lexicographical_compare(
                strA.begin(), strA.end(),
                strB.begin(), strB.end(),
                [](char c1, char c2) {
                    return tolower(c1) < tolower(c2);
                });
        });
}

    void sortByCreationDate() {
         sort(toDoList.begin(), toDoList.end(),
            [](const auto& a, const auto& b) {
                return  get<3>(a) >  get<3>(b);
            });
    }

    auto& getToDoList() { return toDoList; }
    void loadData() { loadFromFile(); }
    void updateDataBase() { saveToFile(); }
};

#endif