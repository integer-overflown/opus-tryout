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

int AudioPipeline::getFrameDuration() const {
    return frame_duration_;
}

int AudioPipeline::getSamplesPerChannel() const {
    return samples_per_channel_;
}

int AudioPipeline::getFrameByteSize() const {
    return frame_byte_size_;
}
