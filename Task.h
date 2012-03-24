#ifndef TASK_H
#define TASK_H

#include <QDate>
#include <QDataStream>
#include <QMetaType>

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
    QString toString() const;
    QString details() const;

    inline QDate getDate() const { return date_; }
    inline QTime getTime() const { return time_; }
    inline QString getTitle() const { return title_; }
    inline QString getDescription() const { return description_; }
    inline TaskPriority getPriority() const { return priority_; }
    inline TaskSeverity getSeverity() const { return severity_; }

    bool setDate(QDate date);
    bool setTime(QTime time);
    bool setTitle(const QString &title);
    bool setDescription(const QString &description);
    bool setPriority(TaskPriority priority);
    bool setSeverity(TaskSeverity severity);

    static QString priorityToString(TaskPriority priority);
    static QString severityToString(TaskSeverity severity);

    friend QDataStream &operator<<(QDataStream &out, const Task &task);
    friend bool operator==(const Task &first, const Task &second);
    friend bool operator!=(const Task &first, const Task &second);

private:
    QDate date_;
    QTime time_;
    QString title_;
    QString description_;
    TaskPriority priority_;
    TaskSeverity severity_;

};

Q_DECLARE_METATYPE(Task)
QDataStream &operator>>(QDataStream &in, Task &task);

#endif // TASK_H
