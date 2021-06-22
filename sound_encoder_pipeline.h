//
// Created by overflown on 6/22/21.
//

#ifndef OPUS_TRYOUT_SOUND_ENCODER_PIPELINE_H
#define OPUS_TRYOUT_SOUND_ENCODER_PIPELINE_H

#include <QAudioFormat>

class QByteArray;
class OpusEncoder;

class SoundEncoderPipeline {
public:
    explicit SoundEncoderPipeline(const QAudioFormat &format);
    QByteArray encode(const QByteArray&);
    [[nodiscard]] QAudioFormat format() const;

private:
    QAudioFormat format_;
    OpusEncoder *encoder_;
};


#endif //OPUS_TRYOUT_SOUND_ENCODER_PIPELINE_H
