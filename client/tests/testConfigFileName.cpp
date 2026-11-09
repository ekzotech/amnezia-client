#include <QTest>

#include "core/utils/utilities.h"

class TestConfigFileName : public QObject
{
    Q_OBJECT

private slots:
    void keepsPlainName();
    void trimsSpaces();
    void dropsTrailingDots();
    void dropsPathAndInvalidCharacters();
    void fallsBackForEmptyName();
    void keepsUnicodeName();
    void splitsClientNames();
    void dropsDuplicateAndEmptyClientNames();
    void keepsLongNamesSharingPrefix();
    void limitsFileNameLength();
    void limitsBatchSize();
    void makesFileNamesUnique();
};

void TestConfigFileName::keepsPlainName()
{
    QCOMPARE(Utils::configFileName("ivan"), QString("ivan"));
}

void TestConfigFileName::trimsSpaces()
{
    QCOMPARE(Utils::configFileName("  ivan petrov \t"), QString("ivan petrov"));
}

void TestConfigFileName::dropsTrailingDots()
{
    QCOMPARE(Utils::configFileName("ivan. ."), QString("ivan"));
    QCOMPARE(Utils::configFileName("ivan petrov."), QString("ivan petrov"));
}

void TestConfigFileName::dropsPathAndInvalidCharacters()
{
    QCOMPARE(Utils::configFileName("../../etc/passwd"), QString("etcpasswd"));
    QCOMPARE(Utils::configFileName("a:b*c?d\"e<f>g|h\\i"), QString("abcdefghi"));
}

void TestConfigFileName::fallsBackForEmptyName()
{
    QCOMPARE(Utils::configFileName(""), QString("client"));
    QCOMPARE(Utils::configFileName(" .. "), QString("client"));
}

void TestConfigFileName::keepsUnicodeName()
{
    const QString name = QString::fromUtf8("\xd0\x98\xd0\xb2\xd0\xb0\xd0\xbd");
    QCOMPARE(Utils::configFileName(name), name);
}

void TestConfigFileName::splitsClientNames()
{
    const QStringList expected = { "ivan", "petr", "anna" };
    QCOMPARE(Utils::clientNames("ivan, petr\nanna\r\n"), expected);
}

void TestConfigFileName::dropsDuplicateAndEmptyClientNames()
{
    QCOMPARE(Utils::clientNames("ivan, IVAN, ivan"), QStringList { "ivan" });
    QCOMPARE(Utils::clientNames("  \n , \n"), QStringList {});
}

void TestConfigFileName::keepsLongNamesSharingPrefix()
{
    const QStringList expected = { "Aleksandr Ivanovich Petrov", "Aleksandr Ivanovich Sokolov" };
    QCOMPARE(Utils::clientNames("Aleksandr Ivanovich Petrov, Aleksandr Ivanovich Sokolov"), expected);
}

void TestConfigFileName::limitsFileNameLength()
{
    QCOMPARE(Utils::configFileName(QString(100, 'a')), QString(64, 'a'));
}

void TestConfigFileName::limitsBatchSize()
{
    QStringList longList;
    for (int i = 0; i < 25; ++i) {
        longList << QString("user%1").arg(i);
    }
    QCOMPARE(Utils::clientNames(longList.join(",")).size(), 20);
}

void TestConfigFileName::makesFileNamesUnique()
{
    QCOMPARE(Utils::configFileNames({ "ivan", "ivan", "ivan" }), QStringList { "ivan", "ivan-2", "ivan-3" });
    QCOMPARE(Utils::configFileNames({ "ivan", "ivan", "Ivan" }), QStringList { "ivan", "ivan-2", "Ivan-3" });
    QCOMPARE(Utils::configFileNames({ "ivan", "ivan-2", "anna" }), QStringList { "ivan", "ivan-2", "anna" });
}

QTEST_MAIN(TestConfigFileName)
#include "testConfigFileName.moc"
