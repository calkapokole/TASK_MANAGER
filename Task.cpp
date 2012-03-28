#include "Task.h"
#include <QMap>

QDataStream &operator<<(QDataStream &out, const Task &task)
{
    out << task.dateTime_
        << task.title_ << task.description_
        << (quint32)task.priority_ << (quint32)task.severity_;
    return out;
}

QDataStream &operator>>(QDataStream &in, Task &task)
{
    quint32 priority = 0;
    quint32 severity = 0;

    in >> task.dateTime_ >> task.title_ >> task.description_
       >> priority >> severity;
    task.priority_ = (Task::TaskPriority)priority;
    task.severity_ = (Task::TaskSeverity)severity;
    return in;
}

bool operator==(const Task &first, const Task &second)
{
    return ((first.dateTime_ == second.dateTime_) &&
            (first.title_ == second.title_) &&
            (first.description_ == second.description_) &&
            (first.priority_ == second.priority_) &&
            (first.severity_ == second.severity_));
}

bool operator!=(const Task &first, const Task &second)
{
    return ((first.dateTime_ != second.dateTime_) ||
            (first.title_ != second.title_) ||
            (first.description_ != second.description_) ||
            (first.priority_ != second.priority_) ||
            (first.severity_ != second.severity_));
}

Task::Task() :
    dateTime_(QDateTime::currentDateTime().addDays(1)),
    title_(QString("New task")),
    description_(QString("New task")),
    priority_(LOW_PRIORITY),
    severity_(COSMETIC)
{
}

Task::Task(const QDate &date, const QTime &time,
           const QString &title, const QString &description,
           TaskPriority priority, TaskSeverity severity) :
    dateTime_(QDateTime(date, time)),
    title_(title.simplified()),
    description_(description.simplified()),
    priority_(priority),
    severity_(severity)
{
    if (!dateTime_.isValid() || (QDateTime::currentDateTime() > dateTime_))
    {
        dateTime_ = QDateTime::currentDateTime().addDays(1);
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

Task::Task(const QDateTime &dateTime, const QString &title, const QString &description,
           TaskPriority priority, TaskSeverity severity) :
    dateTime_(dateTime),
    title_(title.simplified()),
    description_(description.simplified()),
    priority_(priority),
    severity_(severity)
{
    if (!dateTime_.isValid() || (QDateTime::currentDateTime() > dateTime_))
    {
        dateTime_ = QDateTime::currentDateTime().addDays(1);
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
    return QString("%1  %2  %3  %4  %5").
            arg(dateTime_.date().toString("yyyy-MM-dd")).
            arg(dateTime_.time().toString("hh:mm:ss")).
            arg(title_.leftJustified(16, QLatin1Char(' '), true)).
            arg(priorityToString(priority_), -8).
            arg(severityToString(severity_), -14);
}

QString Task::details() const
{
    const int desc_field_width = 12;

    return QString("%1  %2\n").
            arg(QString("DATE:"), desc_field_width).
            arg(dateTime_.date().toString("yyyy-MM-dd")) %
            QString("%1  %2\n").
            arg(QString("TIME:"), desc_field_width).
            arg(dateTime_.time().toString("hh:mm:ss")) %
            QString("%1  %2\n").
            arg(QString("TITLE:"), desc_field_width).
            arg(title_) %
            QString("%1  %2\n").
            arg(QString("DESCRIPTION:"), desc_field_width).
            arg(description_) %
            QString("%1  %2\n").
            arg(QString("PRIORITY:"), desc_field_width).
            arg(priorityToString(priority_)) %
            QString("%1  %2").
            arg(QString("SEVERITY:"), desc_field_width).
            arg(severityToString(severity_));
}

bool Task::setDate(const QDate &date)
{
    if (date.isValid() && (QDateTime(date, dateTime_.time()) > QDateTime::currentDateTime()))
    {
        dateTime_ = QDateTime(date, dateTime_.time());
        return true;
    }

    return false;
}

bool Task::setTime(const QTime &time)
{
    if (time.isValid() && (QDateTime(dateTime_.date(), time) > QDateTime::currentDateTime()))
    {
        dateTime_ = QDateTime(dateTime_.date(), time);
        return true;
    }

    return false;
}

bool Task::setDateTime(const QDateTime &dateTime)
{
    if (dateTime.isValid() && (dateTime > QDateTime::currentDateTime()))
    {
        dateTime_ = dateTime;
        return true;
    }

    return false;
}

bool Task::setTitle(const QString &title)
{
    if (title.simplified().isEmpty())
    {
#ifndef USE_EXCEPTIONS
        return false;
#else
        EmptyStringException e;
        e.raise();
#endif // USE_EXCEPTIONS
    }

    title_ = title.simplified();
    return true;
}

bool Task::setDescription(const QString &description)
{
    if (description.simplified().isEmpty())
    {
#ifndef USE_EXCEPTIONS
        return false;
#else
        EmptyStringException e;
        e.raise();
#endif // USE_EXCEPTIONS
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
