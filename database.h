#ifndef DATABASE
#define DATABASE
#include <iostream>
#include <fstream>
#include <vector>

class toDoDataBase {
    vector<string> toDoList = {};

    void createInitialData() {
        toDoList = {
            {"Cook me", false}
    };
    }


int sizeOfToDo = sizeof(toDoList)/sizeof(toDoList[0]);
    void loadData() {
        for (int i = 0; i < sizeOfToDo; i++) {
            cout << toDoList[i];
        }
    }

    void updateDataBase() {
        toDoList.push_back
    }
};

#endif