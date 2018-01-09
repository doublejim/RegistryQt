#include <QCoreApplication>
#include "RegistryQt/tests.h"

/* Original author: Nikolai S | https://github.com/doublejim
 *
 * This file is released under the MIT license.
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RunTests();

    return 0;
}
