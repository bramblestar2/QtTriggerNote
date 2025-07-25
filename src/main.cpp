#include <QApplication>
#include <QMainWindow>
#include <QWidget>

#include <kddockwidgets/KDDockWidgets.h>

#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    KDDockWidgets::initFrontend(KDDockWidgets::FrontendType::QtWidgets);

    MainWindow window;
    window.resize(800, 600);
    window.show();

    return app.exec();
}