#include <opus/opus_types.h>
#include "sound_encoder_pipeline.h"

#include <QDebug>
#include <opus/opus.h>

SoundEncoderPipeline::SoundEncoderPipeline(const QAudioFormat &format) : format_(format) {
    int error;
    encoder_ = opus_encoder_create(format.sampleRate(), format.channelCount(), OPUS_APPLICATION_VOIP, &error);
    if (error < 0) {
        qCritical() << "Got an error code: " << error;
        return;
    }
    opus_encoder_ctl(encoder_, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
}

QByteArray SoundEncoderPipeline::encode(const QByteArray& frame) {
    const auto msFrameDuration = static_cast<int>(format_.durationForFrames(1));
    const auto samplesPerChannel = format_.sampleRate() / 1000 * msFrameDuration;
    const auto frameByteSize = samplesPerChannel * format_.channelCount() * format_.sampleSize() / 8;

    QByteArray out;
    out.reserve(frameByteSize);
    auto packetSize = opus_encode(encoder_,
                                  reinterpret_cast<const opus_int16 *>(frame.constData()),
                                  samplesPerChannel,
                                  reinterpret_cast<quint8 *>(out.data()),
                                  frameByteSize);
    if (packetSize < 0) {
        qCritical() << "Failed to encode a package (code" << packetSize << "), stopping";
        return QByteArray();
    }
    out.resize(packetSize);
    return out;
}

QAudioFormat SoundEncoderPipeline::format() const {
    return format_;
}
