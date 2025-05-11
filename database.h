#ifndef DATABASE
#define DATABASE

#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <iostream>
#include <stdexcept>

class ToDoDataBase {
private:
    std::vector<std::tuple<std::string, bool>> toDoList;
    std::string filename = "tododatabase.dat";

    // Helper function to serialize data to file
    void saveToFile() {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            throw std::runtime_error("Could not open file for writing");
        }

        size_t size = toDoList.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        
        for (const auto& item : toDoList) {
            const std::string& task = std::get<0>(item);
            bool completed = std::get<1>(item);
            
            size_t taskSize = task.size();
            outFile.write(reinterpret_cast<const char*>(&taskSize), sizeof(size_t));
            outFile.write(task.c_str(), taskSize);
            outFile.write(reinterpret_cast<const char*>(&completed), sizeof(bool));
        }
    }

    // Helper function to deserialize data from file
    void loadFromFile() {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            // File doesn't exist yet, use initial data
            createInitialData();
            return;
        }

        size_t size;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        
        toDoList.clear();
        toDoList.reserve(size);
        
        for (size_t i = 0; i < size; ++i) {
            size_t taskSize;
            inFile.read(reinterpret_cast<char*>(&taskSize), sizeof(size_t));
            
            std::string task(taskSize, '\0');
            inFile.read(&task[0], taskSize);
            
            bool completed;
            inFile.read(reinterpret_cast<char*>(&completed), sizeof(bool));
            
            toDoList.emplace_back(task, completed);
        }
    }

public:
    // First time open the app
    void createInitialData() {
        toDoList = {
            {"Cook an egg!", false}
        };
        saveToFile();
    }

    // Load data from the database
    void loadData() {
        loadFromFile();
    }

    // Update database
    void updateDataBase() {
        saveToFile();
    }

    // Getter for the to-do list
    const std::vector<std::tuple<std::string, bool>>& getToDoList() const {
        return toDoList;
    }

    // Setter for the to-do list
    void setToDoList(const std::vector<std::tuple<std::string, bool>>& newList) {
        toDoList = newList;
    }
};

#endif 