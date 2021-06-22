#include "sound_decoder.h"

#include <QByteArray>
#include <QDebug>

SoundDecoder::SoundDecoder(const QAudioDeviceInfo &device, const QAudioFormat &format) : device_(device),
                                                                                         pipeline_(format),
                                                                                         output_(new QAudioOutput{
                                                                                                 device_, format}),
                                                                                         output_device_() {}

void SoundDecoder::start() {
    output_device_ = output_->start();
}

void SoundDecoder::playDecoded(const QByteArray &packet) {
    if (!output_device_) return; //todo

    auto decoded = pipeline_.decode(packet);

    output_device_->write(decoded);
    qDebug() << "Decoder: playing" << decoded.size() << "bytes";
}
