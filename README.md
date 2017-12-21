# RegistryQt
This library allows you to perform several Windows Registry operations.<br>
You can use these functions with QString and other typical Qt data types as arguments.

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
• ~~Some functions and variables should probably be renamed.~~ it's looking pretty snazzy now<br>
• Maybe I want a RegOperation struct that contains information on an attempted operation.<br>
• Make sure that it compiles and works on Windows XP, Windows 7, Windows 8.1, Windows 10.<br>
• ~~I want "keyExists()" and "valueExists()" functions.~~ done<br>
• ~~I want to be able to get a list of all valueNames in a key.~~ done<br>

## Example 1: Write/Read a string ##
```c++
QString value = "value";
RegistryQt::insertValueSZ( HKEY_LOCAL_MACHINE, "Software\\1337-test", "hello there", value);
RegValue regValue = RegistryQt::getValue( HKEY_LOCAL_MACHINE, "Software\\1337-test", "hello there");
qDebug() << RegistryQt::toString();
```
### Result ###

Qt prints out: "value" to the console.

## Example 2: Write/Read a string list ##
```c++
QStringList list;
list << "first list item" << "second list item" << "third list item";
RegistryQt::insertValueMultiSZ( HKEY_LOCAL_MACHINE, "Software\\1337-test", "cool, a list", list);
RegValue regValue = RegistryQt::getValue( HKEY_LOCAL_MACHINE, "Software\\1337-test", "cool, a list");
qDebug() << RegistryQt::toStringList();
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

RegistryQt::insertValueBinary( HKEY_LOCAL_MACHINE, "Software\\1337-test", "great", bytes);
RegValue value = RegistryQt::getValue( HKEY_LOCAL_MACHINE, "Software\\1337-test", "great");
QByteArray bytes2 = value.toByteArray();
qDebug() << (bytes == bytes2);
```
### Result ###

bytes2 now contains the data of bytes. So Qt prints out: true.

## Example 4: Write/Read DWORD ##
```c++
RegistryQt::insertValueDWORD( HKEY_LOCAL_MACHINE, "Software\\1337-test", "word", 56456);
RegValue regValue = RegistryQt::getValue( HKEY_LOCAL_MACHINE, "Software\\1337-test", "word");
qDebug() << (regValue.type == REG_DWORD);
qDebug() << regValue.toDword();
```

### Result ###

Qt prints out: true<br>
56456

