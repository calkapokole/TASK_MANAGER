#include <QtTest/QtTest>
#include "../TaskManager.h"

class TaskManagerTest : public QObject
{
    Q_OBJECT

public:
    TaskManagerTest();

private Q_SLOTS:
    void testTaskValidation_data();
    void testTaskValidation();
    void testTaskDate();
    void testTaskTime();
    void testTaskTitle();
    void testTaskDescription();
    void testTaskPriority();
    void testTaskSeverity();
    void testTaskEqual_data();
    void testTaskEqual();

    void testTaskSetAddTask();
    void testTaskSetGetTask();
    void testTaskSetDeleteTask();
    void testTaskSetSize();
    void testTaskSetEqual();

    void testTaskManagerSingleton();

};

TaskManagerTest::TaskManagerTest()
{
}

void TaskManagerTest::testTaskValidation_data()
{
    QTest::addColumn<Task>("task");

    QTest::newRow("default constructor") << Task();
    QTest::newRow("invalid, empty, below") << Task(QDate(), QTime(), QString(), QString(),
                                                   (Task::TaskPriority)((int)(Task::UNKNOWN_PRIORITY)-1),
                                                   (Task::TaskSeverity)((int)(Task::UNKNOWN_SEVERITY)-1));
    QTest::newRow("invalid, empty, above") << Task(QDate(), QTime(), QString(), QString(),
                                                   (Task::TaskPriority)((int)(Task::HIGH_PRIORITY)+1),
                                                   (Task::TaskSeverity)((int)(Task::SYSTEM_FAILURE)+1));
    QTest::newRow("invalid, empty, unknown") << Task(QDate(), QTime(), QString(), QString(),
                                                     Task::UNKNOWN_PRIORITY, Task::UNKNOWN_SEVERITY);
    QTest::newRow("valid, not empty, low") << Task(QDate(2000, 1, 1), QTime(0, 0, 0),
                                                   QString("Test task"), QString("Description of test task"),
                                                   Task::LOW_PRIORITY, Task::COSMETIC);
    QTest::newRow("valid, not empty, high") << Task(QDate(2000, 1, 1), QTime(0, 0, 0),
                                                    QString("Test task"), QString("Description of test task"),
                                                    Task::HIGH_PRIORITY, Task::SYSTEM_FAILURE);
}

void TaskManagerTest::testTaskValidation()
{
    QFETCH(Task, task);
    QVERIFY2(task.getDate().isValid() == true, "Invalid date.");
    QVERIFY2(task.getTime().isValid() == true, "Invalid time.");
    QVERIFY2(task.getTitle().isEmpty() == false, "Empty task title.");
    QVERIFY2(task.getDescription().isEmpty() == false, "Empty task description.");
    QVERIFY2(task.getPriority() >= Task::LOW_PRIORITY, "Invalid task priority.");
    QVERIFY2(task.getPriority() <= Task::HIGH_PRIORITY, "Invalid task priority.");
    QVERIFY2(task.getSeverity() >= Task::COSMETIC, "Invalid task severity.");
    QVERIFY2(task.getSeverity() <= Task::SYSTEM_FAILURE, "Invalid task severity.");
}

void TaskManagerTest::testTaskDate()
{
    Task task;

    QCOMPARE(task.setDate(QDate()), false);
    QVERIFY2(task.getDate() != QDate(), "Invalid date accepted.");
    QCOMPARE(task.setDate(QDate(2016, 2, 29)), true);
    QCOMPARE(task.getDate(), QDate(2016, 2, 29));
    QCOMPARE(task.setDate(QDate(1000, 1, 1)), false);
    QVERIFY2(task.getDate() != QDate(1000, 1, 1), "Older than current date accepted.");
}

void TaskManagerTest::testTaskTime()
{
    Task task;
    QTime time;

    QCOMPARE(task.setTime(QTime()), false);
    QVERIFY2(task.getTime() != QTime(), "Invalid time accepted.");
    time = QTime::currentTime().addSecs(3600);
    QCOMPARE(task.setTime(time), true);
    QCOMPARE(task.getTime(), time);
}

