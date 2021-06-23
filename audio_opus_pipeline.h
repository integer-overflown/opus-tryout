#ifndef OPUS_TRYOUT_AUDIO_OPUS_PIPELINE_H
#define OPUS_TRYOUT_AUDIO_OPUS_PIPELINE_H

#include <QAudioFormat>

#include "audio_pipeline.h"

class QByteArray;
class OpusEncoder;
class OpusDecoder;

class OpusEncoderPipeline : public AudioPipeline {
public:
    explicit OpusEncoderPipeline(const QAudioFormat &format);
    ~OpusEncoderPipeline();
    QByteArray encode(const QByteArray&);

private:
    OpusEncoder *encoder_;
};

class OpusDecoderPipeline : public AudioPipeline {
public:
    explicit OpusDecoderPipeline(const QAudioFormat &format);
    ~OpusDecoderPipeline();
    QByteArray decode(const QByteArray&);
private:
    OpusDecoder *decoder_;
};


#endif //OPUS_TRYOUT_AUDIO_OPUS_PIPELINE_H
