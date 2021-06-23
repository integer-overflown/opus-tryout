#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QAudioInput>
#include <QAudioOutput>

#include "audio_pipeline.h"
#include "audio_opus_pipeline.h"
#include "buffered_device.h"
#include "transforming_device.h"

static void dumpFormat(const QAudioFormat &format) {
    qInfo() << "";
    qInfo() << "FORMAT: sample rate" << format.sampleRate() << "Hz";
    qInfo() << "FORMAT: channel count" << format.channelCount();
    qInfo() << "FORMAT: sample size" << format.sampleSize() << "bits";
    qInfo() << "FORMAT: byte order" << format.byteOrder();
    qInfo() << "";
}

static void checkFormatSupport(const QAudioDeviceInfo& dev, const QAudioFormat& format) {
    if (dev.isFormatSupported(format)) return;
    qWarning() << "The format is incompatible with input device";
    qWarning() << "The nearest is:";
    dumpFormat(dev.nearestFormat(format));
}

static QAudioFormat getDefaultFormat() {
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

    return fmt;
}

int main(int argc, char *argv[]) {
    using namespace std::chrono_literals;
    QCoreApplication app(argc, argv);

    const auto defaultInputDevice = QAudioDeviceInfo::defaultInputDevice();
    const QAudioDeviceInfo &defaultOutputDevice = QAudioDeviceInfo::defaultOutputDevice();
    const auto fmt = getDefaultFormat();

    dumpFormat(fmt);
    checkFormatSupport(defaultInputDevice, fmt);
    checkFormatSupport(defaultOutputDevice, fmt);

    QAudioInput input(defaultInputDevice, fmt);
    QAudioOutput output(defaultOutputDevice, fmt);

    const auto outDevice = output.start();

    auto encoder = std::make_unique<OpusEncoderPipeline>(fmt);
    auto decoder = std::make_unique<OpusDecoderPipeline>(fmt);

    BufferedDevice buf;
    TransformingDevice enc([i = encoder.get()](auto buf) { return i->encode(buf); });
    TransformingDevice dec([i = decoder.get()](auto buf) { return i->decode(buf); });

    // TODO: deal with repeated open() somehow
    enc.open(QIODevice::WriteOnly);
    dec.open(QIODevice::WriteOnly);
    buf.open(QIODevice::WriteOnly);

    buf.setChunkSize(encoder->getFrameByteSize());
    buf.pipe(&enc)->pipe(&dec)->redirect(outDevice);

    input.start(&buf);

    return QCoreApplication::exec();
}
