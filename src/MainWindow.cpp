#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : KDDockWidgets::QtWidgets::MainWindow("MainWindow", KDDockWidgets::MainWindowOption_None, parent)
    , m_centralWidget(new QWidget(this))
{
    connect(&app, &QtApp::audioListChanged, this, &MainWindow::audioListChanged);
    connect(&app, &QtApp::midiBindingsChanged, this, &MainWindow::midiBindingsChanged);
    connect(&app, &QtApp::pageChanged, this, &MainWindow::pageChanged);
    connect(&app, &QtApp::midiMessage, this, &MainWindow::midiMessage);
    connect(&app, &QtApp::deviceRefresh, this, &MainWindow::deviceRefresh);



    m_listsDockWidget = new KDDockWidgets::QtWidgets::DockWidget("Lists");
    m_bindingDockWidget = new KDDockWidgets::QtWidgets::DockWidget("Bindings");

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    // centralLayout->addWidget(splitter);

    m_bindingListWidget = new BindingListWidget();
    splitter->addWidget(m_bindingListWidget);
    connect(&app, &QtApp::midiBindingsChanged, m_bindingListWidget, &BindingListWidget::update);

    m_devicesListWidget = new DevicesListWidget();
    splitter->addWidget(m_devicesListWidget);
    connect(&app, &QtApp::deviceRefresh, m_devicesListWidget, &DevicesListWidget::update);

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
    qDebug() << "Midi message: " << device->name() << " " << msg.status << " " << msg.key << " " << msg.velocity << " " << msg.timestamp << " " << msg.type();
}

void MainWindow::deviceRefresh(std::vector<MidiDevice*> devices) {
    qDebug() << "Device refresh";
}