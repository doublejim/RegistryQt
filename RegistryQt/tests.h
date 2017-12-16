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
    RegistryQt registry;

    int tests_fail = 0;
    int tests_okay = 0;

    QString keysRoot = "SOFTWARE\\___0TESTA___\\";          // <= THIS IS THE ROOT KEY OF ALL TESTS BEING RUN.

    {
        QString description = "Binary to registry and back.";

        QImage img (75,72, QImage::Format_RGBA8888);
        img.fill(Qt::red);

        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "PNG");

        bool okay = registry.insertValueBinary( HKEY_LOCAL_MACHINE, keysRoot, "great", bytes);
        RegValue value = registry.getValue( HKEY_LOCAL_MACHINE, keysRoot, "great");

        check( tests_okay, tests_fail, description, (bytes == value.toByteArray()), okay);
    }

    {
        QString description = "QStringList with unicode letters in key, valueName and value";

        QStringList list;
        list << "hello" << "born" << "æ测试å" << "in" << "the" << "usa" << "æ测试å";
        bool okay = registry.insertValueMultiSZ( HKEY_LOCAL_MACHINE, keysRoot + "Børachio\\测试", "øresund测试", list);
        RegValue value = registry.getValue( HKEY_LOCAL_MACHINE, keysRoot + "Børachio\\测试", "øresund测试");

        check( tests_okay, tests_fail, description, (list == value.toStringList()), okay);
    }

    {
        QString description = "QString with unicode letters in key, valueName and value";

        QString toSave = "测试alabama";
        bool okay = registry.insertValueSZ( HKEY_LOCAL_MACHINE, keysRoot + "Mustar测试\\测试dæåp", "测试", toSave);
        RegValue value = registry.getValue( HKEY_LOCAL_MACHINE, keysRoot + "Mustar测试\\测试dæåp", "测试");

        check( tests_okay, tests_fail, description, (toSave == value.toString()), okay);
    }

    {
        QString description = "QString to valueExpandSz";

        QString toSave = "LOL";
        bool okay = registry.insertValueExpandSZ( HKEY_LOCAL_MACHINE, keysRoot, "bland", toSave);
        RegValue value = registry.getValue( HKEY_LOCAL_MACHINE, keysRoot, "bland");
        check( tests_okay, tests_fail, description, (toSave == value.toString()), okay);
    }

    {
        QString description = "DWORD";

        DWORD toSave = 1561;
        bool okay = registry.insertValueDWORD( HKEY_LOCAL_MACHINE, keysRoot, "blandf\\asdf", toSave);
        RegValue value = registry.getValue( HKEY_LOCAL_MACHINE, keysRoot, "blandf\\asdf");
        if (!check( tests_okay, tests_fail, description, (toSave == value.toDword()), okay))
            qDebug() << "should have:" << toSave << " but got:" << value.toDword();
    }

    {
        QString description = "QWORD";

        QWORD toSave = 1565465448948976866;
        bool okay = registry.insertValueQWORD( HKEY_LOCAL_MACHINE, keysRoot, "blandf\\g/ohome", toSave);
        RegValue value = registry.getValue( HKEY_LOCAL_MACHINE, keysRoot, "blandf\\g/ohome");
        if (!check( tests_okay, tests_fail, description, (toSave == value.toQword()), okay))
            qDebug() << "should have:" << toSave << " but got:" << value.toQword();
    }

    {
        QString description = "Insert key / key exists";

        registry.insertKey( HKEY_LOCAL_MACHINE, keysRoot + "bananas\\øzebra");

        //RegValue value = registry.getValue( HKEY_LOCAL_MACHINE, keysRoot, "blandf\\gohome");
    }

    qDebug() << "";
    qDebug() << "tests succeeded:" << tests_okay;
    qDebug() << "tests failed:   " << tests_fail;
}

#endif // REGISTRYQT_TESTS_H