void TaskManagerTest::testTaskTitle()
{
    Task task;

#ifndef USE_EXCEPTIONS
    QCOMPARE(task.setTitle(QString()), false);
#else
    try
    {
        QCOMPARE(task.setTitle(QString()), false);
    }
    catch (EmptyStringException e)
    {
        qDebug() << e.what();
    }
#endif // USE_EXCEPTIONS
    QVERIFY2(task.getTitle() != QString(), "Empty title accepted.");
#ifndef USE_EXCEPTIONS
    QCOMPARE(task.setTitle(QString("Test task")), true);
#else
    try
    {
        QCOMPARE(task.setTitle(QString("Test task")), true);
    }
    catch (EmptyStringException e)
    {
        qDebug() << e.what();
    }
#endif // USE_EXCEPTIONS
    QCOMPARE(task.getTitle(), QString("Test task"));
}

void TaskManagerTest::testTaskDescription()
{
    Task task;

#ifndef USE_EXCEPTIONS
    QCOMPARE(task.setDescription(QString()), false);
#else
    try
    {
        QCOMPARE(task.setDescription(QString()), false);
    }
    catch (EmptyStringException e)
    {
        qDebug() << e.what();
    }
#endif // USE_EXCEPTIONS
    QVERIFY2(task.getDescription() != QString(), "Empty description accepted.");
#ifndef USE_EXCEPTIONS
    QCOMPARE(task.setDescription(QString("Description of test task")), true);
#else
    try
    {
        QCOMPARE(task.setDescription(QString("Description of test task")), true);
    }
    catch (EmptyStringException e)
    {
        qDebug() << e.what();
    }
#endif // USE_EXCEPTIONS
    QCOMPARE(task.getDescription(), QString("Description of test task"));
}

void TaskManagerTest::testTaskPriority()
{
    Task task;

    QCOMPARE(task.setPriority(Task::UNKNOWN_PRIORITY), false);
    QVERIFY2(task.getPriority() != Task::UNKNOWN_PRIORITY, "UNKNOWN_PRIORITY accepted.");
    QCOMPARE(task.setPriority(Task::LOW_PRIORITY), true);
    QCOMPARE(task.getPriority(), Task::LOW_PRIORITY);
    QCOMPARE(task.setPriority(Task::HIGH_PRIORITY), true);
    QCOMPARE(task.getPriority(), Task::HIGH_PRIORITY);
    QCOMPARE(task.setPriority((Task::TaskPriority)((int)(Task::UNKNOWN_PRIORITY)-1)), false);
    QCOMPARE(task.setPriority((Task::TaskPriority)((int)(Task::HIGH_PRIORITY)+1)), false);
}

void TaskManagerTest::testTaskSeverity()
{
    Task task;

    QCOMPARE(task.setSeverity(Task::UNKNOWN_SEVERITY), false);
    QVERIFY2(task.getSeverity() != Task::UNKNOWN_SEVERITY, "UNKNOWN_SEVERITY accepted.");
    QCOMPARE(task.setSeverity(Task::COSMETIC), true);
    QCOMPARE(task.getSeverity(), Task::COSMETIC);
    QCOMPARE(task.setSeverity(Task::SYSTEM_FAILURE), true);
    QCOMPARE(task.getSeverity(), Task::SYSTEM_FAILURE);
    QCOMPARE(task.setSeverity((Task::TaskSeverity)((int)(Task::UNKNOWN_SEVERITY)-1)), false);
    QCOMPARE(task.setSeverity((Task::TaskSeverity)((int)(Task::SYSTEM_FAILURE)+1)), false);
}

void TaskManagerTest::testTaskEqual_data()
{
    Task t1;
    Task t2;

    QTest::addColumn<Task>("t1");
    QTest::addColumn<Task>("t2");
    QTest::addColumn<bool>("expected");

    QTest::newRow("default constructed") << t1 << t2 << true;
    t1.setDate(QDate::currentDate().addDays(2));
    t2.setDate(QDate::currentDate().addDays(2));
    QTest::newRow("date set 2 days forward") << t1 << t2 << true;
    t2.setTitle(QString("New title"));
    QTest::newRow("task 2 with new title") << t1 << t2 << false;
    t2 = t1;
    QTest::newRow("task 1 assigned to task 2") << t1 << t2 << true;
}

