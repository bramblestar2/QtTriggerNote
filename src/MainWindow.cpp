#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(new QWidget(this))
{
    connect(&app, &QtApp::audioListChanged, this, &MainWindow::audioListChanged);
    connect(&app, &QtApp::midiBindingsChanged, this, &MainWindow::midiBindingsChanged);
    connect(&app, &QtApp::pageChanged, this, &MainWindow::pageChanged);
    connect(&app, &QtApp::midiMessage, this, &MainWindow::midiMessage);

    setCentralWidget(m_centralWidget);
    auto centralLayout = new QVBoxLayout(m_centralWidget);
    m_centralWidget->setLayout(centralLayout);

    m_bindingListWidget = new BindingListWidget(m_centralWidget);
    connect(&app, &QtApp::midiBindingsChanged, m_bindingListWidget, &BindingListWidget::update);

    auto bind = app.midiBind("Novation Launchpad Pro").change_page_to(1).key(0x5F).type(MidiMessage::NoteOn).on_page(0);
    app.addMidiBinding(bind.build());
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