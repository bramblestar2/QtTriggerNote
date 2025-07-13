#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include <QPointF>
#include <QPainter>
#include <thread>
#include <QPainterPath>
#include <QPixmap>

#include <vector>
#include "core/components/AudioLoader.h"

class AudioEditorWidget : public QWidget {
    Q_OBJECT
public:
    AudioEditorWidget(QWidget *parent = nullptr);

    void setFilePath(const QString &path);
    void setScroll(float scroll);
    void setZoom(float zoom);

signals:
    void selectionChanged(double start, double end);
    void fileDropped(QString filepath);

private slots:
    void onLoaderLoaded();

private:
    void paintEvent(QPaintEvent *event) override;
    void drawGrid(QPainter& painter, QPaintEvent *event);
    void drawVisualizer(QPainter& painter, QPaintEvent *event);
    void drawSelection(QPainter& painter, QPaintEvent *event);
    void drawMouseCursor(QPainter& painter, QPaintEvent *event);

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    double mouseToView(double x);
    int64_t mouseToTime(int x);
    int64_t viewToTime(double x);
    int64_t mouseToSample(double x);

    void buildEnvelope();



    std::vector<short> m_samples;
    int m_channels;
    int m_sampleRate;

    QPixmap m_cachedAudioImage;
    // QPainterPath m_cachedAudioPath;

    QString m_filePath;

    AudioLoader *m_loader = nullptr;


    double m_startSelection;
    double m_endSelection;
    
    bool m_mouseDown;
    double m_mouseX;

    float m_scroll;
    float m_zoom;
};