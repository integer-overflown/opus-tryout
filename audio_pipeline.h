#ifndef OPUS_TRYOUT_AUDIO_PIPELINE_H
#define OPUS_TRYOUT_AUDIO_PIPELINE_H
#include <QAudioFormat>

class QAudioDeviceInfo;

class AudioPipeline {
public:
    explicit AudioPipeline(const QAudioFormat&);

    [[nodiscard]] QAudioFormat format() const;
    [[nodiscard]] int getFrameDuration() const;
    [[nodiscard]] int getSamplesPerChannel() const;
    [[nodiscard]] int getFrameByteSize() const;
private:
    int frame_duration_;
    int samples_per_channel_;
    int frame_byte_size_;
    QAudioFormat format_;
};


#endif //OPUS_TRYOUT_AUDIO_PIPELINE_H
