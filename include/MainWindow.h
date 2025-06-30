#pragma once

#include <QMainWindow>
#include <QWidget>
#include <kddockwidgets/MainWindow.h>
#include <kddockwidgets/DockWidget.h>

#include "core/QtApp.h"
#include "core/components/BindingListWidget.h"
#include "core/components/DevicesListWidget.h"
#include "core/components/BindingSetupWidget.h"
#include "core/components/AudioSetupWidget.h"
#include "core/components/AudioListWidget.h"

class MainWindow : public KDDockWidgets::QtWidgets::MainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void createAudio(PlayerInfo info);

private:
    QWidget *m_centralWidget;

    BindingListWidget *m_bindingListWidget = nullptr;
    DevicesListWidget *m_devicesListWidget = nullptr;
    BindingSetupWidget *m_bindingSetupWidget = nullptr;
    AudioSetupWidget *m_audioSetupWidget = nullptr;
    AudioListWidget *m_audioListWidget = nullptr;

    KDDockWidgets::QtWidgets::DockWidget *m_listsDockWidget = nullptr;
    KDDockWidgets::QtWidgets::DockWidget *m_bindingDockWidget = nullptr;
    KDDockWidgets::QtWidgets::DockWidget *m_audioDockWidget = nullptr;

    QtApp app;
};