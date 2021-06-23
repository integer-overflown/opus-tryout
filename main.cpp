#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QAudioOutput>
#include <opus/opus.h>

#include "sound_encoder.h"
#include "sound_decoder.h"

#include "audio_pipeline.h"
#include "buffered_device.h"
#include "encoder_device.h"

int main(int argc, char *argv[]) {
    using namespace std::chrono_literals;
    QCoreApplication app(argc, argv);

    const auto dev = QAudioDeviceInfo::defaultInputDevice();
    const auto fmt = AudioPipeline::setupAudioFormat(dev);
    QAudioInput input(dev, fmt);
    QAudioOutput output(QAudioDeviceInfo::defaultOutputDevice(), fmt);

    const auto outDevice = output.start();

    BufferedDevice buf;
    buf.open(QIODevice::WriteOnly);
    buf.setChunkSize(1920);
    buf.pipe(outDevice);

    input.start(&buf);

    return QCoreApplication::exec();
}
