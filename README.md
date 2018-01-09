# RegistryQt
This library allows you to perform several Windows Registry operations in Qt.<br>
You can use these functions with QString and other typical Qt data types as arguments.

How to get started:<br>
• Add the 4 files: *registryqt.cpp, registryqt.h, regvalue.cpp, regvalue.h* to your project.<br>
• \#include "registryqt.h"<br>
• LIBS += -lAdvapi32<br>

## FEATURES ##

• Read/Write all types of Registry values: Binary, DWORD, ExpandSZ, MultiSZ, QWORD, SZ.<br>
• Use the Qt data types you know and love: QByteArray, QString, QStringList.<br>
• Easily convert a read value to a Qt data type.<br>
• Delete a Registry key and all of its subkeys.<br>
• Expand both expandable and non-expandable strings!<br>
• Value names are allowed to contain both \ and /.<br>
• Keys are allowed to contain /.<br>
• Keys, value names and values are allowed to contain for instance Japanese or Chinese characters.

## THINGS IN THE WORKS ##
• I am not certain that all errors are handled correctly.<br>
• ~~Some functions and variables should probably be renamed.~~ it's looking pretty snazzy now<br>
• Maybe I want a RegOperation struct that contains information on an attempted operation. (probably not)<br>
• Make sure that it compiles and works on Windows XP, Windows 7, Windows 8.1, Windows 10.<br>
• ~~Do so that Expandable String values are easily expanded.~~ done<br>
• More tests?<br>
• ~~I want "keyExists()" and "valueExists()" functions.~~ done<br>
• ~~I want to be able to get a list of all valueNames in a key.~~ done<br>

## Example 1: Write/Read a string ##
```c++
QString value = "value";
RegistryQt::insertValueSZ( HKEY_LOCAL_MACHINE, "Software\\1337-test", "hello there", value);
RegValue regValue = RegistryQt::getValue( HKEY_LOCAL_MACHINE, "Software\\1337-test", "hello there");
qDebug() << regValue.toString();
```
### Result ###

Qt prints out: "value" to the console.

## Example 2: Write/Read a string list ##
```c++
QStringList list;
list << "first list item" << "second list item" << "third list item";
RegistryQt::insertValueMultiSZ( HKEY_LOCAL_MACHINE, "Software\\1337-test", "cool, a list", list);
RegValue regValue = RegistryQt::getValue( HKEY_LOCAL_MACHINE, "Software\\1337-test", "cool, a list");
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

## Example 5: Expandable strings ##
```c++
RegistryQt::insertValueExpandSZ(HKEY_LOCAL_MACHINE, "Software\\1-test", "test", "%windir%");
RegValue value = RegistryQt::value(HKEY_LOCAL_MACHINE, "Software\\1-test", "test");
qDebug() << "expanded:" << value.toExpandedString();
qDebug() << "as string:" << value.toString();
```
### Result ###

Qt prints out:<br>
expanded: "C:\\Windows"<br>
as string: "%windir%"<br>
You could as well store an expandable string in a regular reg_sz, and expand that, but that would defeat the purpose of having the reg_expand_sz type.
