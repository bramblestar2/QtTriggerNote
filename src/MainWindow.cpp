#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : KDDockWidgets::QtWidgets::MainWindow("QtTriggerNoteWindow", KDDockWidgets::MainWindowOption_None, parent)
    , m_centralWidget(new QWidget(this))
{
    connect(&app, &QtApp::audioListChanged, this, &MainWindow::audioListChanged);
    connect(&app, &QtApp::midiBindingsChanged, this, &MainWindow::midiBindingsChanged);
    connect(&app, &QtApp::pageChanged, this, &MainWindow::pageChanged);
    connect(&app, &QtApp::midiMessage, this, &MainWindow::midiMessage);

    app.midiManager().refresh();
}

void MainWindow::audioListChanged() {
    qDebug() << "Audio list changed";
}
void MainWindow::midiBindingsChanged() {
    qDebug() << "Midi bindings changed";
}
void MainWindow::pageChanged(int page) {
    qDebug() << "Page changed to " << page;
}
void MainWindow::midiMessage(MidiDevice* device, MidiMessage msg) {
    qDebug() << "Midi message: " << device->name() << " " << msg.status << " " << msg.key << " " << msg.velocity;
}