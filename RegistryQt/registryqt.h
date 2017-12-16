#ifndef REGISTRYQT_H
#define REGISTRYQT_H

/* Original author: Nikolai S | https://github.com/doublejim
 *
 * This file is released under the MIT license.
 */

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

typedef unsigned __int64 QWORD;     // DEFINE QWORD

#include "regvalue.h"

#include <qt_windows.h>
#include <QByteArray>
#include <QDebug>
#include <QStringList>

class RegistryQt
{
public:
    RegistryQt();
    QStringList getSubkeys(HKEY rootKey, QString directory);
    RegValue getValue(HKEY rootKey, QString directory, QString valueName);
    // getValueNames
    // bool keyExists()
    // bool valueExists()

    bool insertKey(const HKEY rootKey, const QString directory);
    bool insertValue(HKEY rootKey, QString directory, QString valueName, const BYTE *value, DWORD valueType, int sizeOfValue);
    bool insertValueBinary(const HKEY rootKey, const QString directory, const QString valueName, const QByteArray &value);
    bool insertValueDWORD(const HKEY rootKey, const QString directory, const QString valueName, const DWORD &value);
    bool insertValueExpandSZ(const HKEY rootKey, const QString directory, const QString valueName, const QString &value);
    bool insertValueMultiSZ(const HKEY rootKey, const QString directory, const QString valueName, const QStringList &value);
    bool insertValueQWORD(const HKEY rootKey, const QString directory, const QString valueName, const QWORD &value);
    bool insertValueSZ(const HKEY rootKey, const QString directory, const QString valueName, const QString &value);

    void removeKey(HKEY rootKey, QString directory);
};

#endif // REGISTRYQT_H
