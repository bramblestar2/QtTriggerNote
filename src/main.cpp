#include <QApplication>
#include <QMainWindow>
#include <QWidget>

#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    window.resize(800, 600);

    return app.exec();
}