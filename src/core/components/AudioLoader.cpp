#include "core/components/AudioLoader.h" 
#include <sndfile.hh>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

struct DataBlock {
    std::vector<short> samples;
    int channels;
    int sampleRate;
};

AudioLoader::AudioLoader(const std::vector<short>& pcm, int channels, int sampleRate, QObject *parent) 
    : QObject(parent)
    , m_samples(pcm)
    , m_channels(channels)
    , m_sampleRate(sampleRate) 
{
    load(pcm, channels, sampleRate);
}

AudioLoader::AudioLoader(QString filepath, QObject *parent)
    : QObject(parent)
{
    QFuture<DataBlock> future = QtConcurrent::run([filepath](){
        SndfileHandle file(filepath.toStdString().c_str(), SFM_READ);
        if (!file) {
            return DataBlock();
        }
        int channels = file.channels();
        int sampleRate = file.samplerate();
        sf_count_t frames = file.frames();
        
        std::vector<short> pcm(frames * channels);
        file.read(pcm.data(), frames);

        return DataBlock(pcm, channels, sampleRate);
    });

    QFutureWatcher<DataBlock> *watcher = new QFutureWatcher<DataBlock>(this);
    connect(watcher, &QFutureWatcher<DataBlock>::finished, this, [this, watcher](){
        DataBlock d = watcher->result();
        load(d.samples, d.channels, d.sampleRate);
        watcher->deleteLater();
    });

    watcher->setFuture(future);
}

const std::vector<short>& AudioLoader::samples() const {
    return m_samples;
}

int AudioLoader::channels() const noexcept {
    return m_channels;
}

int AudioLoader::sampleRate() const noexcept {
    return m_sampleRate;
}

int64_t AudioLoader::duration() const noexcept {
    return m_duration;
}

void AudioLoader::load(const std::vector<short>& pcm, int channels, int sampleRate) {
    m_samples = pcm;
    m_channels = channels;
    m_sampleRate = sampleRate;
    m_duration = pcm.size() / (channels * sampleRate);

    emit loaded();
}