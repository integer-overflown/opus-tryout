#ifndef OPUS_TRYOUT_AUDIO_OPUS_PIPELINE_H
#define OPUS_TRYOUT_AUDIO_OPUS_PIPELINE_H

#include <QAudioFormat>

class QByteArray;
class OpusEncoder;
class OpusDecoder;

struct AudioStreamFormat {
    int frameDuration;
    int samplesPerChannel;
    int frameByteSize;

    explicit AudioStreamFormat(const QAudioFormat &);
};

class OpusEncoderPipeline {
public:
    explicit OpusEncoderPipeline(const QAudioFormat &format);
    QByteArray encode(const QByteArray&);
    [[nodiscard]] QAudioFormat format() const;

private:
    QAudioFormat format_;
    OpusEncoder *encoder_;
    const AudioStreamFormat constants_;
};

class OpusDecoderPipeline {
public:
    explicit OpusDecoderPipeline(const QAudioFormat &format);
    QByteArray decode(const QByteArray&);
    [[nodiscard]] QAudioFormat format() const;

private:
    QAudioFormat format_;
    OpusDecoder *decoder_;
    const AudioStreamFormat constants_;
};


#endif //OPUS_TRYOUT_AUDIO_OPUS_PIPELINE_H
