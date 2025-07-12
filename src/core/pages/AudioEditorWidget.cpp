#include "core/pages/AudioEditorWidget.h"
#include <sndfile.hh>
#include <QUrl>

#include <QPen>
#include <QPainter>

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

    m_loader = new AudioLoader(m_filePath, this);
    connect(m_loader, &AudioLoader::loaded, this, &AudioEditorWidget::onLoaderLoaded);
}

void AudioEditorWidget::setScroll(float scroll) {
    scroll = qBound(0.0f, scroll, 1.0f);

    if (!qFuzzyCompare(scroll, m_scroll)) {
        m_scroll = scroll;
        buildEnvelope();
        update();
    }
}

void AudioEditorWidget::setZoom(float zoom) {
    zoom = qMax(1.0f, zoom);

    if (!qFuzzyCompare(zoom, m_zoom)) {
        m_zoom = zoom;
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


double AudioEditorWidget::mouseToView(double x) {
    double W = double(width());
    if (W <= 0) return 0;

    double effectiveZoom = qMax(m_zoom, 1e-9);
    double visibleWidthInView = W / effectiveZoom;

    double scrollableWidth = qMax(0.0, W - visibleWidthInView);
    
    double viewLeft = m_scroll * scrollableWidth;

    return viewLeft + (static_cast<double>(x) / effectiveZoom);
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

int64_t AudioEditorWidget::viewToTime(double x) {
    if (m_samples.empty() || m_sampleRate <= 0)
        return 0.0;

    double W = double(width());
    if (W <= 0.0)
        return 0.0;

    double effectiveZoom = std::max(1.0, double(m_zoom));

    int64_t totalSamples = int64_t(m_samples.size());
    int64_t windowSamples = int64_t(totalSamples / effectiveZoom);
    int64_t maxOffset = std::max<int64_t>(0, totalSamples - windowSamples);

    int64_t startSample = int64_t(m_scroll * double(maxOffset));

    double contentWindowWidth = W / effectiveZoom;
    double contentPos = mouseToView(x);

    double fracInWindow = contentPos / contentWindowWidth;
    fracInWindow = qBound(0.0, fracInWindow, 1.0);

    int64_t sampleIndex = startSample + int64_t(fracInWindow * windowSamples);
    sampleIndex = std::clamp(sampleIndex, int64_t(0), totalSamples - 1);

    double timeSec = double(sampleIndex) / double(m_sampleRate);
    return timeSec * 1000.0;
}


void AudioEditorWidget::buildEnvelope() {
    const int W = width();
    const int H = height();
    if (W <= 0 || H <= 0 || m_samples.empty()) {
        m_cachedAudioPath = QPainterPath();
        return;
    }

    const int64_t totalSamples  = int64_t(m_samples.size());
    const double zoom           = std::max(1.0f, m_zoom);
    const int64_t windowSamples = int64_t(totalSamples / zoom);
    const int64_t maxOffset     = std::max(int64_t(0), totalSamples - windowSamples);
    const int64_t startSample   = int64_t(m_scroll * maxOffset);
    const double spp            = double(windowSamples) / double(W);
    const double midY           = H * 0.5;

    QPainterPath path;
    std::vector<float> amplitudes(W);

    // Compute RMS amplitudes for each column
    for (int x = 0; x < W; ++x) {
        int64_t s = startSample + int64_t(std::floor(x * spp));
        int64_t e = startSample + int64_t(std::floor((x + 1) * spp));
        if (e <= s) e = s + 1;
        e = std::min(e, startSample + windowSamples);

        double sumSq = 0;
        for (int64_t i = s; i < e; ++i) {
            double v = m_samples[i] / 32768.0;
            sumSq += v * v;
        }
        double rms = std::sqrt(sumSq / double(e - s));
        amplitudes[x] = rms * midY;
    }

    // Build path (top + bottom in reverse)
    path.moveTo(0, midY - amplitudes[0]);
    for (int x = 1; x < W; ++x) {
        path.lineTo(x, midY - amplitudes[x]);
    }
    for (int x = W-1; x >= 0; --x) {
        path.lineTo(x, midY + amplitudes[x]);
    }
    path.closeSubpath();

    m_cachedAudioPath = path;
}


void AudioEditorWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_startSelection =
        m_endSelection   = mouseToView(event->position().x());
        m_mouseDown = true;
    }

    QWidget::mousePressEvent(event);
}

void AudioEditorWidget::mouseMoveEvent(QMouseEvent *event) {
    double prevX = m_mouseX;

    m_mouseX = event->position().x();

    if (m_mouseDown) {
        m_endSelection = mouseToView(event->position().x());
    }
    
    update();

    QWidget::mouseMoveEvent(event);
}

void AudioEditorWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_endSelection = mouseToView(event->position().x());
        m_mouseDown = false;

        int64_t start = viewToTime(m_startSelection);
        int64_t end   = viewToTime(m_endSelection);

        if (start > end) {
            std::swap(start, end);
        }

        emit selectionChanged(start, end);
    }
    
    QWidget::mouseReleaseEvent(event);
}

