//
// Created by overflown on 5/31/21.
//

#ifndef OPUS_TRYOUT_SOUND_ENCODER_H
#define OPUS_TRYOUT_SOUND_ENCODER_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include "audio_opus_pipeline.h"

class OpusEncoder;

class QByteArray;

class SoundEncoder : public QObject {
    Q_OBJECT
public:
    explicit SoundEncoder(const QAudioDeviceInfo &, const QAudioFormat& format);
    void start();
    void stop();

signals:
    void frameEncoded(QByteArray);

private:
    QByteArray buffer_;
    OpusEncoderPipeline pipeline_;
    QAudioDeviceInfo device_;
    QScopedPointer<QAudioInput> input_;
};


#endif //OPUS_TRYOUT_SOUND_ENCODER_H
