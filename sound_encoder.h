//
// Created by overflown on 5/31/21.
//

#ifndef OPUS_TRYOUT_SOUND_ENCODER_H
#define OPUS_TRYOUT_SOUND_ENCODER_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <any>
#include "sound_encoder_pipeline.h"

class OpusEncoder;

class QByteArray;

class SoundEncoder : public QObject {
    Q_OBJECT
public:
    explicit SoundEncoder(const QAudioDeviceInfo &, const QAudioFormat& format);
    void start();
    void stop();
    QAudioFormat format() const;

signals:
    void frameEncoded(QByteArray);

private:
    SoundEncoderPipeline pipeline_;
    QAudioDeviceInfo device_;
    QScopedPointer<QAudioInput> input_;
};


#endif //OPUS_TRYOUT_SOUND_ENCODER_H
