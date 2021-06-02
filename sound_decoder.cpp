#include "sound_decoder.h"

#include <QByteArray>
#include <QDebug>
#include <opus/opus.h>

SoundDecoder::SoundDecoder(const QAudioDeviceInfo &device, const QAudioFormat &format) : device_(device),
                                                                                         format_(format),
                                                                                         output_device_() {
    int error;
    decoder_ = opus_decoder_create(format_.sampleRate(), format_.channelCount(), &error);
    if (error < 0) {
        qCritical() << "Failed to initialize decoder: receiver error code" << error;
        return;
    }
}

SoundDecoder::~SoundDecoder() {
    opus_decoder_destroy(decoder_);
}

void SoundDecoder::start() {
    output_.reset(new QAudioOutput{device_, format_});
    output_device_ = output_->start();
}

void SoundDecoder::playDecoded(const QByteArray &packet) {
    if (!output_device_) return; //todo

    const auto msFrameDuration = static_cast<int>(format_.durationForFrames(1));
    const auto samplesPerChannel = format_.sampleRate() / 1000 * msFrameDuration;
    const auto frameByteSize = samplesPerChannel * format_.channelCount() * format_.sampleSize() / 8;

    qint16 pcm[samplesPerChannel * format_.channelCount()];
    int frameSize = opus_decode(decoder_, reinterpret_cast<const quint8 *>(packet.data()), packet.size(),
                                pcm, frameByteSize, 0);

    if (frameSize < 0) {
        qCritical() << "Failed to decode, received code" << frameSize;
        return;
    }

    const auto bytes = frameSize * format_.channelCount();
    output_device_->write(reinterpret_cast<const char *>(pcm), bytes);
    qDebug() << "Decoder: playing" << bytes << "bytes";
}
