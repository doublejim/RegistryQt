#include "regvalue.h"

/* Original author: Nikolai S | https://github.com/doublejim
 *
 * This file is released under the MIT license.
 */

RegValue::RegValue(){}

RegValue::~RegValue()
{
    delete[] data;
    data = nullptr;
}

RegValue::RegValue(DWORD errorCode, BYTE *source, DWORD size, DWORD type)
{
    if (errorCode == ERROR_SUCCESS)
        this->exists = true;

    this->errorCode = errorCode;
    this->size = size;
    this->data = new BYTE[size];

    for (uint i = 0; i < size; ++i)
        data[i] = source[i];

    delete[] source;
    source = nullptr;

    this->type = type;
}

QString RegValue::toString()
{
    return QString::fromWCharArray( (wchar_t*) data);
}

QString RegValue::toExpandedString()
{
    wchar_t* unexpandedString = (wchar_t*) data;
    TCHAR expandedString[MAX_KEY_LENGTH];

    DWORD requiredSize = ExpandEnvironmentStrings( unexpandedString, expandedString, MAX_KEY_LENGTH);

    if (requiredSize < MAX_KEY_LENGTH)
    {
        return QString::fromWCharArray( (wchar_t*) expandedString);
    } else {
        TCHAR* expandedStringLarger = new TCHAR[requiredSize];
        ExpandEnvironmentStrings( unexpandedString, expandedStringLarger, requiredSize);
        QString result = QString::fromWCharArray( (wchar_t*) expandedStringLarger);
        delete[] expandedStringLarger;
        expandedStringLarger = nullptr;
        return result;
    }
}

QStringList RegValue::toStringList()
{
    QStringList list;
    uint sizecopied = 0;

    do {
        QString line = QString::fromWCharArray( (wchar_t*) data + sizecopied);

        if (line.isEmpty())
            break;

        list.append( line);
        sizecopied += list.last().size() + 1;

    } while (sizecopied < size);

    return list;
}

DWORD RegValue::toDword()
{
    return qFromLittleEndian((data[0]) | (data[1] << 8) | (data[2] << 16) | (data[3] << 24));
}

QWORD RegValue::toQword()
{
    QWORD result = 0;

    result |= data[0];
    result |= data[1] << 8;
    result |= data[2] << 16;
    result |= data[3] << 24;
    result |= (QWORD) data[4] << 32;
    result |= (QWORD) data[5] << 40;
    result |= (QWORD) data[6] << 48;
    result |= (QWORD) data[7] << 56;

    return qFromLittleEndian(result);
}

QByteArray RegValue::toByteArray()
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.setData(reinterpret_cast<const char*>(data), size);
    return bytes;
}
