/*************************************************************************
 *
 * This file is part of tgtsml_tools
 *
 * Create Date 2020/10/18
 *
 * Copyright: tgtsml
 *
 * Contact email: time_forget@outlook.com
 *
 * GitHub: www.github.com/tgtsml
 *
 *************************************************************************/

#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showFullScreen();
    return a.exec();
}
