#ifndef TASK_H
#define TASK_H

#define USE_EXCEPTIONS

#include <QDateTime>
#include <QDataStream>
#include <QMetaType>

#ifdef USE_EXCEPTIONS
#include "EmptyStringException.h"
#endif // USE_EXCEPTIONS

class Task
{
public:
    enum TaskPriority
    {
        UNKNOWN_PRIORITY,
        LOW_PRIORITY,
        MEDIUM_PRIORITY,
        HIGH_PRIORITY
    };
    enum TaskSeverity
    {
        UNKNOWN_SEVERITY,
        COSMETIC,
        WORKAROUND,
        BLOCKER,
        SYSTEM_FAILURE
    };

    explicit Task();
    explicit Task(const QDate &date, const QTime &time, const QString &title,
                  const QString &description, TaskPriority priority, TaskSeverity severity);
    explicit Task(const QDateTime &dateTime, const QString &title, const QString &description,
                  TaskPriority priority, TaskSeverity severity);
    QString toString() const;
    QString details() const;

    inline QDate getDate() const { return dateTime_.date(); }
    inline QTime getTime() const { return dateTime_.time(); }
    inline QDateTime getDateTime() const { return dateTime_; }
    inline QString getTitle() const { return title_; }
    inline QString getDescription() const { return description_; }
    inline TaskPriority getPriority() const { return priority_; }
    inline TaskSeverity getSeverity() const { return severity_; }

    bool setDate(const QDate &date);
    bool setTime(const QTime &time);
    bool setDateTime(const QDateTime &dateTime);
    bool setTitle(const QString &title);
    bool setDescription(const QString &description);
    bool setPriority(TaskPriority priority);
    bool setSeverity(TaskSeverity severity);

    static QString priorityToString(TaskPriority priority);
    static QString severityToString(TaskSeverity severity);

    friend QDataStream &operator<<(QDataStream &out, const Task &task);
    friend QDataStream &operator>>(QDataStream &in, Task &task);
    friend bool operator==(const Task &first, const Task &second);
    friend bool operator!=(const Task &first, const Task &second);

private:
    QDateTime dateTime_;
    QString title_;
    QString description_;
    TaskPriority priority_;
    TaskSeverity severity_;

};

Q_DECLARE_METATYPE(Task)

#endif // TASK_H
