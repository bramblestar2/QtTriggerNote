#include "core/windows/ProjectWindow.h"

ProjectWindow::ProjectWindow(QWidget *parent)
    : KDDockWidgets::QtWidgets::MainWindow("ProjectWindow", KDDockWidgets::MainWindowOption_None, parent)
{
    this->resize(800, 600);

    this->setWindowIcon(QIcon(":/icons/icon.png"));
}