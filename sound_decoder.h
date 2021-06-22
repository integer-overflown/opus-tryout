#ifndef OPUS_TRYOUT_SOUND_DECODER_H
#define OPUS_TRYOUT_SOUND_DECODER_H
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QScopedPointer>
#include <QAudioOutput>

#include "audio_opus_pipeline.h"

class OpusDecoder;
class QByteArray;

class SoundDecoder : public QObject {
    Q_OBJECT
public:
    explicit SoundDecoder(const QAudioDeviceInfo&, const QAudioFormat&);

public slots:
    void start();
    void playDecoded(const QByteArray&);
private:
    OpusDecoderPipeline pipeline_;
    QAudioDeviceInfo device_;
    QScopedPointer<QAudioOutput> output_;
    QIODevice *output_device_;
};


#endif //OPUS_TRYOUT_SOUND_DECODER_H
