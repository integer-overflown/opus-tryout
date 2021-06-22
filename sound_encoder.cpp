#include "sound_encoder.h"

#include <QDebug>
#include <QAudioDeviceInfo>
#include <QAudioInput>

#include <opus/opus.h>

#include "audio_pipeline.h"

SoundEncoder::SoundEncoder(const QAudioDeviceInfo &device, const QAudioFormat &format) : device_(device),
                                                                                         pipeline_(format) {

}

void SoundEncoder::start() {
    input_.reset(new QAudioInput(device_, pipeline_.format()));
    qInfo() << "Recording on" << device_.deviceName();

    QObject::connect(input_.get(), &QAudioInput::stateChanged, [](auto state) {
        qDebug() << "Input changed state to" << state;
    });

    auto dev = input_->start();

    QObject::connect(dev, &QIODevice::readyRead, [=] {
        auto buf = dev->readAll();
        auto out = pipeline_.encode(buf);
        emit frameEncoded(out);
    });
}

void SoundEncoder::stop() {
    input_->stop();
}
