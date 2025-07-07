#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QMenuBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : KDDockWidgets::QtWidgets::MainWindow("MainWindow", KDDockWidgets::MainWindowOption_None, parent)
{
}