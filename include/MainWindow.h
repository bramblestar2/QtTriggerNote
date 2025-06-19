#pragma once

#include <QMainWindow>
#include <QWidget>
#include <kddockwidgets/MainWindow.h>

#include "core/QtApp.h"
#include "core/components/BindingListWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void audioListChanged();
    void midiBindingsChanged();
    void pageChanged(int page);
    void midiMessage(MidiDevice* device, MidiMessage msg);

private:
    QWidget *m_centralWidget;

    BindingListWidget *m_bindingListWidget;

    QtApp app;
};