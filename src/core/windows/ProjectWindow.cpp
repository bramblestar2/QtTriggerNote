#include "core/windows/ProjectWindow.h"


ProjectWindow::ProjectWindow(QtApp &app, QWidget *parent)
    : KDDockWidgets::QtWidgets::MainWindow("ProjectWindow", KDDockWidgets::MainWindowOption_None, parent)
{
    this->resize(800, 600);
    this->setWindowIcon(QIcon(":/icons/icon.png"));

    
    setupMenuBar();
    setupDocks();
    setupConnnections();
}


void ProjectWindow::setupMenuBar() {
    m_menuBar = new QMenuBar(this);
    m_viewMenu = m_menuBar->addMenu("Views");


    this->setMenuBar(m_menuBar);
}


void ProjectWindow::setupAudioEditor() {
    m_audioEditorWidget = new AudioEditorWidget();
    m_audioEditorDock = new KDDockWidgets::QtWidgets::DockWidget("Audio Editor", KDDockWidgets::DockWidgetOption_None);
    m_audioEditorDock->setWidget(m_audioEditorWidget);

    
    this->addDockWidget(m_audioEditorDock, KDDockWidgets::Location_OnRight);


    m_viewMenu->addAction(m_audioEditorDock->toggleAction());
}


void ProjectWindow::setupDocks() {
    setupAudioEditor();
}


void ProjectWindow::setupConnnections() {
    
}