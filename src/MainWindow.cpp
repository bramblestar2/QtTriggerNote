#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : KDDockWidgets::QtWidgets::MainWindow("QtTriggerNoteWindow", KDDockWidgets::MainWindowOption_None, parent)
    , m_centralWidget(new QWidget(this))
{
}
