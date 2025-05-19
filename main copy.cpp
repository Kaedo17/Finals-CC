#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <limits>
#include "database.h"

using namespace std;

class ToDoApp {
private:
    ToDoDataBase datBase;
     string currentInput;

    // ANSI color codes for terminal formatting
    const  string RED = "\033[31m";
    const  string GREEN = "\033[32m";
    const  string YELLOW = "\033[33m";
    const  string BLUE = "\033[34m";
    const  string RESET = "\033[0m";  // Reset formatting
    const  string STRIKE = "\033[9m"; // Strikethrough

    // Clear terminal screen
    void clearScreen() {
         cout << "\033[2J\033[1;1H"; // ANSI escape codes
    }

    // Display main menu options
    void displayMenu() {
         cout << "\n" << BLUE << "+-----------------------OPTIONS-----------------------+" << RESET << "\n";
         cout << "[1] Add task   [2] Toggle complete   [3] Set priority\n";
         cout << "[4] Delete     [5] Exit\n";
         cout << BLUE << "+----------------------------------------------------+" << RESET << "\n";
         cout << "Choose an option: ";
    }

    // Display all tasks with formatting
    void viewTasks() {
        auto& tasks = datBase.getToDoList();
        if (tasks.empty()) {
             cout << "\nNo tasks found!\n";
            return;
        }

         cout << "\n" << GREEN << "+-----------------------TASKS-----------------------+" << RESET << "\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            // Unpack tuple elements
            const auto& [task, completed, priority] = tasks[i];
            
            // Determine color based on status and priority
             string color;
            if (completed) {
                color = STRIKE + RED; // Completed = strikethrough red
            } else if (priority == 1) {
                color = YELLOW;      // High priority = yellow
            } else if (priority == 2) {
                color = GREEN;        // Medium priority = green
            } else {
                color = BLUE;        // Low priority = blue
            }

            // Format task display
             cout << i + 1 << ". " << color 
                      << (completed ? "[X] " : "[ ] ") 
                      << task << RESET << "\n";
        }
    }

    // Add new task with default medium priority
    void addTask() {
         cout << "\nEnter new task: ";
         cin.ignore(); // Clear input buffer
         getline( cin, currentInput);

        if (!currentInput.empty()) {
            auto& tasks = datBase.getToDoList();
            tasks.emplace_back(currentInput, false, 2); // Priority 2 = Medium
            datBase.updateDataBase();
            clearScreen();
             cout << "Task added!\n";
        }
    }

    // Toggle completion status
    void toggleTask() {
        size_t index;
         cout << "Toggle task number: ";
         cin >> index;

        auto& tasks = datBase.getToDoList();
        if (index > 0 && index <= tasks.size()) {
            auto& [task, completed, priority] = tasks[index-1];
            completed = !completed; // Flip completion status
            datBase.updateDataBase();
            clearScreen();
             cout << "Task toggled!\n";
        } else {
             cout << "Invalid task number!\n";
        }
    }

    // Set task priority (1-3)
    void setPriority() {
        size_t index;
        int priority;
         cout << "Task number: ";
         cin >> index;
         cout << "Priority (1=High, 2=Medium, 3=Low): ";
         cin >> priority;

        auto& tasks = datBase.getToDoList();
        if (index > 0 && index <= tasks.size() && priority >= 1 && priority <= 3) {
             get<2>(tasks[index-1]) = priority; // Set priority value
            datBase.updateDataBase();
            clearScreen();
             cout << "Priority updated!\n";
        } else {
             cout << "Invalid input!\n";
        }
    }

    // Delete a task
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

public:
    void run() {
        datBase.loadData();
        int choice = 0;

        while (choice != 5) {
            clearScreen();
            viewTasks();  // Always show current tasks
            displayMenu();
             cin >> choice;

            switch (choice) {
                case 1: addTask(); break;
                case 2: toggleTask(); break;
                case 3: setPriority(); break;
                case 4: deleteTask(); break;
                case 5: clearScreen();  cout << "Goodbye!\n"; break;
                default: 
                     cout << "Invalid choice!\n";
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