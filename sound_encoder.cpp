#include "sound_encoder.h"

#include <QDebug>
#include <QAudioDeviceInfo>
#include <QAudioInput>

#include <opus/opus.h>

#include "audio_pipeline.h"

SoundEncoder::SoundEncoder(const QAudioDeviceInfo &device) : device_(device),
                                                             format_(AudioPipeline::setupAudioFormat(device)),
                                                             running_(false) {
    int error;
    encoder_ = opus_encoder_create(format_.sampleRate(), format_.channelCount(), OPUS_APPLICATION_VOIP, &error);
    if (error < 0) {
        qCritical() << "Got an error code: " << error;
        return;
    }
    opus_encoder_ctl(encoder_, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
}

SoundEncoder::~SoundEncoder() {
    opus_encoder_destroy(encoder_);
}

void SoundEncoder::start() {
    input_.reset(new QAudioInput(device_, format_));

    const auto msFrameDuration = static_cast<int>(format_.durationForFrames(1));
    const auto samplesPerChannel = format_.sampleRate() / 1000 * msFrameDuration;
    const auto frameByteSize = samplesPerChannel * format_.channelCount() * format_.sampleSize() / 8;

    qint32 bitrate;
    opus_encoder_ctl(encoder_, OPUS_GET_BITRATE(&bitrate));
    qInfo() << "Recording on" << device_.deviceName();
    qInfo() << "Frame duration:" << msFrameDuration << "ms";
    qInfo() << "Samples per channel:" << samplesPerChannel;
    qInfo() << "Frame size:" << frameByteSize << "bytes";
    qInfo() << "Bitrate:" << bitrate;

    QObject::connect(input_.get(), &QAudioInput::stateChanged, [](auto state) {
        qDebug() << "Input changed state to" << state;
    });
    auto dev = input_->start();
    input_->setNotifyInterval(msFrameDuration);

    QObject::connect(dev, &QIODevice::readyRead, [=] {
        auto buf = dev->readAll();

        QByteArray out;
        out.reserve(frameByteSize);
        auto packetSize = opus_encode(encoder_,
                                      reinterpret_cast<const opus_int16 *>(buf.constData()),
                                      samplesPerChannel,
                                      reinterpret_cast<quint8 *>(out.data()),
                                      frameByteSize);
        if (packetSize < 0) {
            qCritical() << "Failed to encode a package (code" << packetSize << "), stopping";
            running_ = false;
            return;
        }
        out.resize(packetSize);
        emit frameEncoded(out);
    });
}

void SoundEncoder::stop() {
    running_.store(false);
    input_->stop();
}

QAudioFormat SoundEncoder::format() const {
    return format_;
}
