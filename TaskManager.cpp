#include "TaskManager.h"
#include <QDate>
#include <QFile>
#include <QMap>
#include <QDataStream>
#include <iostream>
#include <cstdio>
#include <limits>

bool operator==(const TaskManager &first, const TaskManager &second)
{
    return ((first.set_ == second.set_) &&
            (first.usersFileName_ == second.usersFileName_) &&
            (first.tasksFileNameSuffix_ == second.tasksFileNameSuffix_) &&
            (first.loggedInUsername_ == second.loggedInUsername_));
}

bool operator!=(const TaskManager &first, const TaskManager &second)
{
    return ((first.set_ != second.set_) ||
            (first.usersFileName_ != second.usersFileName_) ||
            (first.tasksFileNameSuffix_ != second.tasksFileNameSuffix_) ||
            (first.loggedInUsername_ != second.loggedInUsername_));
}

void TaskManager::run()
{
    int action;

    std::cout << "Task Manager by Pawel Pieta\n";
    while (!login());
    std::cout << "Welcome " << loggedInUsername_.toStdString() << "!\n";
    loadTasks();
    do
    {
        std::cout << "[1] PRINT TASKS; [2] ADD TASK; [3] DELETE TASK; "
                     "[4] EDIT TASK; [5] TASK DETAILS; [6] EXIT\n";
        shellPrompt();
        std::cout << "Choose action: ";
        action = userInput().toInt();
        switch (action)
        {
        case 1: printTasks(); break;
        case 2: addTask(); break;
        case 3: deleteTask(); break;
        case 4: editTask(); break;
        case 5: taskDetails(); break;
        case 6: break;
        default: std::cout << "Error! Action unrecognized. Please try again.\n";
        }
    } while (action != 6);
    saveTasks();
    std::cout << "Thank you for using Task Manager.\n";
}

TaskManager &TaskManager::getInstance()
{
    static TaskManager instance;

    return instance;
}

TaskManager::TaskManager() :
    usersFileName_("users"),
    tasksFileNameSuffix_("_tasks")
{
}

QString TaskManager::userInput() const
{
    std::string buffer;

    std::getline(std::cin, buffer, '\n');
    return QString(buffer.c_str());
}

QString TaskManager::enterUserName() const
{
    shellPrompt();
    std::cout << "Enter user name: ";
    return userInput();
}

QString TaskManager::enterPassword() const
{
    shellPrompt();
    std::cout << "Enter password: ";
    return userInput();
}

bool TaskManager::login()
{
    QMap<QString, QString> user_pass;
    QFile file(usersFileName_);
    QDataStream stream(&file);
    QString user_name;
    QString pass;

    if (!file.exists())
    {
        if (!file.open(QIODevice::WriteOnly))
        {
            std::cout << "Error! Cannot write to disk. Configuration file cannot be accessed.\n";
            return false;
        }

        std::cout << "No user defined yet.\n";
        user_name = enterUserName();
        pass = enterPassword();
        user_pass.insert(user_name, pass);

        stream << user_pass;
        file.flush();
        file.close();
        loggedInUsername_ = user_name;
        return true;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        std::cout << "Error! Cannot read from disk. Configuration file cannot be accessed.\n";
        return false;
    }

    stream >> user_pass;
    file.close();
    if (user_pass.isEmpty())
    {
        std::cout << "Error! Configuration file is corrupted and will be deleted.\n";
        file.remove();
        return false;
    }

    bool logged_in = false;
    int action;

    do
    {
        std::cout << "[1] LOGIN; [2] CREATE NEW USER\n";
        shellPrompt();
        std::cout << "Choose action: ";
        action = userInput().toInt();

        switch (action)
        {
        case 1:
        {
            user_name = enterUserName();
            pass = enterPassword();
            if (!user_pass.contains(user_name) || (user_pass.value(user_name) != pass))
            {
                std::cout << "Error! User name or password does not match.\n";
                break;
            }
            logged_in = true;
            break;
        }
        case 2:
        {
            user_name = enterUserName();
            if (user_pass.contains(user_name))
            {
                std::cout << "Error! User name already exists.\n";
                break;
            }
            pass = enterPassword();

            if (!file.open(QIODevice::WriteOnly))
            {
                std::cout << "Error! Cannot write to disk. Configuration file cannot be accessed.\n";
                return false;
            }

            user_pass.insert(user_name, pass);
            stream << user_pass;
            file.flush();
            file.close();
            logged_in = true;
            break;
        }
        default: std::cout << "Error! Action unrecognized. Please try again.\n";
        }
    } while (!logged_in);

    loggedInUsername_ = user_name;
    return true;
}

