#include "core/pages/AudioEditorWidget.h"
#include <sndfile.hh>
#include <QUrl>

#include <QPen>
#include <QPainter>
#include <QPainterPath>

AudioEditorWidget::AudioEditorWidget(QWidget *parent) 
    : QWidget(parent) 
    , m_channels(0)
    , m_sampleRate(0)
    , m_scroll(0)
    , m_zoom(1)
    , m_mouseX(0)
    , m_startSelection(0)
    , m_endSelection(0)
    , m_mouseDown(false)
{
    setFilePath("/home/jay/Music/C418 - The Fighter.mp3");

    setMouseTracking(true);
}

void AudioEditorWidget::setFilePath(const QString &path) {
    m_filePath = path;

    if (m_loader) {
        m_loader->disconnect(this);
        m_loader->deleteLater();
    }

    m_samples.clear();
    m_envelope.clear();

    m_loader = new AudioLoader(m_filePath, this);
    connect(m_loader, &AudioLoader::loaded, this, &AudioEditorWidget::onLoaderLoaded);
}

void AudioEditorWidget::setScroll(float scroll) {
    float prev_scroll = m_scroll;

    m_scroll = qBound(0.0f, scroll, 1.0f);

    if (!m_samples.empty() && prev_scroll != m_scroll) {
        buildEnvelope();
        update();
    }
}

void AudioEditorWidget::setZoom(float zoom) {
    float prev_zoom = m_zoom;

    m_zoom = qMax(1.0f, zoom);

    if (!m_samples.empty() && prev_zoom != m_zoom) {
        buildEnvelope();
        update();
    }
}

void AudioEditorWidget::onLoaderLoaded() {
    m_channels = m_loader->channels();
    m_sampleRate = m_loader->sampleRate();
    const auto &pcm = m_loader->samples();

    m_samples.resize(pcm.size() / m_channels);
    for (size_t i = 0; i < m_samples.size(); ++i) {
        if (m_channels == 1) {
            m_samples[i] = pcm[i];
        } else {
            int sum = 0;
            for (int j = 0; j < m_channels; ++j) {
                sum += pcm[i * m_channels + j];
            }
            m_samples[i] = sum / m_channels;
        }
    }
    
    buildEnvelope();
    update();
}


double AudioEditorWidget::mouseToView(int x) {
    double W     = double(width());
    if (W <= 0) return 0;

    double viewW = W / m_zoom;
    double xOff  = (W - viewW) * m_scroll;
    double xf    = double(x) - xOff;
    double v     = xf / viewW;
    return qBound(0.0f, v, 1.0f);
}

int64_t AudioEditorWidget::mouseToTime(int x) {
    if (m_samples.empty()) return 0;

    int64_t totalSamples = int64_t(m_samples.size());
    int64_t windowSamples = int64_t(totalSamples / m_zoom);
    int64_t maxOffset     = totalSamples - windowSamples;
    int64_t startSample   = int64_t(m_scroll * float(maxOffset));

    float viewFrac = mouseToView(x);
    int64_t inWin  = int64_t(viewFrac * windowSamples);

    int64_t idx = startSample + inWin;
    return std::clamp(idx, int64_t(0), totalSamples - 1);
}


void AudioEditorWidget::buildEnvelope() {
    const int W = width();
    if (W <= 0 || m_samples.empty()) {
        m_envelope.clear();
        return;
    }

    const int64_t totalSamples  = int64_t(m_samples.size());
    const double zoom           = std::max(1.0f, m_zoom);
    const int64_t windowSamples = int64_t(totalSamples / zoom);
    const int64_t maxOffset     = std::max(int64_t(0), totalSamples - windowSamples);
    const int64_t startSample   = int64_t(m_scroll * maxOffset);

    const double spp = double(windowSamples) / double(W);

    m_envelope.resize(W * 2);
    const double midY = height() * 0.5;

    for (int x = 0; x < W; ++x) {
        int64_t s = startSample + int64_t(std::floor(x * spp));
        int64_t e = startSample + int64_t(std::floor((x + 1) * spp));
        if (e <= s) e = s + 1;
        e = std::min<int64_t>(e, startSample + windowSamples);

        double sumSq = 0;
        for (int64_t i = s; i < e; ++i) {
            double v = m_samples[i] / 32768.0;
            sumSq += v * v;
        }
        double rms = std::sqrt(sumSq / double(e - s));
        double A   = rms * midY;

        m_envelope[2*x    ] = QPointF(x,        midY - A);
        m_envelope[2*x + 1] = QPointF(x,        midY + A);
    }
}


