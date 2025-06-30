#include "core/QtApp.h"

QtApp::QtApp() {
    App::onAudioListChanged([this]() { this->on_audioListChanged(this->audioEngine().list()); });
    App::onMidiBindingsChanged([this]() { this->on_midiBindingsChanged(this->getMidiBindingPages()); });
    App::onPageChanged([this](int page) { this->on_pageChanged(page); });
    App::setMidiCallback([this](MidiDevice* device, MidiMessage msg) { this->on_midiMessage(device, msg); });
    App::onDeviceRefresh([this]() { this->on_deviceRefresh(this->midiManager().getAvailableDevices()); });
}

void QtApp::on_audioListChanged(std::vector<PlayerEntry> audioList) {
    m_audioList.clear();
    for (auto& entry : audioList) {
        m_audioList.append(QString::fromStdString(entry.info.file));
    }
    m_audioListModel.setStringList(m_audioList);

    emit audioListChanged(audioList);
}

void QtApp::on_midiBindingsChanged(std::map<int, std::vector<MidiBinding>> bindings) {
    m_bindingList.clear();
    for (auto& [page, bindings] : bindings) {
        for (auto& binding : bindings) {
            m_bindingList.append(QString::number(binding.id) + " " + QString::fromStdString(binding.deviceName) + " " + QString::number(binding.eventType) + " " + QString::number(binding.key));
        }
    }
    m_bindingListModel.setStringList(m_bindingList);

    emit midiBindingsChanged(bindings);
}

void QtApp::on_pageChanged(int page) {
    m_deviceList.clear();
    for (MidiDevice* device : this->midiManager().getAvailableDevices()) {
        m_deviceList.append(QString::fromStdString(device->name()));
    }
    m_deviceListModel.setStringList(m_deviceList);

    emit pageChanged(page);
}

void QtApp::on_midiMessage(MidiDevice* device, MidiMessage msg) {
    emit midiMessage(device, msg);
}

void QtApp::on_deviceRefresh(std::vector<MidiDevice*> devices) {
    m_deviceList.clear();
    for (MidiDevice* device : devices) {
        m_deviceList.append(QString::fromStdString(device->name()));
    }
    m_deviceListModel.setStringList(m_deviceList);

    emit deviceRefresh(devices);
}