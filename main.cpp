#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QAudioOutput>
#include <opus/opus.h>

#include "sound_encoder.h"
#include "sound_decoder.h"

#include "audio_pipeline.h"

int main(int argc, char *argv[]) {
    using namespace std::chrono_literals;
    QCoreApplication app(argc, argv);

    QThread audio;

    const QAudioDeviceInfo &deviceInfo = QAudioDeviceInfo::defaultInputDevice();
    const QAudioFormat &format = AudioPipeline::setupAudioFormat(deviceInfo);

    SoundEncoder encoder(deviceInfo, format);
    encoder.moveToThread(&audio);

    SoundDecoder decoder(QAudioDeviceInfo::defaultOutputDevice(), format);

    QObject::connect(&encoder, &SoundEncoder::frameEncoded, &decoder, &SoundDecoder::playDecoded);

    audio.start();
    encoder.start();
    decoder.start();

    return QCoreApplication::exec();
}
