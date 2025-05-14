#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <limits>
#include "database.h" // Using the database class from previous conversion

using namespace std;

class ToDoApp {
private:
    ToDoDataBase datBase;
     string currentInput;

    void displayMenu() {
         cout << "\n+---------------------------OPTIONS--------------------------+\n";
         cout << "[1] Add task   ";
         cout << "[2] Toggle tasks ";
         cout << "[3] Delete tasks    ";
         cout << "[4] Exit   ";
         cout << "\n+------------------------------------------------------------+\n";
         cout << "Choose an option: ";
    }

    void viewTasks() {
        const auto& tasks = datBase.getToDoList();
        if (tasks.empty()) {
             cout << "\nNo tasks found!\n";
            return;
        }

         cout << "\n+----------------------------TASKS---------------------------+\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            const auto& task = tasks[i];
             cout << i + 1 << ". [" << ( get<1>(task) ? "X" : " ") << "] "
                      <<  get<0>(task) << "\n";
        }

        
    }

    void addTask() {
        cout << "\n+---------------------------ADD TASKS-------------------------+\n";
         cout << "Enter new task: ";
         cin.ignore( numeric_limits< streamsize>::max(), '\n');
         getline( cin, currentInput);

        if (!currentInput.empty()) {
            auto tasks = datBase.getToDoList();
            tasks.emplace_back(currentInput, false);
            datBase.setToDoList(tasks);
            datBase.updateDataBase();
             cout << "Task added successfully!\n";
        }
    }

    void toggleTask() {
        viewTasks();
        if (datBase.getToDoList().empty()) return;

         cout << "Enter task number to toggle: ";
        size_t index;
         cin >> index;

        if (index > 0 && index <= datBase.getToDoList().size()) {
            auto tasks = datBase.getToDoList();
            auto& task = tasks[index - 1];
             get<1>(task) = ! get<1>(task); // Toggle completion status
            datBase.setToDoList(tasks);
            datBase.updateDataBase();
             cout << "Task updated!\n";
        } else {
             cout << "Invalid task number!\n";
        }
    }

    void deleteTask() {
        viewTasks();
        if (datBase.getToDoList().empty()) return;

         cout << "Enter task number to delete: ";
        size_t index;
         cin >> index;

        if (index > 0 && index <= datBase.getToDoList().size()) {
            auto tasks = datBase.getToDoList();
            tasks.erase(tasks.begin() + index - 1);
            datBase.setToDoList(tasks);
            datBase.updateDataBase();
             cout << "Task deleted!\n";
        } else {
             cout << "Invalid task number!\n";
        }
    }

public:
    void clearScreen() {
        cout << "\033[2J\033[1;1H";
    }

    void run() {
        datBase.loadData(); // Load existing data or create initial data

        int choice = 0;
        while (choice != 4) {
            
            viewTasks(); // display all to do list first
            displayMenu();
            
             cin >> choice;

            switch (choice) {
                case 1:
                    clearScreen();
                    viewTasks();
                    addTask();
                    break;
                case 2:
                    toggleTask();
                    break;
                case 3:
                    deleteTask();
                    break;
                case 4:
                     cout << "GoodatBaseye!\n";
                    break;
                default:
                     cout << "Invalid choice! Try again.\n";
                     cin.clear();
                     cin.ignore( numeric_limits< streamsize>::max(), '\n');
            }
        }
    }
};

int main() {
    ToDoApp app;
    app.run();
    return 0;
}