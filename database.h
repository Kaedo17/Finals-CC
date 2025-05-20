// database.h - Updated with due date support
#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

class ToDoDataBase {
private:
    // Updated tuple format: (description, completed, priority, creation_time, due_date)
    // due_date is stored as time_t (0 means no due date)
    vector<tuple<string, bool, int, time_t, time_t>> toDoList;
    string filename = "tododatabase.txt";

    void saveToFile() {
        ofstream outFile(filename);
        for (const auto& task : toDoList) {
            outFile << get<0>(task) << "\n"
                    << get<1>(task) << "\n"
                    << get<2>(task) << "\n"
                    << get<3>(task) << "\n"
                    << get<4>(task) << "\n"; // Added due date to file output
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
        time_t due_date; // Added due date variable
        
        while (getline(inFile, task)) {
            inFile >> completed >> priority >> created >> due_date; // Added due date reading
            inFile.ignore();
            toDoList.emplace_back(task, completed, priority, created, due_date);
        }
    }

public:
    void createInitialData() {
        toDoList = {{"Cook an egg!", false, 2, time(nullptr), 0}}; // 0 means no due date
        saveToFile();
    }

    // Updated addTask with optional due_date parameter (defaults to 0 - no due date)
    void addTask(const string& desc, int priority = 2, time_t due_date = 0) {
        toDoList.emplace_back(desc, false, priority, time(nullptr), due_date);
        saveToFile();
    }

    // Existing sort functions remain the same
    void sortByPriority() {
        sort(toDoList.begin(), toDoList.end(),
            [](const auto& a, const auto& b) {
                return get<2>(a) < get<2>(b);
            });
    }

    void sortAlphabetically() {
        sort(toDoList.begin(), toDoList.end(),
            [](const auto& a, const auto& b) {
                const string& strA = get<0>(a);
                const string& strB = get<0>(b);
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
                return get<3>(a) > get<3>(b);
            });
    }

    auto& getToDoList() { return toDoList; }
    void loadData() { loadFromFile(); }
    void updateDataBase() { saveToFile(); }
};

#endif