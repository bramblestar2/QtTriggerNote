#include "core/pages/AudioEditorWidget.h"
#include <sndfile.hh>
#include <QUrl>

#include <QPen>
#include <QPainter>

AudioEditorWidget::AudioEditorWidget(QWidget *parent) 
    : QWidget(parent) 
    , m_channels(0)
    , m_sampleRate(0)
{
    setFilePath("/home/jay/Music/laserShoot.wav");
}

void AudioEditorWidget::setFilePath(const QString &path) {
    m_filePath = path;
    m_samples.clear();

    processSamples();
}

void AudioEditorWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    QPen pen(Qt::red);
    pen.setWidth(2);
    QPainter painter(this);
    painter.setPen(pen);

    // if (m_decoder) {
    //     QAudioBuffer buffer = m_decoder->read();
    //     const qint16 *samples = buffer.constData<qint16>();
    //     painter.drawLine(0, height() / 2, width(), height() / 2);
    //     for (int i = 0; i < buffer.sampleCount(); ++i) {
    //         painter.drawLine(i, height() / 2 - samples[i], i, height() / 2 + samples[i]);
    //     }
    // }
}

void AudioEditorWidget::processSamples(int64_t start /* frames */, int64_t end /* frames */) {
    SndfileHandle file(m_filePath.toStdString().c_str(), SFM_READ);

    if (!file) {
        return;
    }

    m_channels = file.channels();
    m_sampleRate = file.samplerate();
    sf_count_t frames = file.frames();

    if (start == -1) {
        start = 0;
    }

    if (end == -1) {
        end = frames;
    }

    int64_t startSample = start * m_channels;
    int64_t endSample = end * m_channels;

    if (startSample < 0 || endSample > m_samples.size()) {
        return;
    }
    
    std::vector<short> rawData(frames * m_channels);
    file.read(rawData.data(), frames);

    m_samples = rawData;
    for (sf_count_t i = 0; i < frames; ++i) {
        if (m_channels == 1) {
            m_samples[i] = rawData[i];
        } else {
            int idx = i * m_channels;
            float sum = 0;
            for (int j = 0; j < m_channels; ++j) {
                sum += rawData[idx + j];
            }
            m_samples[i] = sum / m_channels;
        }
    }

    update();
}