#include <QCoreApplication>
#include <clocale>
#include <Windows.h>

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "");
    Server s;
    return a.exec();
}
