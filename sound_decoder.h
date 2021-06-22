#ifndef OPUS_TRYOUT_SOUND_DECODER_H
#define OPUS_TRYOUT_SOUND_DECODER_H
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QScopedPointer>
#include <QAudioOutput>

class OpusDecoder;
class QByteArray;

class SoundDecoder : public QObject {
    Q_OBJECT
public:
    explicit SoundDecoder(const QAudioDeviceInfo&, const QAudioFormat&);
    ~SoundDecoder() override;
public slots:
    void start();
    void playDecoded(const QByteArray&);
private:
    QAudioDeviceInfo device_;
    OpusDecoder *decoder_;
    QAudioFormat format_;
    QScopedPointer<QAudioOutput> output_;
    QIODevice *output_device_;
};


#endif //OPUS_TRYOUT_SOUND_DECODER_H
