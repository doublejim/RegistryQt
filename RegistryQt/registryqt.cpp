#include "registryqt.h"

/* Original author: Nikolai S | https://github.com/doublejim
 *
 * This file is released under the MIT license.
 */

RegistryQt::RegistryQt(){}

QStringList RegistryQt::getSubkeys( HKEY rootKey, QString directory)
{
    HKEY hKey;                              // Handle

    QStringList result;

    if (ERROR_SUCCESS == RegOpenKeyEx( rootKey, (LPCWSTR) directory.utf16(), 0, KEY_READ, &hKey))
    {
        TCHAR achKey[MAX_KEY_LENGTH];       // Buffer for subkey name
        DWORD cSubKeys = 0;                 // Number of subkeys
        DWORD retCode;

        // Get the number of subkeys:

        RegQueryInfoKey(hKey, NULL, NULL, NULL, &cSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        // If there are any subkeys, we iterate through them, gathering their names.

        if (cSubKeys)
        {
            for (DWORD i = 0; i < cSubKeys; i++)
            {
                DWORD cbName = MAX_KEY_LENGTH;       // Size of name string

                // Find the name of the key at index i:

                retCode = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL, NULL, NULL, NULL);

                // Append key name to results:

                if (retCode == ERROR_SUCCESS)
                    result.append( QString::fromWCharArray(achKey));
            }
        }
    }

    RegCloseKey(hKey);

    return result;
}

bool RegistryQt::insertValue( const HKEY rootKey, const QString directory, const QString valueName, const BYTE* value, DWORD valueType, int sizeOfValue)
{
    HKEY hKey;           // Handle
    bool okay = true;

    if (ERROR_SUCCESS == RegCreateKeyEx( rootKey, (LPCWSTR) directory.utf16(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL))
    {
        DWORD retCode = RegSetValueEx( hKey, (LPCWSTR) valueName.utf16(), 0, valueType, value, sizeOfValue);

        if (retCode != ERROR_SUCCESS)
            okay = false;
    } else {
        okay = false;
    }
    RegCloseKey(hKey);

    return okay;
}

bool RegistryQt::insertKey( const HKEY rootKey, const QString directory)
{
    HKEY hKey;           // Handle
    bool okay = true;

    if (ERROR_SUCCESS != RegCreateKeyEx( rootKey, (LPCWSTR) directory.utf16(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL))
        okay = false;

    RegCloseKey(hKey);
    return okay;
}

bool RegistryQt::insertValueDWORD( const HKEY rootKey, const QString directory, const QString valueName, const DWORD& value)
{
    return insertValue( rootKey, directory, valueName, (const BYTE*) &value, REG_DWORD, sizeof(DWORD));
}

bool RegistryQt::insertValueQWORD( const HKEY rootKey, const QString directory, const QString valueName, const QWORD& value)
{
    return insertValue( rootKey, directory, valueName, (const BYTE*) &value, REG_QWORD, sizeof(QWORD));
}

bool RegistryQt::insertValueSZ( const HKEY rootKey, const QString directory, const QString valueName, const QString& value)
{
    wchar_t* castVal = (wchar_t*) value.utf16();
    return insertValue( rootKey, directory, valueName, (const BYTE*) castVal, REG_SZ, value.size() * sizeof(wchar_t));
}

bool RegistryQt::insertValueMultiSZ(const HKEY rootKey, const QString directory, const QString valueName, const QStringList &value)
{
    QString combinedData;

    for (const QString& str : value)
        combinedData.append( str + QChar('\0'));
    combinedData.append(QChar('\0'));

    wchar_t* castVal = (wchar_t*) combinedData.utf16();
    return insertValue( rootKey, directory, valueName, (const BYTE*) castVal, REG_MULTI_SZ, combinedData.size() * sizeof(wchar_t));
}

bool RegistryQt::insertValueExpandSZ( const HKEY rootKey, const QString directory, const QString valueName, const QString& value)
{
    wchar_t* castVal = (wchar_t*) value.utf16();
    return insertValue( rootKey, directory, valueName, (const BYTE*) castVal, REG_EXPAND_SZ, value.size() * sizeof(wchar_t));
}

bool RegistryQt::insertValueBinary( const HKEY rootKey, const QString directory, const QString valueName, const QByteArray& value)
{
    return insertValue( rootKey, directory, valueName, (const BYTE*) value.constData(), REG_BINARY, value.size() * sizeof(BYTE));
}

RegValue RegistryQt::getValue( HKEY rootKey, QString directory, QString valueName)
{
    BYTE* data = new BYTE[MAX_KEY_LENGTH];      // Deleted by RegistryValueInfo after it has been copied.
    DWORD cbData = MAX_KEY_LENGTH;
    DWORD type;
    HKEY hKey;
    bool okay = true;

    DWORD retCode = RegOpenKeyEx( rootKey, (LPCWSTR) directory.utf16(), 0, KEY_READ, &hKey);

    if (retCode == ERROR_SUCCESS)       // Directory existed.
    {
        // Retrieve value.
        retCode = RegQueryValueEx( hKey, (LPCWSTR) valueName.utf16(), 0, &type, data, &cbData);

        switch(retCode)
        {
        case ERROR_MORE_DATA:           // The 255 bytes aren't enough. Allocate the necessary amount.
        {
            delete data;
            data = new BYTE[cbData];
            retCode = RegQueryValueEx( hKey, (LPCWSTR) valueName.utf16(), 0, &type, data, &cbData);
        }
        default: break;
        }
    }

    if (retCode != ERROR_SUCCESS)
        okay = false;

    RegCloseKey(hKey);

    return RegValue( retCode, data, cbData, type);
}

void RegistryQt::removeKey( HKEY rootKey, QString directory)
{
    HKEY hKey;

    if (ERROR_SUCCESS == RegOpenKeyEx( rootKey, (LPCWSTR) directory.utf16(), 0, KEY_READ, &hKey))
    {
        DWORD cSubKeys = 0;                  // number of subkeys

        // Get the number of subkeys:

        RegQueryInfoKey(hKey, NULL, NULL, NULL, &cSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        if (cSubKeys)                                   // If there are any subkeys:
        {                                               // Go to each subkey.
            for (int i = cSubKeys - 1; i >= 0; --i)     // Find the name of the key at index i. Start at the highest index,
            {                                           // then delete lower and lower indices until (including) index 0.
                TCHAR achKey[MAX_KEY_LENGTH];           // Buffer for subkey name
                DWORD cbName = MAX_KEY_LENGTH;          // Size of name string

                DWORD retCode = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL, NULL, NULL, NULL);

                if (retCode == ERROR_SUCCESS)
                {
                    QString subkey = directory + "\\" + QString::fromWCharArray(achKey);
                    removeKey( rootKey, subkey);
                }
            }
        }
        RegDeleteKey( rootKey, (LPCWSTR) directory.utf16());
    }
    RegCloseKey(hKey);
}
