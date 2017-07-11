#include <QApplication>

#include <QtWidgets>

#include "mainwindow.h"
#include "imagecropwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow main;
    main.show();

    return app.exec();
}

