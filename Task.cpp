#include "Task.h"
#include <QMap>
#include <QMetaType>

QDataStream &operator<<(QDataStream &out, const Task &task)
{
    out << task.date_ << task.time_ <<
           task.title_ << task.description_ <<
           (quint32)task.priority_ << (quint32)task.severity_;
    return out;
}

QDataStream &operator>>(QDataStream &in, Task &task)
{
    QDate date;
    QTime time;
    QString title;
    QString description;
    quint32 priority = 0;
    quint32 severity = 0;

    in >> date;
    in >> time;
    in >> title;
    in >> description;
    in >> priority;
    in >> severity;
    task.setDate(date);
    task.setTime(time);
    task.setTitle(title);
    task.setDescription(description);
    task.setPriority((Task::TaskPriority)priority);
    task.setSeverity((Task::TaskSeverity)severity);
    return in;
}

bool operator==(const Task &first, const Task &second)
{
    return ((first.date_ == second.date_) &&
            (first.time_ == second.time_) &&
            (first.title_ == second.title_) &&
            (first.description_ == second.description_) &&
            (first.priority_ == second.priority_) &&
            (first.severity_ == second.severity_));
}

bool operator!=(const Task &first, const Task &second)
{
    return ((first.date_ != second.date_) ||
            (first.time_ != second.time_) ||
            (first.title_ != second.title_) ||
            (first.description_ != second.description_) ||
            (first.priority_ != second.priority_) ||
            (first.severity_ != second.severity_));
}

Task::Task() :
    date_(QDate::currentDate().addDays(1)),
    time_(QTime::currentTime()),
    title_(QString("New task")),
    description_(QString("New task")),
    priority_(LOW_PRIORITY),
    severity_(COSMETIC)
{
}

Task::Task(const QDate &date, const QTime &time,
           const QString &title, const QString &description,
           Task::TaskPriority priority, Task::TaskSeverity severity) :
    date_(date),
    time_(time),
    title_(title.simplified()),
    description_(description.simplified()),
    priority_(priority),
    severity_(severity)
{
    if (!date_.isValid() || (QDate::currentDate() > date_))
    {
        date_ = QDate::currentDate().addDays(1);
    }
    if (!time_.isValid())
    {
        time_ = QTime::currentTime();
    }
    if ((QDate::currentDate() == date_) && (QTime::currentTime() >= time_))
    {
        time_ = QTime::currentTime().addSecs(3600);
    }
    if (title_.isEmpty())
    {
        title_ = QString("New task");
    }
    if (description_.isEmpty())
    {
        description_ = QString("New task");
    }
    if ((priority_ < LOW_PRIORITY) || (priority_ > HIGH_PRIORITY))
    {
        priority_ = LOW_PRIORITY;
    }
    if ((severity_ < COSMETIC) || (severity_ > SYSTEM_FAILURE))
    {
        severity_ = COSMETIC;
    }
}

QString Task::toString() const
{
    return QString("%1/%2  %3  %4  %5").
            arg(date_.toString("yyyy-MM-dd")).
            arg(time_.toString("hh:mm:ss")).
            arg(title_.leftJustified(16, ' ', true)).
            arg(priorityToString(priority_), -8).
            arg(severityToString(severity_), -14);
}

QString Task::details() const
{
    return QString("%1%2/%3\n").
            arg(QString("DATE/TIME:"), -20).
            arg(date_.toString("yyyy-MM-dd")).
            arg(time_.toString("hh:mm:ss")) %
            QString("%1%2\n").
            arg(QString("TITLE:"), -20).
            arg(title_) %
            QString("%1%2\n").
            arg(QString("DESCRIPTION:"), -20).
            arg(description_) %
            QString("%1%2\n").
            arg(QString("PRIORITY:"), -20).
            arg(priorityToString(priority_)) %
            QString("%1%2").
            arg(QString("SEVERITY:"), -20).
            arg(severityToString(severity_));
}

bool Task::setDate(QDate date)
{
    if (date.isValid() && (date >= QDate::currentDate()))
    {
        date_ = date;
        return true;
    }

    return false;
}

bool Task::setTime(QTime time)
{
    if (!time.isValid() ||
        ((QDate::currentDate() == date_) && (QTime::currentTime() >= time)))
    {
        return false;
    }

    time_ = time;
    return true;
}

bool Task::setTitle(const QString &title)
{
    if (title.simplified().isEmpty())
    {
        return false;
    }

    title_ = title.simplified();
    return true;
}

bool Task::setDescription(const QString &description)
{
    if (description.simplified().isEmpty())
    {
        return false;
    }

    description_ = description.simplified();
    return true;
}

bool Task::setPriority(TaskPriority priority)
{
    if ((priority >= LOW_PRIORITY) && (priority <= HIGH_PRIORITY))
    {
        priority_ = priority;
        return true;
    }

    return false;
}

bool Task::setSeverity(TaskSeverity severity)
{
    if ((severity >= COSMETIC) && (severity <= SYSTEM_FAILURE))
    {
        severity_ = severity;
        return true;
    }

    return false;
}

QString Task::priorityToString(TaskPriority priority)
{
    static QMap<int, QString> map;
    static bool first_time = true;

    if (first_time)
    {
        map.insert(UNKNOWN_PRIORITY, QString("UNKNOWN"));
        map.insert(LOW_PRIORITY, QString("LOW"));
        map.insert(MEDIUM_PRIORITY, QString("MEDIUM"));
        map.insert(HIGH_PRIORITY, QString("HIGH"));
        first_time = false;
    }
    return map.value(priority, QString("UNKNOWN"));
}

QString Task::severityToString(TaskSeverity severity)
{
    static QMap<int, QString> map;
    static bool first_time = true;

    if (first_time)
    {
        map.insert(UNKNOWN_SEVERITY, QString("UNKNOWN"));
        map.insert(COSMETIC, QString("COSMETIC"));
        map.insert(WORKAROUND, QString("WORKAROUND"));
        map.insert(BLOCKER, QString("BLOCKER"));
        map.insert(SYSTEM_FAILURE, QString("SYSTEM FAILURE"));
        first_time = false;
    }
    return map.value(severity, QString("UNKNOWN"));
}
