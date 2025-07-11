#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPointF>
#include <thread>

#include <vector>
#include "core/components/AudioLoader.h"

class AudioEditorWidget : public QWidget {
    Q_OBJECT
public:
    AudioEditorWidget(QWidget *parent = nullptr);

    void setFilePath(const QString &path);
    void setScroll(float scroll);
    void setZoom(float zoom);

private slots:
    void onLoaderLoaded();

private:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    double mouseToView(int x);
    int64_t mouseToTime(int x);

    void buildEnvelope();

    std::vector<short> m_samples;
    int m_channels;
    int m_sampleRate;

    std::vector<QPointF> m_envelope;

    QString m_filePath;

    AudioLoader *m_loader = nullptr;


    double m_startSelection;
    double m_endSelection;
    
    bool m_mouseDown;
    double m_mouseX;

    float m_scroll;
    float m_zoom;
};