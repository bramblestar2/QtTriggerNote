#include "core/QtApp.h"

QtApp::QtApp() {
    App::onAudioListChanged([this]() { this->audioListChanged(); });
    App::onMidiBindingsChanged([this]() { this->midiBindingsChanged(this->getMidiBindingPages()); });
    App::onPageChanged([this](int page) { this->pageChanged(page); });
    App::setMidiCallback([this](MidiDevice* device, MidiMessage msg) { this->midiMessage(device, msg); });
    App::onDeviceRefresh([this]() { this->deviceRefresh(this->midiManager().getAvailableDevices()); });
}