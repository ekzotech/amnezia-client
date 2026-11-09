#include <QFile>
#include <QTemporaryDir>
#include <QTest>

#include "ui/controllers/systemController.h"

class TestSaveFiles : public QObject
{
    Q_OBJECT

private slots:
    void writesEveryFile();
    void overwritesExistingFile();
    void rejectsEmptyList();
    void reportsFailureWhenDirectoryIsMissing();
};

void TestSaveFiles::writesEveryFile()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    const QList<QPair<QString, QString>> files = {
        { "ivan.conf", "[Interface]\n" },
        { "petr.conf", "[Interface]\nAddress = 10.8.0.3\n" }
    };

    QVERIFY(SystemController::saveFiles(dir.path(), files, false));

    QFile ivan(dir.path() + "/ivan.conf");
    QVERIFY(ivan.open(QIODevice::ReadOnly));
    QCOMPARE(ivan.readAll(), QByteArray("[Interface]\n"));
    ivan.close();

    QFile petr(dir.path() + "/petr.conf");
    QVERIFY(petr.open(QIODevice::ReadOnly));
    QCOMPARE(petr.readAll(), QByteArray("[Interface]\nAddress = 10.8.0.3\n"));
}

void TestSaveFiles::overwritesExistingFile()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QFile existing(dir.path() + "/ivan.conf");
    QVERIFY(existing.open(QIODevice::WriteOnly));
    existing.write("[Interface]\nAddress = 10.8.0.9\n");
    existing.close();

    QVERIFY(SystemController::saveFiles(dir.path(), { { "ivan.conf", "[Interface]\n" } }, false));

    QFile saved(dir.path() + "/ivan.conf");
    QVERIFY(saved.open(QIODevice::ReadOnly));
    QCOMPARE(saved.readAll(), QByteArray("[Interface]\n"));
}

void TestSaveFiles::rejectsEmptyList()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    QVERIFY(!SystemController::saveFiles(dir.path(), {}, false));
}

void TestSaveFiles::reportsFailureWhenDirectoryIsMissing()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    QVERIFY(!SystemController::saveFiles(dir.path() + "/missing", { { "ivan.conf", "[Interface]\n" } }, false));
}

QTEST_MAIN(TestSaveFiles)
#include "testSaveFiles.moc"
