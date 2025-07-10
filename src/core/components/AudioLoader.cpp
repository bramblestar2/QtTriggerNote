#include "core/components/AudioLoader.h" 

#include <QObject>
#include <vector>

class AudioLoader : public QObject
{
    Q_OBJECT
public:
    AudioLoader(const std::vector<short>& pcm, int channels, int sampleRate);
    AudioLoader(QString file);

    const std::vector<short>& samples() const;
    int channels() const noexcept;
    int sampleRate() const noexcept;
    int64_t duration() const noexcept;

signals:
    void loaded();

private:
    std::vector<short> m_samples;
    int m_channels;
    int m_sampleRate;
    int64_t m_duration;
};