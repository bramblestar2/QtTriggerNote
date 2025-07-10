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
    // setFilePath("C:\\Users\\thega\\Downloads\\brackeys_platformer_assets\\brackeys_platformer_assets\\sounds\\power_up.wav");
}

void AudioEditorWidget::setFilePath(const QString &path) {
    m_filePath = path;
    m_samples.clear();
    m_loader = new AudioLoader(m_filePath, this);
    connect(m_loader, &AudioLoader::loaded, this, &AudioEditorWidget::onLoaderLoaded);



    // processSamples();
}

void AudioEditorWidget::onLoaderLoaded() {
    processSamples();
}

void AudioEditorWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    if (m_samples.empty()) {
        return;
    }

    QPen pen(Qt::red);
    pen.setWidth(2);
    QPainter painter(this);
    painter.setPen(pen);

    // std::vector<QPointF> points;

    // painter.drawPolyline(points.data(), m_samples.size());
}

void AudioEditorWidget::processSamples(int64_t start /* sample */, int64_t end /* sample */) {
    if (!m_loader) return;
    if (m_loader->samples().empty()) return;


    m_channels = m_loader->channels();
    m_sampleRate = m_loader->sampleRate();

    qDebug() << m_channels << m_sampleRate;

    if (start == -1) {
        start = 0;
    }

    if (end == -1) {
        end = m_loader->samples().size();
    }

    int64_t startSample = start;
    int64_t endSample = end;

    if (startSample < 0 || endSample > m_loader->samples().size()) {
        return;
    }

    const std::vector<short>& rawData = m_loader->samples();

    m_samples = std::vector<short>(rawData.begin() + startSample, rawData.begin() + endSample);

    for (int i = 0; i < m_samples.size(); ++i) {
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
    
    // for (sf_count_t i = 0; i < ; ++i) {
    //     if (m_channels == 1) {
    //         m_samples[i] = rawData[i];
    //     } else {
    //         int idx = i * m_channels;
    //         float sum = 0;
    //         for (int j = 0; j < m_channels; ++j) {
    //             sum += rawData[idx + j];
    //         }
    //         m_samples[i] = sum / m_channels;
    //     }
    // }

    update();
}