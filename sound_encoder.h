//
// Created by overflown on 5/31/21.
//

#ifndef OPUS_TRYOUT_SOUND_ENCODER_H
#define OPUS_TRYOUT_SOUND_ENCODER_H
#include <QObject>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioDeviceInfo>

class OpusEncoder;
class QByteArray;

class SoundEncoder : public QObject {
    Q_OBJECT
public:
    explicit SoundEncoder(const QAudioDeviceInfo&);
    ~SoundEncoder() override;
    void start();
    void stop();
    QAudioFormat format() const;

signals:
    void frameEncoded(QByteArray);
private:
    OpusEncoder *encoder_;
    QAudioFormat format_;
    QAudioDeviceInfo device_;
    QScopedPointer<QAudioInput> input_;
    std::atomic<bool> running_;
};


#endif //OPUS_TRYOUT_SOUND_ENCODER_H
