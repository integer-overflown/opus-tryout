#include <opus/opus_types.h>
#include "audio_opus_pipeline.h"

#include <QDebug>
#include <opus/opus.h>

OpusEncoderPipeline::OpusEncoderPipeline(const QAudioFormat &format) : AudioPipeline(format) {
    int error;
    encoder_ = opus_encoder_create(format.sampleRate(), format.channelCount(), OPUS_APPLICATION_VOIP, &error);
    if (error < 0) {
        qCritical() << "Got an error code: " << error;
        return;
    }
    opus_encoder_ctl(encoder_, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
}

OpusEncoderPipeline::~OpusEncoderPipeline() {
    opus_encoder_destroy(encoder_);
}

QByteArray OpusEncoderPipeline::encode(const QByteArray& frame) {
    QByteArray out;
    const auto frameSize = AudioPipeline::getFrameByteSize();
    out.reserve(frameSize);
    auto packetSize = opus_encode(encoder_,
                                  reinterpret_cast<const opus_int16 *>(frame.constData()),
                                  getSamplesPerChannel(),
                                  reinterpret_cast<quint8 *>(out.data()),
                                  frameSize);
    if (packetSize < 0) {
        qCritical() << "Failed to encode a package (code" << packetSize << "), stopping";
        return QByteArray();
    }
    out.resize(packetSize);
    return out;
}

OpusDecoderPipeline::OpusDecoderPipeline(const QAudioFormat &format) : AudioPipeline(format) {
    int error;
    decoder_ = opus_decoder_create(format.sampleRate(), format.channelCount(), &error);
    if (error < 0) {
        qCritical() << "Failed to initialize decoder: received error code" << error;
        return;
    }
}

OpusDecoderPipeline::~OpusDecoderPipeline() {
    opus_decoder_destroy(decoder_);
}

QByteArray OpusDecoderPipeline::decode(const QByteArray &frame) {
    QByteArray pcm(getFrameByteSize(), Qt::Uninitialized);
    int samples = opus_decode(decoder_, reinterpret_cast<const quint8 *>(frame.data()), frame.size(),
                              reinterpret_cast<opus_int16 *>(pcm.data()), getSamplesPerChannel(), 0);

    if (samples < 0) {
        qCritical() << "Failed to decode, received code" << samples;
    }

    const auto format = AudioPipeline::format();
    auto total = samples * format.channelCount() * format.sampleSize() / 8;
    Q_ASSERT(pcm.size() <= total);
    pcm.resize(total);
    qDebug() << "DECODER: received packet [ size" << frame.size() << "] decompressed to" << total;
    return pcm;
}
