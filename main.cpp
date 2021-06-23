#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QAudioOutput>
#include <opus/opus.h>

#include "sound_encoder.h"

#include "audio_pipeline.h"
#include "buffered_device.h"
#include "transforming_device.h"

int main(int argc, char *argv[]) {
    using namespace std::chrono_literals;
    QCoreApplication app(argc, argv);

    const auto dev = QAudioDeviceInfo::defaultInputDevice();
    const auto fmt = AudioPipeline::setupAudioFormat(dev);
    QAudioInput input(dev, fmt);
    QAudioOutput output(QAudioDeviceInfo::defaultOutputDevice(), fmt);

    const auto outDevice = output.start();

    auto encoder = std::make_unique<OpusEncoderPipeline>(fmt);
    auto decoder = std::make_unique<OpusDecoderPipeline>(fmt);

    BufferedDevice buf;
    TransformingDevice enc([i = encoder.get()](auto buf) { return i->encode(buf); });
    TransformingDevice dec([i = decoder.get()](auto buf) { return i->decode(buf); });
    enc.open(QIODevice::WriteOnly);
    dec.open(QIODevice::WriteOnly);

    buf.open(QIODevice::WriteOnly);
    buf.setChunkSize(encoder->getFrameByteSize());
    buf.pipe(&enc)->pipe(&dec)->redirect(outDevice);

    input.start(&buf);

    return QCoreApplication::exec();
}
