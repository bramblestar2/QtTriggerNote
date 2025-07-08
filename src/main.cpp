#include <QApplication>
#include <QMainWindow>
#include <QWidget>

#include "MainWindow.h"
#include "core/Application.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setOrganizationName("Jay");
    app.setApplicationName("TriggerNote");

    Application appInstance;

    return app.exec();
}