void TaskManager::loadTasks()
{
    QFile file(loggedInUsername_ % tasksFileNameSuffix_);

    if (!file.exists())
    {
        std::cout << "You have no defined tasks.\n";
        return;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        std::cout << "Error! Cannot read from disk. User tasks were not loaded.\n";
        return;
    }

    QDataStream in(&file);

    in >> set_;
    file.close();
    std::cout << set_.size() << " task(s) loaded.\n";
}

void TaskManager::saveTasks() const
{
    QFile file(loggedInUsername_ % tasksFileNameSuffix_);

    if (set_.size() == 0)
    {
        file.remove();
        std::cout << "Task list empty. Nothing to write.\n";
        return;
    }

    if (!file.open(QIODevice::WriteOnly))
    {
        std::cout << "Error! Cannot write to disk. User tasks were not saved.\n";
        return;
    }

    QDataStream out(&file);

    out << set_;
    file.flush();
    file.close();
    std::cout << set_.size() << " task(s) saved.\n";
}

void TaskManager::printTasks() const
{
    int order_by;
    int order_type;

    std::cout << "[0] ID; [1] DATE/TIME; [2] TITLE; [3] PRIORITY; [4] SEVERITY\n";
    shellPrompt();
    std::cout << "Order by: [0] ";
    order_by = userInput().toInt();
    std::cout << "[0] ASCENDING; [1] DESCENDING\n";
    shellPrompt();
    std::cout << "Order type: [0] ";
    order_type = userInput().toInt();
    set_.print((TaskSet::OrderBy)order_by, (TaskSet::OrderType)order_type);
}

quint32 TaskManager::enterTaskId() const
{
    shellPrompt();
    std::cout << "Enter task ID: ";
    return (quint32)userInput().toUInt();
}

QDate TaskManager::enterTaskDate() const
{
    shellPrompt();
    std::cout << "Enter date (yyyy-mm-dd): ";
    return QDate::fromString(userInput(), "yyyy-MM-dd");
}

QTime TaskManager::enterTaskTime() const
{
    shellPrompt();
    std::cout << "Enter time (hh:mm:ss): ";
    return QTime::fromString(userInput(), "hh:mm:ss");
}

QString TaskManager::enterTaskTitle() const
{
    shellPrompt();
    std::cout << "Enter title: ";
    return userInput();
}

QString TaskManager::enterTaskDescription() const
{
    shellPrompt();
    std::cout << "Enter description: ";
    return userInput();
}

Task::TaskPriority TaskManager::enterTaskPriority() const
{
    std::cout << "[1] LOW; [2] MEDIUM; [3] HIGH\n";
    shellPrompt();
    std::cout << "Enter priority: ";
    return (Task::TaskPriority)userInput().toInt();
}

Task::TaskSeverity TaskManager::enterTaskSeverity() const
{
    std::cout << "[1] COSMETIC; [2] WORKAROUND; [3] BLOCKER; [4] SYSTEM FAILURE\n";
    shellPrompt();
    std::cout << "Enter severity: ";
    return (Task::TaskSeverity)userInput().toInt();
}

bool TaskManager::addTask()
{
    return set_.addTask(new Task(enterTaskDate(),
                                 enterTaskTime(),
                                 enterTaskTitle(),
                                 enterTaskDescription(),
                                 enterTaskPriority(),
                                 enterTaskSeverity()));
}

bool TaskManager::deleteTask()
{
    quint32 id = enterTaskId();

    if (set_.deleteTask(id))
    {
        std::cout << "Task ID " << id << " deleted.\n";
        return true;
    }

    std::cout << "Error! No task with ID " << id << ".\n";
    return false;
}

