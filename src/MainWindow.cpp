#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QMenuBar>
#include <QFileDialog>

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

    m_menuBar = new QMenuBar(this);
    this->setMenuBar(m_menuBar);
    QMenu* viewMenu = m_menuBar->addMenu("View");
    viewMenu->addAction(m_listsDockWidget->toggleAction());
    viewMenu->addAction(m_bindingDockWidget->toggleAction());
    viewMenu->addAction(m_audioDockWidget->toggleAction());

    QMenu *fileMenu = m_menuBar->addMenu("File");
    m_saveAction = fileMenu->addAction("Save");
    m_openAction = fileMenu->addAction("Open");

    connect(m_menuBar, &QMenuBar::triggered, this, &MainWindow::menuActionTriggered);

    QSplitter *splitter = new QSplitter(Qt::Vertical);

    m_listsDockWidget->setWidget(splitter);
    // centralLayout->addWidget(splitter);

    m_bindingListWidget = new BindingListWidget();
    splitter->addWidget(m_bindingListWidget);
    connect(&m_app, &QtApp::midiBindingsChanged, m_bindingListWidget, &BindingListWidget::update);

    m_devicesListWidget = new DevicesListWidget();
    splitter->addWidget(m_devicesListWidget);
    connect(&m_app, &QtApp::deviceRefresh, m_devicesListWidget, &DevicesListWidget::update);

    m_audioListWidget = new AudioListWidget();
    splitter->addWidget(m_audioListWidget);
    connect(&m_app, &QtApp::audioListChanged, m_audioListWidget, &AudioListWidget::update);

    m_bindingSetupWidget = new BindingSetupWidget();
    m_bindingDockWidget->setWidget(m_bindingSetupWidget);
    connect(&m_app, &QtApp::deviceRefresh, m_bindingSetupWidget, &BindingSetupWidget::update);
    connect(m_bindingSetupWidget, &BindingSetupWidget::bindingCreated, this, &MainWindow::createBinding);

    m_audioSetupWidget = new AudioSetupWidget();
    m_audioDockWidget->setWidget(m_audioSetupWidget);
    connect(m_audioSetupWidget, &AudioSetupWidget::audioCreated, this, &MainWindow::createAudio);

    // auto bind = app.midiBind("Novation Launchpad Pro").change_page_to(1).key(0x5F).type(libremidi::message_type::NOTE_ON).on_page(0).audio(1);
    // app.addMidiBinding(bind.build());

    connect(&m_app, &QtApp::midiMessage, this, [this](MidiDevice* device, MidiMessage msg) {
        qDebug() << "Midi Message: " << device->name() << " " << (int)msg[0] << " " << (int)msg[1] << " " << (int)msg[2];
    });
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

    m_app.createAudio(std::move(builder));
}


void MainWindow::createBinding(MidiBinding binding) {
    m_app.addMidiBinding(binding);
}

void MainWindow::menuActionTriggered(QAction *action) {
    if (action == m_saveAction) {
        qDebug() << "Save";

        QString dir = QFileDialog::getExistingDirectory(
            this,
            "Select Directory",
            QString{}
        );

        if (dir.isEmpty())
            return;

        m_app.save(dir.toStdString());
    } else if (action == m_openAction) {
        qDebug() << "Open";

        QString dir = QFileDialog::getExistingDirectory(
            this,
            "Select Directory",
            QString{}
        );

        if (dir.isEmpty())
            return;

        m_app.load(dir.toStdString());
    }
}