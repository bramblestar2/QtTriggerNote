#pragma once

#include <QObject>
#include <App/App.h>

class QtApp : public QObject, public App
{
    Q_OBJECT
public:
    QtApp();

signals:
    void midiBindingsChanged();
    void audioListChanged();
    void pageChanged(int page);
    void midiMessage(MidiDevice* device, MidiMessage msg);
};