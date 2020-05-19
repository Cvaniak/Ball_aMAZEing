#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include <QtDebug>
#include <box2d.h>
#include <QKeyEvent>
#include <QBrush>
#include <QList>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "mainwindow.h"

// 360x640 px = 36x64m


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow w;

    w.setWindowTitle("Ball aMAZEing");
    w.show();


    return app.exec();
}

/*#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#ifndef QT_NO_OPENGL
#include "mainwidget.h"
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("cube");
    app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
    MainWidget widget;
    widget.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}*/
