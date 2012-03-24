#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "TaskSet.h"

class TaskManager
{
public:
    void run();

    static TaskManager &getInstance();

    friend bool operator==(const TaskManager &first, const TaskManager &second);
    friend bool operator!=(const TaskManager &first, const TaskManager &second);

private:
    TaskSet set_;
    QString usersFileName_;
    QString tasksFileNameSuffix_;
    QString loggedInUsername_;
    int bufferSize_;

    TaskManager();
    TaskManager(const TaskManager &other);
    TaskManager &operator=(const TaskManager &other);
    QString enterUserName() const;
    QString enterPassword() const;
    bool login();
    void loadTasks();
    void saveTasks() const;
    void printTasks() const;
    quint32 enterTaskId() const;
    QDate enterTaskDate() const;
    QTime enterTaskTime() const;
    QString enterTaskTitle() const;
    QString enterTaskDescription() const;
    Task::TaskPriority enterTaskPriority() const;
    Task::TaskSeverity enterTaskSeverity() const;
    bool addTask();
    bool deleteTask();
    bool editTask();
    bool taskDetails() const;
    void shellPrompt() const;
    void flushCin(int lastBufferSize) const;

};

#endif // TASKMANAGER_H
