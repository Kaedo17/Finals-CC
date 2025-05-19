#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <limits>
#include <ctime>
#include <iomanip>
#include "database.h"

using namespace std;

class ToDoApp {
private:
    ToDoDataBase datBase;
     string currentInput;

    // ANSI color codes
    const  string RED = "\033[31m";
    const  string GREEN = "\033[32m";
    const  string YELLOW = "\033[33m";
    const  string BLUE = "\033[34m";
    const  string MAGENTA = "\033[35m";
    const  string RESET = "\033[0m";
    const  string STRIKE = "\033[9m";
    const  string BOLD = "\033[1m";

    void clearScreen() {
         cout << "\033[2J\033[1;1H";
    }

    void displayMainMenu() {
         cout << "\n" << BOLD << BLUE << "+-----------------------MAIN MENU-----------------------+" << RESET << "\n";
         cout << "[1] Add task      [2] Toggle complete  [3] Set priority\n";
         cout << "[4] Delete task  [5] Sort tasks      [6] Exit\n";
         cout << BOLD << BLUE << "+----------------------------------------------------+" << RESET << "\n";
         cout << "Choose an option: ";
    }

    void displaySortMenu() {
         cout << "\n" << BOLD << MAGENTA << "+-----------------------SORT BY-----------------------+" << RESET << "\n";
         cout << "[1] Priority (High to Low)  [2] Alphabetical (A-Z)\n";
         cout << "[3] Creation Date (Newest)  [4] Back to Main Menu\n";
         cout << BOLD << MAGENTA << "+----------------------------------------------------+" << RESET << "\n";
         cout << "Choose sort option: ";
    }

    void viewTasks(bool showPriorityHeaders = true) {
        auto& tasks = datBase.getToDoList();
        if (tasks.empty()) {
             cout << "\nNo tasks found!\n";
            return;
        }

         vector< tuple< string, bool, int,  time_t>> highPriority;
         vector< tuple< string, bool, int,  time_t>> normalPriority;

        for (const auto& task : tasks) {
            if ( get<2>(task) == 1) {
                highPriority.push_back(task);
            } else {
                normalPriority.push_back(task);
            }
        }

         cout << "\n" << BOLD << GREEN << "+-----------------------TASKS-----------------------+" << RESET << "\n";
        
        if (!highPriority.empty() && showPriorityHeaders) {
             cout << YELLOW << "            >>> HIGH PRIORITY TASKS <<<\n" << RESET;
            displayTaskList(highPriority);
        }

        if (!normalPriority.empty() && showPriorityHeaders) {
             cout << BLUE << "\n              >>> NORMAL TASKS <<<\n" << RESET;
        }
        displayTaskList(normalPriority);
    }

    // Changed from auto to explicit type
    void displayTaskList(const  vector< tuple< string, bool, int,  time_t>>& tasks) {
        for (size_t i = 0; i < tasks.size(); ++i) {
            const auto& [task, completed, priority, created] = tasks[i];
            
             string color;
            if (completed) {
                color = STRIKE + RED;
            } else if (priority == 1) {
                color = YELLOW;
            } else if (priority == 2) {
                color = BLUE;
            }

             tm* ptm =  localtime(&created);
            char buffer[32];
             strftime(buffer, 32, "%Y-%m-%d %H:%M", ptm);

             cout << i + 1 << ". " << color 
                      << (completed ? "[X] " : "[ ] ") 
                      << left << setw(35) << task << RESET
                      << MAGENTA << "(" << buffer << ")" << RESET << "\n";
        }
    }

    void addTask() {
         cout << "\nEnter new task: ";
         cin.ignore();
         getline( cin, currentInput);

        if (!currentInput.empty()) {
            int priority;
             cout << "Set priority (1=High, 2=Low): ";
             cin >> priority;
            if (priority < 1 || priority > 2) priority = 2;
            
            datBase.addTask(currentInput, priority);
            clearScreen();
             cout << GREEN << "Task added successfully!\n" << RESET;
        }
    }

    void toggleTask() {
        size_t index;
         cout << "Toggle task number: ";
         cin >> index;

        auto& tasks = datBase.getToDoList();
        if (index > 0 && index <= tasks.size()) {
            auto& task = tasks[index-1];
             get<1>(task) = ! get<1>(task); // Toggle completion status
            datBase.updateDataBase();
            clearScreen();
             cout << "Task toggled!\n";
        } else {
             cout << "Invalid task number!\n";
        }
    }

    void setPriority() {
        size_t index;
        int priority;
         cout << "Task number: ";
         cin >> index;
         cout << "Priority (1=High, 2=Medium, 3=Low): ";
         cin >> priority;

        auto& tasks = datBase.getToDoList();
        if (index > 0 && index <= tasks.size() && priority >= 1 && priority <= 3) {
             get<2>(tasks[index-1]) = priority;
            datBase.updateDataBase();
            clearScreen();
             cout << "Priority updated!\n";
        } else {
             cout << "Invalid input!\n";
        }
    }

    void deleteTask() {
        size_t index;
         cout << "Delete task number: ";
         cin >> index;

        auto& tasks = datBase.getToDoList();
        if (index > 0 && index <= tasks.size()) {
            tasks.erase(tasks.begin() + index - 1);
            datBase.updateDataBase();
            clearScreen();
             cout << "Task deleted!\n";
        } else {
             cout << "Invalid task number!\n";
        }
    }

    void sortTasks() {
        int choice;
        do {
            clearScreen();
            viewTasks(false);
            displaySortMenu();
             cin >> choice;

            switch (choice) {
                case 1:
                    datBase.sortAlphabetically();
                    clearScreen();
                     cout << GREEN << "Tasks sorted alphabetically!\n" << RESET;
                    viewTasks();
                    break;
                case 2:
                    datBase.sortByCreationDate();
                    clearScreen();
                     cout << BLUE << "Tasks sorted by creation date!\n" << RESET;
                    viewTasks();
                    break;
                case 3:
                    break;
                default:
                     cout << RED << "Invalid option!\n" << RESET;
            }
        } while (choice != 4);
    }

public:
    void run() {
        datBase.loadData();
        int choice = 0;

        while (choice != 6) {
            clearScreen();
            viewTasks();
            displayMainMenu();
             cin >> choice;

            switch (choice) {
                case 1: addTask(); break;
                case 2: toggleTask(); break;
                case 3: setPriority(); break;
                case 4: deleteTask(); break;
                case 5: sortTasks(); break;
                case 6: clearScreen();  cout << GREEN << "Goodbye!\n" << RESET; break;
                default:
                     cout << RED << "Invalid choice!\n" << RESET;
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