#ifndef REGISTRYQT_TESTS_H
#define REGISTRYQT_TESTS_H

/* Original author: Nikolai S | https://github.com/doublejim
 *
 * This file is released under the MIT license.
 */

#include "registryqt.h"
#include <QImage>
#include <QString>
#include <QDebug>

static bool check( int& okay, int& fail, QString& description, bool value, bool value2 = true)
{
    if (value && value2)
    {
        ++okay;
        return true;
    } else {
        ++fail;
        if (value2)
            qDebug() << "failed at:" << description;        // if the command didn't report any error (returned true).
        else
            qDebug() << "failed at:" << description << ". reports failing."; // if the command returned false.
        return false;
    }
}

static void RunTests()
{
    int tests_fail = 0;
    int tests_okay = 0;

    QString TestSubkey = "SOFTWARE\\___0TESTA___\\";          // <= THIS IS THE ROOT KEY OF ALL TESTS BEING RUN.
                                                              //    SET IT TO SOMETHING UNIQUE. IT WILL BE DELETED
                                                              //    IN THE END.
    {
        QString description = "Binary to registry and back.";

        QImage img (75,75, QImage::Format_RGBA8888);
        img.fill(Qt::red);

        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "PNG");

        bool okay = RegistryQt::insertValueBinary( HKEY_LOCAL_MACHINE, TestSubkey, "great", bytes);
        RegValue value = RegistryQt::value( HKEY_LOCAL_MACHINE, TestSubkey, "great");

        check( tests_okay, tests_fail, description, (bytes == value.toByteArray()), okay);
    }

    {
        QString description = "QStringList with unicode letters in key, valueName and value";

        QStringList list;
        list << "hello" << "born" << "æ测试å" << "in" << "the" << "usa" << "æ测试å";
        bool okay = RegistryQt::insertValueMultiSZ( HKEY_LOCAL_MACHINE, TestSubkey + "Børachio\\测试", "øresund测试", list);
        RegValue value = RegistryQt::value( HKEY_LOCAL_MACHINE, TestSubkey + "Børachio\\测试", "øresund测试");

        check( tests_okay, tests_fail, description, (list == value.toStringList()), okay);
    }

    {
        QString description = "QString with unicode letters in key, valueName and value";

        QString toSave = "测试alabama";
        bool okay = RegistryQt::insertValueSZ( HKEY_LOCAL_MACHINE, TestSubkey + "Mustar测试\\测试dæåp", "测试", toSave);
        RegValue value = RegistryQt::value( HKEY_LOCAL_MACHINE, TestSubkey + "Mustar测试\\测试dæåp", "测试");

        check( tests_okay, tests_fail, description, (toSave == value.toString()), okay);
    }

    {
        QString description = "QString to valueExpandSz";

        QString toSave = "LOL";
        bool okay = RegistryQt::insertValueExpandSZ( HKEY_LOCAL_MACHINE, TestSubkey, "bland", toSave);
        RegValue value = RegistryQt::value( HKEY_LOCAL_MACHINE, TestSubkey, "bland");
        check( tests_okay, tests_fail, description, (toSave == value.toString()), okay);
    }

    {
        QString description = "DWORD";

        DWORD toSave = 1561;
        bool okay = RegistryQt::insertValueDWORD( HKEY_LOCAL_MACHINE, TestSubkey, "blandf\\asdf", toSave);
        RegValue value = RegistryQt::value( HKEY_LOCAL_MACHINE, TestSubkey, "blandf\\asdf");
        if (!check( tests_okay, tests_fail, description, (toSave == value.toDword()), okay))
            qDebug() << "should have:" << toSave << " but got:" << value.toDword();
    }

    #ifdef Q_PROCESSOR_X86_64
    {
        QString description = "QWORD";

        QWORD toSave = 1565465448948976866;
        bool okay = RegistryQt::insertValueQWORD( HKEY_LOCAL_MACHINE, TestSubkey, "blandf\\g/ohome", toSave);
        RegValue value = RegistryQt::value( HKEY_LOCAL_MACHINE, TestSubkey, "blandf\\g/ohome");
        if (!check( tests_okay, tests_fail, description, (toSave == value.toQword()), okay))
            qDebug() << "should have:" << toSave << " but got:" << value.toQword();
    }
    #else
    {
        DWORD toSave = 1565465447;
        RegistryQt::insertValueDWORD( HKEY_LOCAL_MACHINE, TestSubkey, "blandf\\g/ohome", toSave);
    }
    #endif

    {
        QString description = "Get value names";

        QStringList getList = RegistryQt::valueNames( HKEY_LOCAL_MACHINE, TestSubkey );
        QStringList correctList = {"bland", "blandf\\asdf", "blandf\\g/ohome", "great"};
        bool okay = (getList == correctList);
        check( tests_okay, tests_fail, description, okay);
    }

    {
        QString description = "Value exists positive";

        bool okay = RegistryQt::valueExists( HKEY_LOCAL_MACHINE, TestSubkey, "great");

        check( tests_okay, tests_fail, description, okay);
    }

    {
        QString description = "Value exists negative";

        bool exists = RegistryQt::valueExists( HKEY_LOCAL_MACHINE, TestSubkey, "grea");

        check( tests_okay, tests_fail, description, exists == false);
    }

    {
        QString description = "Key exists positive 1";

        bool exists = RegistryQt::keyExists( HKEY_LOCAL_MACHINE, TestSubkey);

        check( tests_okay, tests_fail, description, exists);
    }

    {
        QString description = "Key exists positive 2";

        bool exists = RegistryQt::keyExists( HKEY_LOCAL_MACHINE, "SOFTWARE");

        check( tests_okay, tests_fail, description, exists);
    }

    {
        QString description = "Key exists negative 1";

        bool exists = RegistryQt::keyExists( HKEY_LOCAL_MACHINE, TestSubkey + "\\great");

        check( tests_okay, tests_fail, description, exists == false);
    }

    {
        QString description = "Key exists negative 2";

        bool exists = RegistryQt::keyExists( HKEY_LOCAL_MACHINE, "SOFTWAR");

        check( tests_okay, tests_fail, description, exists == false);
    }

    {
        QString description = "Expand sz test 1";

        RegistryQt::insertValueExpandSZ(HKEY_LOCAL_MACHINE, TestSubkey + "\\1A", "checkerBoy", "%windir%\\gruesome_test\\%homedrive%");
        RegValue value = RegistryQt::value(HKEY_LOCAL_MACHINE, TestSubkey + "\\1A", "checkerBoy");

        // Assuming the windir is C:\Windows

        bool okayA = value.toExpandedString() == "C:\\Windows\\gruesome_test\\C:";
        bool okayB = value.toString() == "%windir%\\gruesome_test\\%homedrive%";

        check( tests_okay, tests_fail, description, okayA && okayB);
    }

    {
        QString description = "Expand sz test 2";

        // This is long enough to be expanded to a size of more than 255.
        QString longstr = QString("%windir%").repeated(26);

        RegistryQt::insertValueExpandSZ(HKEY_LOCAL_MACHINE, TestSubkey + "\\1A", "checkerBoy", longstr);
        RegValue value = RegistryQt::value(HKEY_LOCAL_MACHINE, TestSubkey + "\\1A", "checkerBoy");

        // Assuming the windir is C:\Windows

        QString expandedstr = QString("C:\\Windows").repeated(26);
        bool okay = expandedstr == value.toExpandedString();

        check( tests_okay, tests_fail, description, okay);
    }

    {
        QString description = "Remove test";
        RegistryQt::removeKey( HKEY_LOCAL_MACHINE, TestSubkey);
        bool exists = RegistryQt::keyExists( HKEY_LOCAL_MACHINE, TestSubkey);

        check( tests_okay, tests_fail, description, exists == false);
    }

    qDebug() << "tests succeeded:" << tests_okay;
    qDebug() << "tests failed:   " << tests_fail;
}

#endif // REGISTRYQT_TESTS_H
