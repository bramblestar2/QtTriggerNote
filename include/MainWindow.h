#pragma once

#include <QMainWindow>
#include <QWidget>
#include <kddockwidgets/MainWindow.h>

#include "core/QtApp.h"
#include "core/components/BindingListWidget.h"
#include "core/components/DevicesListWidget.h"  

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void audioListChanged();
    void midiBindingsChanged();
    void pageChanged(int page);
    void midiMessage(MidiDevice* device, MidiMessage msg);
    void deviceRefresh(std::vector<MidiDevice*> devices);

private:
    QWidget *m_centralWidget;

    BindingListWidget *m_bindingListWidget = nullptr;
    DevicesListWidget *m_devicesListWidget = nullptr;

    QtApp app;
};