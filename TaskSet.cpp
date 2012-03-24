#include "TaskSet.h"
#include <iostream>
#include <cstdlib>
#include <limits>

QDataStream &operator<<(QDataStream &out, const TaskSet &set)
{
    QMap<quint32, Task *>::const_iterator it = set.constBegin();

    out << (quint32)set.size();
    while (set.constEnd() != it)
    {
        out << *it.value();
        ++it;
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, TaskSet &set)
{
    quint32 size = 0;

    in >> size;
    for (quint32 i = 0; i < size; ++i)
    {
        Task *task = new Task();

        if (!task)
        {
            continue;
        }

        in >> *task;
        set.addTask(task);
    }
    return in;
}

bool operator==(const TaskSet &first, const TaskSet &second)
{
    if (first.map_.size() != second.map_.size())
    {
        return false;
    }

    if (first.map_.size() == 0)
    {
        return true;
    }

    QMap<quint32, Task *>::const_iterator it1 = first.map_.constBegin();
    QMap<quint32, Task *>::const_iterator it2 = second.map_.constBegin();

    while (first.map_.constEnd() != it1)
    {
        if (it1.key() != it2.key())
        {
            return false;
        }

        Task *task1 = it1.value();
        Task *task2 = it2.value();

        if (!task1 || !task2 ||
            (*task1 != *task2))
        {
            return false;
        }

        ++it1;
        ++it2;
    }

    return true;
}

bool operator!=(const TaskSet &first, const TaskSet &second)
{
    if (first.map_.size() != second.map_.size())
    {
        return true;
    }

    if (first.map_.size() == 0)
    {
        return false;
    }

    QMap<quint32, Task *>::const_iterator it1 = first.map_.constBegin();
    QMap<quint32, Task *>::const_iterator it2 = second.map_.constBegin();

    while (first.map_.constEnd() != it1)
    {
        if (it1.key() != it2.key())
        {
            return true;
        }

        Task *task1 = it1.value();
        Task *task2 = it2.value();

        if (!task1 || !task2 ||
            (*task1 != *task2))
        {
            return true;
        }

        ++it1;
        ++it2;
    }

    return false;
}

TaskSet::TaskSet() :
    cntTaskId_(0)
{
}

TaskSet::TaskSet(const TaskSet &other) :
    cntTaskId_(0)
{
    QMap<quint32, Task *>::const_iterator it = other.constBegin();

    while (other.constEnd() != it)
    {
        addTask(new Task(*it.value()));
        ++it;
    }
}

TaskSet &TaskSet::operator=(const TaskSet &other)
{
    QMap<quint32, Task *>::const_iterator it = other.constBegin();

    clear();
    while (other.constEnd() != it)
    {
        addTask(new Task(*it.value()));
        ++it;
    }
    return *this;
}

TaskSet::~TaskSet()
{
    QMap<quint32, Task *>::const_iterator it = map_.constBegin();

    while (map_.constEnd() != it)
    {
        delete map_.take(it.key());
        ++it;
    }
}

quint32 TaskSet::addTask(Task *task)
{
    if (!task || (std::numeric_limits<quint32>::max() == cntTaskId_))
    {
        return 0;
    }

    map_.insert(++cntTaskId_, task);
    return cntTaskId_;
}

bool TaskSet::deleteTask(quint32 id)
{
    Task *task = map_.take(id);

    if (task)
    {
        delete task;
        return true;
    }

    return false;
}

void TaskSet::clear()
{
    QMap<quint32, Task *>::const_iterator it = map_.constBegin();

    while (map_.constEnd() != it)
    {
        delete map_.take(it.key());
        ++it;
    }
}

void TaskSet::print(OrderBy orderBy, OrderType orderType) const
{
    if (map_.size() == 0)
    {
        std::cout << "No task found.\n";
        return;
    }

    QString header = QString("%1  %2  %3  %4  %5\n").
            arg(QString("ID"), -4).
            arg(QString("DATE/TIME"), -19).
            arg(QString("TITLE"), -16).
            arg(QString("PRIORITY"), -8).
            arg(QString("SEVERITY"), -14);
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << header.toStdString();
    std::cout << "----------------------------------------------------------------------\n";

    switch (orderBy)
    {
    case DATE_TIME:
    {
        QMultiMap<QDateTime, quint32> ordered_map;
        QMap<quint32, Task *>::const_iterator it = map_.constBegin();
        QList<quint32> keys;

        while (map_.constEnd() != it)
        {
            ordered_map.insert(QDateTime(it.value()->getDate(), it.value()->getTime()), it.key());
            ++it;
        }
        keys = ordered_map.values();
        if (DESCENDING == orderType)
        {
            for (int i = keys.size()-1; i >= 0; --i)
            {
                QString task_string = QString("%1  %2\n").
                        arg(keys.at(i), -4).
                        arg(map_.value(keys.at(i))->toString());
                std::cout << task_string.toStdString();
            }
        }
        else
        {
            foreach (quint32 key, keys)
            {
                QString task_string = QString("%1  %2\n").
                        arg(key, -4).
                        arg(map_.value(key)->toString());
                std::cout << task_string.toStdString();
            }
        }
        break;
    }
    case PRIORITY:
    {
        QMultiMap<int, quint32> ordered_map;
        QMap<quint32, Task *>::const_iterator it = map_.constBegin();
        QList<quint32> keys;

        while (map_.constEnd() != it)
        {
            ordered_map.insert(it.value()->getPriority(), it.key());
            ++it;
        }
        keys = ordered_map.values();
        if (DESCENDING == orderType)
        {
            for (int i = keys.size()-1; i >= 0; --i)
            {
                QString task_string = QString("%1  %2\n").
                        arg(keys.at(i), -4).
                        arg(map_.value(keys.at(i))->toString());
                std::cout << task_string.toStdString();
            }
        }
        else
        {
            foreach (quint32 key, keys)
            {
                QString task_string = QString("%1  %2\n").
                        arg(key, -4).
                        arg(map_.value(key)->toString());
                std::cout << task_string.toStdString();
            }
        }
        break;
    }
    case SEVERITY:
    {
        QMultiMap<int, quint32> ordered_map;
        QMap<quint32, Task *>::const_iterator it = map_.constBegin();
        QList<quint32> keys;

        while (map_.constEnd() != it)
        {
            ordered_map.insert(it.value()->getSeverity(), it.key());
            ++it;
        }
        keys = ordered_map.values();
        if (DESCENDING == orderType)
        {
            for (int i = keys.size()-1; i >= 0; --i)
            {
                QString task_string = QString("%1  %2\n").
                        arg(keys.at(i), -4).
                        arg(map_.value(keys.at(i))->toString());
                std::cout << task_string.toStdString();
            }
        }
        else
        {
            foreach (quint32 key, keys)
            {
                QString task_string = QString("%1  %2\n").
                        arg(key, -4).
                        arg(map_.value(key)->toString());
                std::cout << task_string.toStdString();
            }
        }
        break;
    }
    case ID:
    default:
    {
        QMap<quint32, Task *>::const_iterator it;

        if (DESCENDING == orderType)
        {
            it = map_.constEnd();
            do
            {
                --it;
                QString task_string = QString("%1  %2\n").
                        arg(it.key(), -4).
                        arg(it.value()->toString());
                std::cout << task_string.toStdString();
            } while (it != map_.constBegin());
        }
        else
        {
            it = map_.constBegin();
            while (map_.constEnd() != it)
            {
                QString task_string = QString("%1  %2\n").
                        arg(it.key(), -4).
                        arg(it.value()->toString());
                std::cout << task_string.toStdString();
                ++it;
            }
        }
    }
    }
    std::cout << "----------------------------------------------------------------------\n";
}
