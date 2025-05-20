// main.cpp - Updated with due date functionality
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
  const string RED = "\033[31m";
  const string GREEN = "\033[32m";
  const string YELLOW = "\033[33m";
  const string BLUE = "\033[34m";
  const string MAGENTA = "\033[35m";
  const string RESET = "\033[0m";
  const string STRIKE = "\033[9m";
  const string BOLD = "\033[1m";

  void clearScreen() {
    cout << "\033[2J\033[1;1H";
  }

  void displayMainMenu() {
    cout << "\n" << BOLD << BLUE << "+-----------------------MAIN MENU-----------------------+" << RESET << "\n";
    cout << "[1] Add task      [2] Toggle complete  [3] Pin/Unpin Task\n";
    cout << "[4] Delete task   [5] Sort tasks      [6] Set due date\n"; // Added due date option
    cout << "[7] Exit\n"; // Changed from 6 to 7
    cout << BOLD << BLUE << "+----------------------------------------------------+" << RESET << "\n";
    cout << "Choose an option: ";
  }

  void displaySortMenu() {
    cout << "\n" << BOLD << MAGENTA << "+-----------------------SORT BY-----------------------+" << RESET << "\n";
    cout << "[1] Alphabetical (A-Z)  [2] Creation Date (Newest)\n";
    cout << "[3] Back to Main Menu\n";
    cout << BOLD << MAGENTA << "+----------------------------------------------------+" << RESET << "\n";
    cout << "Choose sort option: ";
  }

  /* NEW FUNCTION: Displays a simple calendar for due date selection
       Parameters:
       - reference_time: The time to display calendar for (defaults to current time)
       Returns: void (outputs to console) */
  void displayCalendar(time_t reference_time = time(nullptr)) {
    tm* timeinfo = localtime(&reference_time);
    int current_month = timeinfo->tm_mon;
    int current_year = timeinfo->tm_year + 1900;

    cout << "\n" << BOLD << BLUE << "   " << put_time(timeinfo, "%B %Y") << RESET << "\n";
    cout << " Su Mo Tu We Th Fr Sa\n";

    // Find first day of month
    tm first_day = *timeinfo;
    first_day.tm_mday = 1;
    mktime(&first_day);
    int start_day = first_day.tm_wday;

    // Print leading spaces
    for (int i = 0; i < start_day; i++) cout << "   ";

    // Print days
    int days_in_month;
    if (current_month == 1) {
      // February
      days_in_month = (current_year % 4 == 0 && (current_year % 100 != 0 || current_year % 400 == 0)) ? 29: 28;
    } else if (current_month == 3 || current_month == 5 || current_month == 8 || current_month == 10) {
      days_in_month = 30;
    } else {
      days_in_month = 31;
    }

    for (int day = 1; day <= days_in_month; day++) {
      cout << setw(3) << day;
      if ((day + start_day) % 7 == 0) cout << "\n";
    }
    cout << "\n\n";
  }

  /* NEW FUNCTION: Gets due date from user with calendar interface
       Returns: time_t representing due date (0 means no due date) */
  time_t getDueDateFromUser() {
    time_t now = time(nullptr);
    tm* today = localtime(&now);

    while (true) {
      clearScreen();
      displayCalendar(now);

      cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
      cout << "                  SET DUE DATE\n";
      cout << "    [1] Select date   [2] No due date   [3] Next month\n";
      cout << "    [4] Prev month    [5] Today\n";
      cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
      cout << "Choose option: ";

      int choice;
      cin >> choice;

      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max(), '\n');
        continue;
      }

      switch (choice) {
        case 1: {
          // Select date
          int day,
          month,
          year;
          cout << "Enter day (1-31): ";
          cin >> day;
          cout << "Enter month (1-12): ";
          cin >> month;
          cout << "Enter year (e.g., 2023): ";
          cin >> year;

          tm due_date = *today;
          due_date.tm_mday = day;
          due_date.tm_mon = month - 1;
          due_date.tm_year = year - 1900;
          mktime(&due_date);

          time_t due_time = mktime(&due_date);
          time_t today_time = mktime(today);

          if (due_time >= today_time) {
            return due_time;
          } else {
            cout << RED << "Due date cannot be in the past!\n" << RESET;
            continue;
          }
        }
        case 2: // No due date
        return 0;
        case 3: // Next month
        today->tm_mon++;
        mktime(today);
        now = mktime(today);
        break;
        case 4: // Previous month
        today->tm_mon--;
        mktime(today);
        now = mktime(today);
        break;
        case 5: // Today
        now = time(nullptr);
        today = localtime(&now);
        break;
        default:
        continue;
      }
    }
  }

  // Updated to show due dates
  void viewTasks(bool showPriorityHeaders = true) {
    auto& tasks = datBase.getToDoList();
    if (tasks.empty()) {
      cout << "\nNo tasks found!\n";
      return;
    }

    vector < size_t > taskIndices;
    vector < tuple < string,
    bool,
    int,
    time_t,
    time_t>> highPriority;
    vector < tuple < string,
    bool,
    int,
    time_t,
    time_t>> normalPriority;

    // Separate tasks by priority
    for (size_t i = 0; i < tasks.size(); ++i) {
      if (get < 2 > (tasks[i]) == 1) {
        highPriority.push_back(tasks[i]);
        taskIndices.push_back(i);
      } else {
        normalPriority.push_back(tasks[i]);
        taskIndices.push_back(i);
      }
    }

    cout << "\n" << BOLD << GREEN << "+-----------------------TASKS-----------------------+" << RESET << "\n";

    size_t displayNumber = 1;

    if (!highPriority.empty() && showPriorityHeaders) {
      cout << YELLOW << "                 >>> PINNED TASKS <<<\n" << RESET;
      displayNumber = displayTaskList(highPriority, displayNumber);
    }

    if (!normalPriority.empty() && showPriorityHeaders) {
      cout << "\n                     >>> TASKS <<<\n";
    }
    displayTaskList(normalPriority, displayNumber);
  }

  /* Updated function to display due dates
       Parameters:
       - tasks: vector of tasks to display
       - startNumber: starting number for display
       Returns: next display number to use */
  size_t displayTaskList(const vector < tuple < string, bool, int, time_t, time_t>>& tasks, size_t startNumber = 1) {
    size_t currentNumber = startNumber;
    time_t now = time(nullptr);

    for (const auto& [task, completed, priority, created, due_date]: tasks) {
      string color;
      if (completed) {
        color = STRIKE + GREEN;
      } else if (priority == 1) {
        color = YELLOW;
      } else {
        color = BLUE;
      }

      tm* ptm = localtime(&created);
      char created_buffer[32];
      strftime(created_buffer, 32, "%Y-%m-%d %H:%M", ptm);

      cout << currentNumber << ". " << color
      << (completed ? "[X] ": "[ ] ")
      << left << setw(35) << task << RESET
      << MAGENTA << "(" << created_buffer << ")";

      // Display due date if it exists
      if (due_date != 0) {
        tm* due_tm = localtime(&due_date);
        char due_buffer[32];
        strftime(due_buffer, 32, "%Y-%m-%d", due_tm);

        if (due_date < now && !completed) {
          cout << RED << " [Due: " << due_buffer << "]" << RESET;
        } else {
          cout << GREEN << " [Due: " << due_buffer << "]" << RESET;
        }
      }
      cout << "\n";
      currentNumber++;
    }
    return currentNumber;
  }

  // Updated addTask with due date support
  void addTask() {
    clearScreen();
    viewTasks();
    cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
    cout << "                      ADD TASK";
    cout << "\n               [000] Return to Menu";
    cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";

    cout << "Enter new task: ";
    cin.ignore();
    getline(cin, currentInput);

    while(true) {
      if (currentInput == "000") {
        clearScreen();
        break;
      }
      if (!currentInput.empty()) {
        int priority;
        cout << "\n__________________________________________________________\n";
        cout << "\nPin this task? ([1] Yes, [2] No): ";
        cin >> priority;

        if (priority == 1 || priority == 2) {
          time_t due_date = getDueDateFromUser();
          datBase.addTask(currentInput, priority, due_date);
          clearScreen();
          cout << GREEN << "Task added successfully!\n" << RESET;
          break;
        } else {
          cin.clear();
          cin.ignore(numeric_limits < streamsize > ::max(), '\n');
          clearScreen();
          viewTasks();
          cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
          cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
          cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";
          cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
          cout << "                      ADD TASK";
          cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";
          cout << "Enter new task: " << currentInput;
        }
      }
    }
  }

  /* NEW FUNCTION: Allows setting/changing due date for existing tasks */
  void setDueDate() {
    auto& tasks = datBase.getToDoList();
    if (tasks.empty()) {
      cout << "No tasks to modify!\n";
      return;
    }

    clearScreen();
    viewTasks();
    while(true) {
      cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
      cout << "                     SET DUE DATE";
      cout << "\n               [000] Return to Menu";
      cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";
      size_t displayNumber;
      cout << "Task number: ";
      cin >> displayNumber;

      if (displayNumber == 000) {
        clearScreen();
        break;
      }

      if (!cin.fail()) {
        // Rebuild the task order to map display numbers to actual indices
        vector < size_t > taskIndices;
        for (size_t i = 0; i < tasks.size(); ++i) {
          if (get < 2 > (tasks[i]) == 1) {
            taskIndices.push_back(i);
          }
        }
        for (size_t i = 0; i < tasks.size(); ++i) {
          if (get < 2 > (tasks[i]) != 1) {
            taskIndices.push_back(i);
          }
        }

        if (displayNumber > 0 && displayNumber <= taskIndices.size()) {
          size_t actualIndex = taskIndices[displayNumber - 1];
          time_t new_due_date = getDueDateFromUser();
          get < 4 > (tasks[actualIndex]) = new_due_date;
          datBase.updateDataBase();
          clearScreen();
          cout << BOLD << YELLOW << "Due date updated!\n" << RESET;
          break;
        } else {
          cin.clear();
          cin.ignore(numeric_limits < streamsize > ::max(), '\n');
          clearScreen();
          viewTasks();
          cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
          cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
          cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";
        }
      } else {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max(), '\n');
        clearScreen();
        viewTasks();
        cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
        cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
        cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";
      }
    }
  }

  void toggleTask() {
    auto& tasks = datBase.getToDoList();
    if (tasks.empty()) {
      cout << "No tasks to toggle!\n";
      return;
    }

    clearScreen();
    viewTasks();
    while(true) {
      // First display the tasks with their current numbering
      cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
      cout << "                      TOGGLE TASK";
      cout << "\n               [000] Return to Menu";
      cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";
      size_t displayNumber;
      cout << "Toggle task number: ";
      cin >> displayNumber;
      if (displayNumber == 000) {
        clearScreen();
        break;
      }
      if (!cin.fail()) {
        // Rebuild the task order to map display numbers to actual indices
        vector < size_t > taskIndices;
        for (size_t i = 0; i < tasks.size(); ++i) {
          if (get < 2 > (tasks[i]) == 1) {
            taskIndices.push_back(i);
          }
        }
        for (size_t i = 0; i < tasks.size(); ++i) {
          if (get < 2 > (tasks[i]) != 1) {
            taskIndices.push_back(i);
          }
        }

        if (displayNumber > 0 && displayNumber <= taskIndices.size()) {
          size_t actualIndex = taskIndices[displayNumber - 1];
          auto& task = tasks[actualIndex];
          get < 1 > (task) = !get < 1 > (task); // Toggle completion status
          datBase.updateDataBase();
          clearScreen();
          cout << BOLD << YELLOW << "Task toggled!\n" << RESET;
          break;
        }

        else {
          cin.clear();
          cin.ignore(numeric_limits < streamsize > ::max(), '\n');
          clearScreen();
          viewTasks();
          cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
          cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
          cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";

        }
      }
      if(cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max(), '\n');
        clearScreen();
        viewTasks();
        cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
        cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
        cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";
      }


    }


  }



  void deleteTask() {
    auto& tasks = datBase.getToDoList();
    if (tasks.empty()) {
      cout << "No tasks to delete!\n";
      return;
    }

    clearScreen();
    viewTasks();

    while (true) {
      cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
      cout << "                      DELETE TASK";
      cout << "\n               [000] Return to Menu";
      cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";

      size_t displayNumber;
      cout << "Delete task number: ";
      cin >> displayNumber;

      if (displayNumber == 000) {
        clearScreen();
        break;
      }

      clearScreen();
      viewTasks();
      if(!cin.fail()) {

        vector < size_t > taskIndices;
        for (size_t i = 0; i < tasks.size(); ++i) {
          if (get < 2 > (tasks[i]) == 1) {
            taskIndices.push_back(i);
          }
        }
        for (size_t i = 0; i < tasks.size(); ++i) {
          if (get < 2 > (tasks[i]) != 1) {
            taskIndices.push_back(i);
          }
        }

        if (displayNumber > 0 && displayNumber <= taskIndices.size()) {
          while (true) {
            cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
            cout << "                      DELETE TASK";
            cout << "\n                 [1] Yes        [2] No";
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";

            size_t confirmNumber;
            cout << "Confirm Delete? : ";
            cin >> confirmNumber;

            if (cin.fail() || confirmNumber != 1 && confirmNumber != 2) {
              cin.clear();
              cin.ignore(numeric_limits < streamsize > ::max(), '\n');
              clearScreen();
              viewTasks();
              cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
              cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
              cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";

              continue; // Ask for confirmation again
            }

            if (confirmNumber == 1) {
              vector < size_t > taskIndices;
              for (size_t i = 0; i < tasks.size(); ++i) {
                if (get < 2 > (tasks[i]) == 1) {
                  taskIndices.push_back(i);
                }
              }
              for (size_t i = 0; i < tasks.size(); ++i) {
                if (get < 2 > (tasks[i]) != 1) {
                  taskIndices.push_back(i);
                }
              }

              if (displayNumber > 0 && displayNumber <= taskIndices.size()) {
                size_t actualIndex = taskIndices[displayNumber - 1];
                tasks.erase(tasks.begin() + actualIndex);
                datBase.updateDataBase();
                clearScreen();
                cout << BOLD << RED << "Task deleted!\n" << RESET;
              } else {
                cout << BOLD << RED << "Invalid task number!\n" << RESET;
              }
              return; // Done, exit function

            } else if (confirmNumber == 2) {
              clearScreen();
              cout << "Deletion Cancelled\n";
              return; // Exit function without deleting

            } else {
              cout << "Please enter 1 (Yes) or 2 (No).\n";
              // Loop again for valid input
            }
          }
        }
      }
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max(), '\n');
        clearScreen();
        viewTasks();
        cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
        cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
        cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";
        continue; // Prompt again
      }
      else {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max(), '\n');
        clearScreen();
        viewTasks();
        cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
        cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
        cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";
        continue; // Prompt again
      }
    }
  }
  void setPriority() {
    auto& tasks = datBase.getToDoList();
    if (tasks.empty()) {
      cout << "No tasks to modify!\n";
      return;
    }

    clearScreen();
    viewTasks();
    while(true) {
      cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
      cout << "               	TOGGLE PIN/UNPIN TASK";
      cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";

      size_t displayNumber;
      cout << "Task number: ";
      cin >> displayNumber;

      if(!cin.fail()) {
        // Rebuild the task order to map display numbers to actual indices
        vector < size_t > taskIndices;
        for (size_t i = 0; i < tasks.size(); ++i) {
          if (get < 2 > (tasks[i]) == 1) {
            taskIndices.push_back(i);
          }
        }
        for (size_t i = 0; i < tasks.size(); ++i) {
          if (get < 2 > (tasks[i]) != 1) {
            taskIndices.push_back(i);
          }
        }

        if (displayNumber > 0 && displayNumber <= taskIndices.size()) {
          size_t actualIndex = taskIndices[displayNumber - 1];
          auto& task = tasks[actualIndex];
          get < 2 > (task) = (get < 2 > (task) == 1) ? 2: 1; // Toggle pin state: 1 => 2, 2 => 1
          datBase.updateDataBase();
          clearScreen();
          cout << BOLD << YELLOW << "Task pin state toggled!\n" << RESET;
          break;
        } else {
          cin.clear();
          cin.ignore(numeric_limits < streamsize > ::max(), '\n');
          clearScreen();
          viewTasks();
          cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
          cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
          cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";
        }

      }
      if(cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max(), '\n');
        clearScreen();
        viewTasks();
        cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
        cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
        cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";

      }

    }


  }

  void sortTasks() {
    int choice;

    clearScreen();
    viewTasks();
    do {
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
        clearScreen();
        break;
        default:
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max(), '\n');
        clearScreen();
        viewTasks();
        cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
        cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
        cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";
      }
    } while (choice != 3);
  }

  void mainPage() {
    string enterKey;
    cout << "+----------------------------------------------------+\n";
    cout <<BOLD << GREEN << R"(       _________ _______    ______   _______
       \__   __/(  ___  )  (  __  \ (  ___  )
   	  ) (   | (   ) |  | (  \  )| (   ) |
   	  | |   | |   | |  | |   ) || |   | |
   	  | |   | |   | |  | |   | || |   | |
   	  | |   | |   | |  | |   ) || |   | |
   	  | |   | (___) |  | (__/  )| (___) |
   	  )_(   (_______)  (______/ (_______)
  	   _       _________ _______ _________
  	  ( \      \__   __/(  ____ \\__   __/
   	  | (         ) (   | (    \/   ) (
   	  | |         | |   | (_____    | |
   	  | |         | |   (_____  )   | |
   	  | |         | |         ) |   | |
   	  | (____/\___) (___/\____) |   | |
   	  (_______/\_______/\_______)   )_(
    )" << RESET;
    cout << "______________________________________________________\n";
    cout << BOLD << YELLOW << "       Stay focused. Stay organized. Stay in control.\n" << RESET;
    cout << R"(
This to-do list app is more than just a task manager,
it's your daily partner in productivity. Whether it's
managing school projects, planning your day, or
tracking personal goals, this app helps you stay on
top of everything. Add tasks, set priorities, mark
them done, and make progress one step at a time.)";
    cout << "\n+----------------------------------------------------+\n";
    cout << "Enter any key to continue. ";
    cin >> enterKey;

  }

  // ... [Keep all other existing functions exactly the same (toggleTask, deleteTask, setPriority, sortTasks, mainPage)]
  public:
  // Updated run function with new menu option
  void run() {
    datBase.loadData();
    int choice = 0;
    mainPage();

    clearScreen();
    viewTasks();
    while (choice != 7) {
      // Changed from 6 to 7
      displayMainMenu();
      cin >> choice;

      switch (choice) {
        case 1: addTask(); viewTasks(); break;
        case 2: toggleTask(); viewTasks(); break;
        case 3: setPriority(); viewTasks(); break;
        case 4: deleteTask(); viewTasks(); break;
        case 5: sortTasks(); viewTasks(); break;
        case 6: setDueDate(); viewTasks(); break; // New due date option
        case 7: clearScreen();
        // ... [keep your existing exit code]
        break;
        default:
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max(), '\n');
        clearScreen();
        viewTasks();
        cout << BOLD << RED << "+---------------------------------------------------+" << RESET << "\n";
        cout << BOLD << RED << "                    INPUT ERROR!" << RESET;
        cout << BOLD << RED << "\n+---------------------------------------------------+" << RESET << "\n";
      }
    }
  }
};

int main() {
  ToDoApp app;
  app.run();
  return 0;
}