void AudioEditorWidget::wheelEvent(QWheelEvent *event) {
    QWidget::wheelEvent(event);

    int delta = event->angleDelta().y();
    if (delta == 0) return;

    bool ctrl = event->modifiers() & Qt::ControlModifier;
    float steps = delta / 120.0f;
    float factor = std::pow(1.2f, steps);

    if (ctrl) {
        float mouseX = event->position().x();

        float oldContentW = width() * m_zoom;
        float oldScrollable = qMax(0.0f, oldContentW - width());
        float oldScrollOffset = m_scroll * oldScrollable;

        float contentMouseX = oldScrollOffset + mouseX;

        float newZoom = qMax(1.0f, m_zoom * factor);
        float newContentW = width() * newZoom;
        float newScrollable = qMax(0.0f, newContentW - width());

        float newScrollOffset = contentMouseX * (newZoom / m_zoom) - mouseX;
        newScrollOffset = qBound(0.0f, newScrollOffset, newScrollable);

        m_zoom   = newZoom;
        m_scroll = (newScrollable > 0 ? newScrollOffset / newScrollable : 0.0f);

        buildEnvelope();
        update();
    }
    else {
        float scrollStep = 0.1f * (1.0f / m_zoom);
        setScroll(m_scroll - steps * scrollStep);
    }
}



void AudioEditorWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawGrid(painter, event);
    drawVisualizer(painter, event);
    drawSelection(painter, event);
    drawMouseCursor(painter, event);
}


void AudioEditorWidget::drawGrid(QPainter& painter, QPaintEvent *event) {
    return;

    painter.save();

    QPen gridPen(Qt::gray);
    gridPen.setWidth(1);
    gridPen.setCapStyle(Qt::RoundCap);
    painter.setPen(gridPen);

    QColor gridColor = QColor(255,255,255, 20);
    QBrush gridBrush(gridColor);
    painter.setBrush(gridBrush);

    int H = height();

    painter.restore();
}

void AudioEditorWidget::drawVisualizer(QPainter& painter, QPaintEvent *event) {
    QPen pen(Qt::white);
    pen.setWidth(1);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    QColor fillColor = QColor(255,255,255, 60);
    QBrush brush(fillColor);
    painter.setBrush(brush);
    
    painter.drawPath(m_cachedAudioPath);
}

void AudioEditorWidget::drawSelection(QPainter& painter, QPaintEvent *event) {
    float contentWidth = width() * m_zoom;
    float maxScrollOffset = qMax(0.0f, contentWidth - width());
    float scrollOffset = m_scroll * maxScrollOffset;

    if (m_mouseDown || m_startSelection != m_endSelection) {
        float v0 = m_startSelection  * m_zoom - scrollOffset;
        float v1 = m_endSelection * m_zoom - scrollOffset;

        float x0 = qBound(0.0f, qMin(v0, v1), float(width()));
        float x1 = qBound(0.0f, qMax(v0, v1), float(width()));

        QColor selCol(255, 255, 255, 50);
        painter.save();
        painter.setCompositionMode(QPainter::CompositionMode_Difference);
        painter.fillRect(x0, 0, x1-x0, height(), selCol);
        painter.restore();
    }
}

void AudioEditorWidget::drawMouseCursor(QPainter& painter, QPaintEvent *event) {
    painter.setPen(QPen(Qt::white, 1, Qt::DashLine));
    float mouseX = mouseToView(m_mouseX);
    painter.drawLine(m_mouseX, 0, m_mouseX, height());
}




void AudioEditorWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if (!m_samples.empty()) {

        // - recompute start/end selections -
        // float v0 = m_startSelection;
        // float v1 = m_endSelection;
        // m_startSelection = qBound(0.0f, v0, 1.0f);
        // m_endSelection   = qBound(0.0f, v1, 1.0f);

        buildEnvelope();
    }
}