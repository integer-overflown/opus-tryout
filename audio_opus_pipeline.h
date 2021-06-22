#ifndef OPUS_TRYOUT_AUDIO_OPUS_PIPELINE_H
#define OPUS_TRYOUT_AUDIO_OPUS_PIPELINE_H

#include <QAudioFormat>

class QByteArray;
class OpusEncoder;

class AudioOpusPipeline {
public:
    explicit AudioOpusPipeline(const QAudioFormat &format);
    QByteArray encode(const QByteArray&);
    [[nodiscard]] QAudioFormat format() const;

private:
    QAudioFormat format_;
    OpusEncoder *encoder_;
};


#endif //OPUS_TRYOUT_AUDIO_OPUS_PIPELINE_H
