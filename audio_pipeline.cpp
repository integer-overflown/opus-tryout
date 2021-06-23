#include "audio_pipeline.h"
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QDebug>

AudioPipeline::AudioPipeline(const QAudioFormat &format) : format_(format) {
    // TODO: maybe replace frame duration with constant?
    frame_duration_ = static_cast<int>(format.durationForFrames(1));
    samples_per_channel_ = format.sampleRate() / 1000 * frame_duration_;
    frame_byte_size_ = samples_per_channel_ * format.channelCount() * format.sampleSize() / 8;
    qDebug() << "Frame duration is" << frame_duration_ << "ms";
}

QAudioFormat AudioPipeline::format() const {
    return format_;
}

QAudioFormat AudioPipeline::setupAudioFormat(const QAudioDeviceInfo &device) {
    constexpr int kSampleRate = 48'000;
    constexpr int kChannelCount = 1;
    constexpr int kSampleSize = 16;

    QAudioFormat fmt;
    fmt.setSampleRate(kSampleRate);
    fmt.setChannelCount(kChannelCount);
    fmt.setCodec("audio/pcm");
    fmt.setSampleSize(kSampleSize);
    fmt.setSampleType(QAudioFormat::SignedInt); // required for 16 bits
    fmt.setByteOrder(QAudioFormat::LittleEndian); // required for Opus

    if (!device.isFormatSupported(fmt)) {
        qInfo() << "Format is not supported, falling back to nearest";
        fmt = device.nearestFormat(fmt);
    }

    qInfo() << "FORMAT: sample rate" << fmt.sampleRate() << "Hz";
    qInfo() << "FORMAT: channel count" << fmt.channelCount();
    qInfo() << "FORMAT: sample size" << fmt.sampleSize() << "bits";
    qInfo() << "FORMAT: byte order" << fmt.byteOrder();
    qInfo() << "";

    return fmt;
}

int AudioPipeline::getFrameDuration() const {
    return frame_duration_;
}

int AudioPipeline::getSamplesPerChannel() const {
    return samples_per_channel_;
}

int AudioPipeline::getFrameByteSize() const {
    return frame_byte_size_;
}
