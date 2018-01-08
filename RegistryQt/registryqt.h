#ifndef REGISTRYQT_H
#define REGISTRYQT_H

/* Original author: Nikolai S | https://github.com/doublejim
 *
 * This file is released under the MIT license.
 */

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#ifdef Q_PROCESSOR_X86_64
typedef unsigned __int64 QWORD;     // DEFINE QWORD
#endif

#include "regvalue.h"

#include <qt_windows.h>
#include <QByteArray>
#include <QDebug>
#include <QStringList>

namespace RegistryQt
{
    // Retrieve data
    extern QStringList subkeys(const HKEY rootKey, const QString subkey);
    extern RegValue value(const HKEY rootKey, const QString subkey, const QString valueName);
    extern QStringList valueNames(const HKEY rootKey, const QString subkey);
    extern bool valueExists(const HKEY rootKey, const QString subkey, const QString valueName);
    extern bool keyExists(const HKEY rootKey, const QString subkey);

    // Modify registry
    extern bool insertKey(const HKEY rootKey, const QString subkey);
    extern bool insertValue(const HKEY rootKey, const QString subkey, const QString valueName, const BYTE *value, const DWORD valueType, const int sizeOfValue);
    extern bool insertValueBinary(const HKEY rootKey, const QString subkey, const QString valueName, const QByteArray &value);
    extern bool insertValueDWORD(const HKEY rootKey, const QString subkey, const QString valueName, const DWORD &value);
    extern bool insertValueExpandSZ(const HKEY rootKey, const QString subkey, const QString valueName, const QString &value);
    extern bool insertValueMultiSZ(const HKEY rootKey, const QString subkey, const QString valueName, const QStringList &value);
    #ifdef Q_PROCESSOR_X86_64
    extern bool insertValueQWORD(const HKEY rootKey, const QString subkey, const QString valueName, const QWORD &value);
    #endif
    extern bool insertValueSZ(const HKEY rootKey, const QString subkey, const QString valueName, const QString &value);
    extern void removeKey(const HKEY rootKey, const QString subkey);
}

#endif // REGISTRYQT_H
