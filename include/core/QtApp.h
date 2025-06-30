#pragma once

#include <QObject>
#include <App/App.h>
#include <QStringListModel>

class QtApp : public QObject, public App
{
    Q_OBJECT
public:
    QtApp();

    const QAbstractListModel* audioList() const { return &m_audioListModel; }
    const QAbstractListModel* bindingList() const { return &m_bindingListModel; }
    const QAbstractListModel* deviceList() const { return &m_deviceListModel; }

signals:
    void midiBindingsChanged(const std::map<int, std::vector<MidiBinding>>& bindings);
    void audioListChanged(std::vector<PlayerEntry> audioList);
    void pageChanged(int page);
    void midiMessage(MidiDevice* device, MidiMessage msg);
    void deviceRefresh(std::vector<MidiDevice*> devices);
    
private:
    void on_midiBindingsChanged(std::map<int, std::vector<MidiBinding>> bindings);
    void on_audioListChanged(std::vector<PlayerEntry> audioList);
    void on_pageChanged(int page);
    void on_midiMessage(MidiDevice* device, MidiMessage msg);
    void on_deviceRefresh(std::vector<MidiDevice*> devices);

    QStringList m_audioList;
    QStringList m_bindingList;
    QStringList m_deviceList;

    QStringListModel m_audioListModel;
    QStringListModel m_bindingListModel;
    QStringListModel m_deviceListModel;
};