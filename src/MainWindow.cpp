#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : KDDockWidgets::QtWidgets::MainWindow("MainWindow", KDDockWidgets::MainWindowOption_None, parent)
    , m_centralWidget(new QWidget(this))
{
    m_listsDockWidget = new KDDockWidgets::QtWidgets::DockWidget("Lists");
    m_bindingDockWidget = new KDDockWidgets::QtWidgets::DockWidget("Bindings");
    m_audioDockWidget = new KDDockWidgets::QtWidgets::DockWidget("Audio");

    this->addDockWidget(m_listsDockWidget, KDDockWidgets::Location_OnLeft);
    this->addDockWidget(m_bindingDockWidget, KDDockWidgets::Location_OnRight);
    this->addDockWidget(m_audioDockWidget, KDDockWidgets::Location_OnBottom);

    QMenuBar *menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);
    QMenu* viewMenu = menuBar->addMenu("View");
    viewMenu->addAction(m_listsDockWidget->toggleAction());
    viewMenu->addAction(m_bindingDockWidget->toggleAction());
    viewMenu->addAction(m_audioDockWidget->toggleAction());

    QSplitter *splitter = new QSplitter(Qt::Vertical);

    m_listsDockWidget->setWidget(splitter);
    // centralLayout->addWidget(splitter);

    m_bindingListWidget = new BindingListWidget();
    splitter->addWidget(m_bindingListWidget);
    connect(&app, &QtApp::midiBindingsChanged, m_bindingListWidget, &BindingListWidget::update);

    m_devicesListWidget = new DevicesListWidget();
    splitter->addWidget(m_devicesListWidget);
    connect(&app, &QtApp::deviceRefresh, m_devicesListWidget, &DevicesListWidget::update);

    m_audioListWidget = new AudioListWidget();
    splitter->addWidget(m_audioListWidget);
    connect(&app, &QtApp::audioListChanged, m_audioListWidget, &AudioListWidget::update);

    m_bindingSetupWidget = new BindingSetupWidget();
    m_bindingDockWidget->setWidget(m_bindingSetupWidget);

    m_audioSetupWidget = new AudioSetupWidget();
    m_audioDockWidget->setWidget(m_audioSetupWidget);
    connect(m_audioSetupWidget, &AudioSetupWidget::audioCreated, this, &MainWindow::createAudio);

    auto bind = app.midiBind("Novation Launchpad Pro").change_page_to(1).key(0x5F).type(MidiMessage::NoteOn).on_page(0);
    app.addMidiBinding(bind.build());
    app.midiManager().refresh();
}


void MainWindow::createAudio(PlayerInfo info) {
    qDebug() << "Creating Audio with File: " << info.file;

    AudioBuilder builder;
    builder.set_file(info.file);
    builder.set_loop(info.settings.looped);
    builder.set_speed(info.settings.speed);
    builder.set_volume(info.settings.volume);
    builder.set_fade_in(info.settings.fade_in);
    builder.set_fade_out(info.settings.fade_out);
    builder.set_start(info.settings.start_time);
    builder.set_end(info.settings.end_time);

    app.createAudio(std::move(builder));
}