#include "audio_pipeline.h"
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QDebug>

QAudioFormat AudioPipeline::setupAudioFormat(const QAudioDeviceInfo &device) {
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
        qInfo() << "FORMAT: sample rate" << fmt.sampleRate() << "Hz";
        qInfo() << "FORMAT: channel count" << fmt.channelCount();
        qInfo() << "FORMAT: sample size" << fmt.sampleSize() << "bits";
        qInfo() << "FORMAT: byte order" << fmt.byteOrder();
        qInfo() << "";
    }

    return fmt;
}
