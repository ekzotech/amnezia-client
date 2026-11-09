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
    void limitsClientNameLength();
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
    const QStringList names = Utils::clientNames("Aleksandr Ivanovich Petrov, Aleksandr Ivanovich Sokolov");
    QCOMPARE(names.size(), 2);
    QCOMPARE(names.first(), QString("Aleksandr Ivanovich"));
}

void TestConfigFileName::limitsClientNameLength()
{
    const QString longName(25, 'a');
    QCOMPARE(Utils::clientNames(longName), QStringList { QString(20, 'a') });
}

void TestConfigFileName::makesFileNamesUnique()
{
    QCOMPARE(Utils::configFileNames({ "ivan", "ivan", "ivan" }), QStringList { "ivan", "ivan-2", "ivan-3" });
    QCOMPARE(Utils::configFileNames({ "ivan", "ivan", "Ivan" }), QStringList { "ivan", "ivan-2", "Ivan-3" });
    QCOMPARE(Utils::configFileNames({ "ivan", "ivan-2", "anna" }), QStringList { "ivan", "ivan-2", "anna" });
}

QTEST_MAIN(TestConfigFileName)
#include "testConfigFileName.moc"
