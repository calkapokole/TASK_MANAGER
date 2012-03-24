#ifndef TASKSET_H
#define TASKSET_H

#include "Task.h"
#include <QMap>

class TaskSet
{
public:
    enum OrderBy
    {
        ID,
        DATE_TIME,
        PRIORITY,
        SEVERITY
    };
    enum OrderType
    {
        ASCENDING,
        DESCENDING
    };

    explicit TaskSet();
    TaskSet(const TaskSet &other);
    TaskSet &operator=(const TaskSet &other);
    ~TaskSet();
    inline int size() const { return map_.size(); }
    inline QMap<quint32, Task *>::const_iterator constBegin() const { return map_.constBegin(); }
    inline QMap<quint32, Task *>::const_iterator constEnd() const { return map_.constEnd(); }
    quint32 addTask(Task *task);
    inline Task *getTask(quint32 id) const { return map_.value(id); }
    bool deleteTask(quint32 id);
    void clear();
    void print(OrderBy orderBy = ID, OrderType orderType = ASCENDING) const;

    friend bool operator==(const TaskSet &first, const TaskSet &second);
    friend bool operator!=(const TaskSet &first, const TaskSet &second);

private:
    QMap<quint32, Task *> map_;
    quint32 cntTaskId_;

};

Q_DECLARE_METATYPE(TaskSet)
QDataStream &operator<<(QDataStream &out, const TaskSet &set);
QDataStream &operator>>(QDataStream &in, TaskSet &set);

#endif // TASKSET_H