bool TaskManager::editTask()
{
    quint32 id = enterTaskId();
    Task *task = set_.getTask(id);

    if (!task)
    {
        std::cout << "Error! No task with ID " << id << ".\n";
        return false;
    }

    int action;

    do
    {
        std::cout << "[1] DATE; [2] TIME; [3] TITLE; [4] DESCRIPTION; " <<
                     "[5] PRIORITY; [6] SEVERITY; [7] RETURN TO MAIN MENU\n";
        shellPrompt();
        std::cout << "What would you like to edit? ";
        action = userInput().toInt();
#ifdef USE_EXCEPTIONS
        switch (action)
        {
        case 1:
        {
            try
            {
                task->setDate(enterTaskDate());
                std::cout << "Date successfully changed.\n";
            }
            catch (InvalidDataTypeException e)
            {
                std::cout << "Error! Date was not changed. Wrong input.\n";
            }
            break;
        }
        case 2:
        {
            try
            {
                task->setTime(enterTaskTime());
                std::cout << "Time successfully changed.\n";
            }
            catch (InvalidDataTypeException e)
            {
                std::cout << "Error! Time was not changed. Wrong input.\n";
            }
            break;
        }
        case 3:
        {
            try
            {
                task->setTitle(enterTaskTitle());
                std::cout << "Title successfully changed.\n";
            }
            catch (EmptyStringException e)
            {
                std::cout << "Error! Title was not changed. Wrong input.\n";
            }
            break;
        }
        case 4:
        {
            try
            {
                task->setDescription(enterTaskDescription());
                std::cout << "Description successfully changed.\n";
            }
            catch (EmptyStringException e)
            {
                std::cout << "Error! Description was not changed. Wrong input.\n";
            }
            break;
        }
        case 5:
        {
            try
            {
                task->setPriority(enterTaskPriority());
                std::cout << "Priority successfully changed.\n";
            }
            catch (InvalidEnumException e)
            {
                std::cout << "Error! Priority was not changed. Wrong input.\n";
            }
            break;
        }
        case 6:
        {
            try
            {
                task->setSeverity(enterTaskSeverity());
                std::cout << "Severity successfully changed.\n";
            }
            catch (InvalidEnumException e)
            {
                std::cout << "Error! Severity was not changed. Wrong input.\n";
            }
            break;
        }
        case 7: break;
        default: std::cout << "Error! Action unrecognized. Please try again.\n";
        }
#else
        switch (action)
        {
        case 1:
        {
            std::cout << ((task->setDate(enterTaskDate())) ?
                              "Date successfully changed.\n" :
                              "Error! Date was not changed. Wrong input.\n");
            break;
        }
        case 2:
        {
            std::cout << ((task->setTime(enterTaskTime())) ?
                              "Time successfully changed.\n" :
                              "Error! Time was not changed. Wrong input.\n");
            break;
        }
        case 3:
        {
            std::cout << ((task->setTitle(enterTaskTitle())) ?
                              "Title successfully changed.\n" :
                              "Error! Title was not changed. Wrong input.\n");
            break;
        }
        case 4:
        {
            std::cout << ((task->setDescription(enterTaskDescription())) ?
                              "Description successfully changed.\n" :
                              "Error! Description was not changed. Wrong input.\n");
            break;
        }
        case 5:
        {
            std::cout << ((task->setPriority(enterTaskPriority())) ?
                              "Priority successfully changed.\n" :
                              "Error! Priority was not changed. Wrong input.\n");
            break;
        }
        case 6:
        {
            std::cout << ((task->setSeverity(enterTaskSeverity())) ?
                              "Severity successfully changed.\n" :
                              "Error! Severity was not changed. Wrong input.\n");
            break;
        }
        case 7: break;
        default: std::cout << "Error! Action unrecognized. Please try again.\n";
        }
#endif // USE_EXCEPTIONS
    } while (action != 7);
    return true;
}

bool TaskManager::taskDetails() const
{
    quint32 id = enterTaskId();
    Task *task = set_.getTask(id);

    if (task)
    {
        std::cout << "----------------------------------------------------------------------\n";
        std::cout << task->details().toStdString() << "\n";
        std::cout << "----------------------------------------------------------------------\n";
        return true;
    }

    std::cout << "Error! No task with ID " << id << ".\n";
    return false;
}

void TaskManager::shellPrompt() const
{
    std::cout << "$ ";
}

//void TaskManager::flushCin(int lastBufferSize) const
//{
//    if ((0 == lastBufferSize) ||
//        (std::cin.gcount() == lastBufferSize-1))
//    {
//        std::cin.clear();
//        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//    }
//}
