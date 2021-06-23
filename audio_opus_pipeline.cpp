#include <opus/opus_types.h>
#include "audio_opus_pipeline.h"

#include <QDebug>
#include <opus/opus.h>

OpusEncoderPipeline::OpusEncoderPipeline(const QAudioFormat &format) : format_(format), constants_(format) {
    int error;
    encoder_ = opus_encoder_create(format.sampleRate(), format.channelCount(), OPUS_APPLICATION_VOIP, &error);
    if (error < 0) {
        qCritical() << "Got an error code: " << error;
        return;
    }
    opus_encoder_ctl(encoder_, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
}

QByteArray OpusEncoderPipeline::encode(const QByteArray& frame) {
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

QAudioFormat OpusEncoderPipeline::format() const {
    return format_;
}

AudioStreamFormat::AudioStreamFormat(const QAudioFormat &format) {
    frameDuration = static_cast<int>(format.durationForFrames(1));
    samplesPerChannel = format.sampleRate() / 1000 * frameDuration;
    frameByteSize = samplesPerChannel * format.channelCount() * format.sampleSize() / 8;
}

OpusDecoderPipeline::OpusDecoderPipeline(const QAudioFormat &format) : format_(format), constants_(format) {
    int error;
    decoder_ = opus_decoder_create(format_.sampleRate(), format_.channelCount(), &error);
    if (error < 0) {
        qCritical() << "Failed to initialize decoder: received error code" << error;
        return;
    }
}

QByteArray OpusDecoderPipeline::decode(const QByteArray &frame) {
    QByteArray pcm(constants_.frameByteSize, Qt::Uninitialized);
    int samples = opus_decode(decoder_, reinterpret_cast<const quint8 *>(frame.data()), frame.size(),
                              reinterpret_cast<opus_int16 *>(pcm.data()), constants_.frameByteSize, 0);

    if (samples < 0) {
        qCritical() << "Failed to decode, received code" << samples;
    }

    auto total = samples * format_.channelCount() * format_.sampleSize() / 8;
    Q_ASSERT(pcm.size() <= total);
    pcm.resize(total);
    qDebug() << "DECODER: received packet [ size" << frame.size() << "] decompressed to" << total;
    return pcm;
}

QAudioFormat OpusDecoderPipeline::format() const {
    return format_;
}
