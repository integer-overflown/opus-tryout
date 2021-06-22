#include <opus/opus_types.h>
#include "audio_opus_pipeline.h"

#include <QDebug>
#include <opus/opus.h>

AudioOpusPipeline::AudioOpusPipeline(const QAudioFormat &format) : format_(format), constants_(format) {
    int error;
    encoder_ = opus_encoder_create(format.sampleRate(), format.channelCount(), OPUS_APPLICATION_VOIP, &error);
    if (error < 0) {
        qCritical() << "Got an error code: " << error;
        return;
    }
    opus_encoder_ctl(encoder_, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
}

QByteArray AudioOpusPipeline::encode(const QByteArray& frame) {
    QByteArray out;
    out.reserve(constants_.frameByteSize);
    auto packetSize = opus_encode(encoder_,
                                  reinterpret_cast<const opus_int16 *>(frame.constData()),
                                  constants_.samplesPerChannel,
                                  reinterpret_cast<quint8 *>(out.data()),
                                  constants_.frameByteSize);
    if (packetSize < 0) {
        qCritical() << "Failed to encode a package (code" << packetSize << "), stopping";
        return QByteArray();
    }
    out.resize(packetSize);
    return out;
}

QAudioFormat AudioOpusPipeline::format() const {
    return format_;
}

AudioStreamFormat::AudioStreamFormat(const QAudioFormat &format) {
    frameDuration = static_cast<int>(format.durationForFrames(1));
    samplesPerChannel = format.sampleRate() / 1000 * frameDuration;
    frameByteSize = samplesPerChannel * format.channelCount() * format.sampleSize() / 8;
}
