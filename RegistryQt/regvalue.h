#ifndef REGVALUE_H
#define REGVALUE_H

/* Original author: Nikolai S | https://github.com/doublejim
 *
 * This file is released under the MIT license.
 */

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#include <qt_windows.h>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QBuffer>
#include <QtEndian>

typedef unsigned __int64 QWORD;     // DEFINE QWORD

struct RegValue {
    BYTE* data = nullptr;
    bool exists = false;
    DWORD type = REG_NONE;
    DWORD errorCode = ERROR_SUCCESS;
    DWORD size = 0;

    RegValue();
    ~RegValue();
    RegValue( DWORD errorCode, BYTE* source, DWORD size, DWORD type);
    QString toString();
    QString toExpandedString();
    QStringList toStringList();
    DWORD toDword();
    QWORD toQword();
    QByteArray toByteArray();
};

#endif // REGVALUE_H
