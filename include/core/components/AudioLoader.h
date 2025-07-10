#pragma once

#include <QObject>
#include <vector>

class AudioLoader : public QObject
{
    Q_OBJECT
public:
    AudioLoader(const std::vector<short>& pcm, int channels, int sampleRate, QObject *parent = nullptr);
    AudioLoader(QString file, QObject *parent = nullptr);

    const std::vector<short>& samples() const;
    int channels() const noexcept;
    int sampleRate() const noexcept;
    int64_t duration() const noexcept;

signals:
    void loaded();

private:
    void load(const std::vector<short>& pcm, int channels, int sampleRate);

    std::vector<short> m_samples;
    int m_channels;
    int m_sampleRate;
    int64_t m_duration;
};