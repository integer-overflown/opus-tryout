#ifndef OPUS_TRYOUT_AUDIO_PIPELINE_H
#define OPUS_TRYOUT_AUDIO_PIPELINE_H

class QAudioFormat;
class QAudioDeviceInfo;

class AudioPipeline {
public:
    static QAudioFormat setupAudioFormat(const QAudioDeviceInfo &);
public:
    static constexpr int kSampleRate = 48'000;
    static constexpr int kChannelCount = 1;
    static constexpr int kSampleSize = 16;
};


#endif //OPUS_TRYOUT_AUDIO_PIPELINE_H