void AudioEditorWidget::mousePressEvent(QMouseEvent *event) {
    m_startSelection =
    m_endSelection   = mouseToView(int(event->position().x())) * width();
    m_mouseDown = true;
    
    QWidget::mousePressEvent(event);
}

void AudioEditorWidget::mouseMoveEvent(QMouseEvent *event) {
    double prevX = m_mouseX;

    m_mouseX = event->position().x();

    if (m_mouseDown) {
        // SELECTION WHEN SCROLLED/VIEWED IS BROKEN
        m_endSelection = mouseToView(int(event->position().x())) * width();
    }
    
    update();

    QWidget::mouseMoveEvent(event);
}

void AudioEditorWidget::mouseReleaseEvent(QMouseEvent *event) {
    m_endSelection = mouseToView(int(event->position().x())) * width();
    m_mouseDown = false;
    
    QWidget::mouseReleaseEvent(event);
}

void AudioEditorWidget::wheelEvent(QWheelEvent *event) {
    QWidget::wheelEvent(event);

    if (event->angleDelta().y() == 0) {
        return;
    }

    bool ctrl = event->modifiers() & Qt::ControlModifier;
    float steps = event->angleDelta().y() / 120.0f;
    float factor = std::pow(1.2f, steps);
    
    if (ctrl) {
        float x = event->position().x();
        float viewFrac = x / float(width());

        float absFrac = m_scroll + viewFrac * (1.0f / m_zoom);

        float newZoom = m_zoom * factor;
        newZoom = qMax(1.0f, newZoom);

        float newWindowFrac = 1.f / newZoom;
        float newScroll = absFrac - viewFrac * newWindowFrac;

        newScroll = qBound(0.0f, newScroll, 1.0f - newWindowFrac);
        
        m_zoom = newZoom;
        m_scroll = newScroll;

        buildEnvelope();
        update();
    } else {
        float scrollStep = 0.1f * (1.0f / m_zoom);
        setScroll(m_scroll - steps * scrollStep);
    }
}


void AudioEditorWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    if (m_envelope.empty()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::white);
    pen.setWidth(1);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    QColor fillColor = QColor(255,255,255, 60);
    QBrush brush(fillColor);
    painter.setBrush(brush);

    // — apply zoom & scroll —
    int W     = width();
    int viewW = qMax(1, int(W));
    int xOff   = int(W - viewW);

    std::vector<QPointF> top, bottom;
    top.reserve(viewW);
    bottom.reserve(viewW);
    for (int i = 0; i < viewW*2; i += 2) {
        top   .push_back(m_envelope[i]);
        bottom.push_back(m_envelope[i+1]);
    }

    QPainterPath fillPath;
    if (!top.empty()) {
        fillPath.moveTo(top[0]);

        for (size_t i = 1; i < top.size(); ++i)
            fillPath.lineTo(top[i]);

        for (int i = bottom.size()-1; i >= 0; --i)
            fillPath.lineTo(bottom[i]);

        fillPath.closeSubpath();
    }

    painter.drawPath(fillPath);

    painter.setPen(QPen(Qt::white, 1, Qt::DashLine));
    float mouseX = mouseToView(m_mouseX);
    painter.drawLine(m_mouseX, 0, m_mouseX, height());

    if (m_mouseDown || m_startSelection != m_endSelection) {
        float v0 = mouseToView(int(m_startSelection));
        float v1 = mouseToView(int(m_endSelection));
        float x0 = qBound(0.0f, qMin(v0, v1) * width(), float(width()));
        float x1 = qBound(0.0f, qMax(v0, v1) * width(), float(width()));

        QColor selCol(255, 255, 255, 60);
        painter.fillRect(x0, 0, x1-x0, height(), selCol);


    }
}



void AudioEditorWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if (!m_samples.empty()) {

        // - recompute start/end selections -
        float v0 = mouseToView(int(m_startSelection));
        float v1 = mouseToView(int(m_endSelection));
        m_startSelection = qBound(0.0f, v0, 1.0f);
        m_endSelection   = qBound(0.0f, v1, 1.0f);


        buildEnvelope();
    }
}