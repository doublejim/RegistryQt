# RegistryQt
Easily use the Windows Registry with Qt.
This library allows you to perform several Windows Registry operations.<br>
You can write QString and QStringList directly to the Registry.

How to get started:<br>
• Add the 4 files: *registryqt.cpp, registryqt.h, regvalue.cpp, regvalue.h* to your project.<br>
• \#include "registryqt.h"<br>
• LIBS += -lAdvapi32<br>

## FEATURES ##

• Write all basic types of Registry values: Binary, DWORD, ExpandSZ, MultiSZ, QWORD, SZ.<br>
• Use basic Qt data types as input: QByteArray, QString, QStringList.<br>
• Delete a key and all of its subkeys.<br>
• Read a value, and easily convert to a Qt data type.<br>
• Also works with chinese characters, \ and /.

## THINGS IN THE WORKS ##
• I am not certain that all errors are handled correctly.<br>
• Some functions and variables should probably be renamed.<br>
• Maybe I want a RegOperation struct that contains information on an attempted operation.<br>
• Make sure that it also compiles and works on Windows XP.<br>
• I want "keyExists()" and "valueExists()" functions.<br>
• I want to be able to get a list of all valueNames in a key.<br>

## Example 1: Write/Read a string ##
```c++
RegistryQt registry;
QString value = "value";
registry.insertValueSZ( HKEY_LOCAL_MACHINE, "Software\\1337-test", "hello there", value);
RegValue regValue = registry.getValue( HKEY_LOCAL_MACHINE, "Software\\1337-test", "hello there");
qDebug() << regValue.toString();
```
### Result ###

Qt prints out: "value" to the console.

## Example 2: Write/Read a string list ##
```c++
RegistryQt registry;
QStringList list;
list << "first list item" << "second list item" << "third list item";
registry.insertValueMultiSZ( HKEY_LOCAL_MACHINE, "Software\\1337-test", "cool, a list", list);
RegValue regValue = registry.getValue( HKEY_LOCAL_MACHINE, "Software\\1337-test", "cool, a list");
qDebug() << regValue.toStringList();
```
### Result ###

Qt prints out: ("first list item", "second list item", "third list item") to the console.

## Example 3: Write/Read binary ##
```c++
QImage img (75,75, QImage::Format_RGBA8888);
img.fill(Qt::red);

QByteArray bytes;
QBuffer buffer(&bytes);
buffer.open(QIODevice::WriteOnly);
img.save(&buffer, "PNG");

RegistryQt registry;
registry.insertValueBinary( HKEY_LOCAL_MACHINE, "Software\\1337-test", "great", bytes);
RegValue value = registry.getValue( HKEY_LOCAL_MACHINE, "Software\\1337-test", "great");
QByteArray bytes2 = value.toByteArray();
qDebug() << (bytes == bytes2);
```
### Result ###

bytes2 now contains the data of bytes. So Qt prints out: true.

## Example 4: Write/Read DWORD ##
```c++
RegistryQt registry;
registry.insertValueDWORD( HKEY_LOCAL_MACHINE, "Software\\1337-test", "word", 56456);
RegValue regValue = registry.getValue( HKEY_LOCAL_MACHINE, "Software\\1337-test", "word");
qDebug() << (regValue.type == REG_DWORD);
qDebug() << regValue.toDword();
```

### Result ###

Qt prints out: true<br>
56456

