#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <thread>

#include <vector>

class AudioEditorWidget : public QWidget {
    Q_OBJECT
public:
    AudioEditorWidget(QWidget *parent = nullptr);

    void setFilePath(const QString &path);

private:
    void paintEvent(QPaintEvent *event) override;

    void processSamples(int64_t start = -1, int64_t end = -1);

    std::vector<short> m_samples;
    int m_channels;
    int m_sampleRate;

    QString m_filePath;
};