void TaskManagerTest::testTaskEqual()
{
    QFETCH(Task, t1);
    QFETCH(Task, t2);
    QFETCH(bool, expected);
    QCOMPARE(t1 == t2, expected);
}

void TaskManagerTest::testTaskSetAddTask()
{
    TaskSet set;
    Task *task = NULL;

    QVERIFY2(set.addTask(NULL) == 0, "NULL added as a task.");
    if (!(task = new Task()))
    {
        QSKIP("Not enough free memory.", SkipAll);
    }
    QVERIFY2(set.addTask(task) != 0, "New task not added.");
}

void TaskManagerTest::testTaskSetGetTask()
{
    TaskSet set;
    Task *t1 = new Task();
    Task *t2 = new Task();

    if (!t1)
    {
        QSKIP("Not enough free memory.", SkipAll);
    }
    if (!t2)
    {
        delete t1;
        QSKIP("Not enough free memory.", SkipAll);
    }
    quint32 id1 = set.addTask(t1);
    quint32 id2 = set.addTask(t2);
    QCOMPARE(*set.getTask(id1), *t1);
    QCOMPARE(*set.getTask(id2), *t2);
    QVERIFY2(set.getTask(id2+1) == NULL, "Task found but never created.");
    set.clear();
    QVERIFY2(set.getTask(id1) == NULL, "Task was deleted but still exists.");
    QVERIFY2(set.getTask(id2) == NULL, "Task was deleted but still exists.");
}

void TaskManagerTest::testTaskSetDeleteTask()
{
    TaskSet set;
    Task *task = new Task();
    quint32 id;

    if (!task)
    {
        QSKIP("Not enough free memory.", SkipAll);
    }
    id = set.addTask(task);
    QCOMPARE(set.deleteTask(0), false);
    QCOMPARE(set.deleteTask(id), true);
    QCOMPARE(set.deleteTask(id), false);
}

void TaskManagerTest::testTaskSetSize()
{
    TaskSet set;
    Task *task = NULL;
    quint32 id;

    QCOMPARE(set.size(), 0);
    set.addTask(NULL);
    QCOMPARE(set.size(), 0);
    if (!(task = new Task()))
    {
        QSKIP("Not enough free memory.", SkipAll);
    }
    set.addTask(task);
    QCOMPARE(set.size(), 1);
    if (!(task = new Task()))
    {
        QSKIP("Not enough free memory.", SkipAll);
    }
    id = set.addTask(task);
    QCOMPARE(set.size(), 2);
    set.deleteTask(id);
    QCOMPARE(set.size(), 1);
    set.clear();
    QCOMPARE(set.size(), 0);
}

void TaskManagerTest::testTaskSetEqual()
{
    TaskSet s1;
    TaskSet s2;
    Task *task = NULL;

    QVERIFY2(s1 == s2, "Empty task sets should be equal.");
    if (!(task = new Task()))
    {
        QSKIP("Not enough free memory.", SkipAll);
    }
    s1.addTask(task);
    if (!(task = new Task()))
    {
        QSKIP("Not enough free memory.", SkipAll);
    }
    s2.addTask(task);
    QVERIFY2(s1 == s2, "Task sets with one default-constructed task should be equal.");
    s1.clear();
    s2.clear();
    if (!(task = new Task(QDate(2000, 1, 1), QTime(0, 0, 0),
                          QString("Test task"), QString("Description of test task"),
                          Task::LOW_PRIORITY, Task::COSMETIC)))
    {
        QSKIP("Not enough free memory.", SkipAll);
    }
    s1.addTask(task);
    s2 = s1;
    QVERIFY2(s1 == s2, "Task sets with the same tasks should be equal.");
}

void TaskManagerTest::testTaskManagerSingleton()
{
    QCOMPARE(TaskManager::getInstance(), TaskManager::getInstance());
}

QTEST_APPLESS_MAIN(TaskManagerTest)

#include "tst_TaskManagerTest.moc"
