#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.setFixedSize(900,675);
    w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
    w.setWindowTitle("Card Match Game");
    w.show();


    return app.exec();
}
