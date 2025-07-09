#pragma once

#include <QMainWindow>
#include <QMenuBar>

#include <kddockwidgets/MainWindow.h>
#include <kddockwidgets/DockWidget.h>

#include "core/pages/AudioEditorWidget.h"
#include "core/QtApp.h"

class ProjectWindow : public KDDockWidgets::QtWidgets::MainWindow {
public:
    ProjectWindow(QtApp &app, QWidget *parent = nullptr);
private:
    void setupMenuBar();
    void setupAudioEditor();
    void setupDocks();
    void setupConnnections();

    QMenuBar *m_menuBar = nullptr;
    QMenu *m_viewMenu = nullptr;

    KDDockWidgets::QtWidgets::DockWidget *m_audioEditorDock = nullptr;
    KDDockWidgets::QtWidgets::DockWidget *m_pageTwo = nullptr;

    AudioEditorWidget *m_audioEditorWidget = nullptr;
};