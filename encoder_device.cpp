#include "encoder_device.h"
#include <QDebug>
#include <limits>

EncoderDevice::EncoderDevice(EncoderDevice::Encoder encoder) : encoder_(std::move(encoder)) {}

qint64 EncoderDevice::writeData(const char *data, qint64 len) {
    Q_ASSERT(len <= std::numeric_limits<int>::max());
    if (auto pipe = getPipe(); pipe && encoder_) {
        auto enc = encoder_(QByteArray::fromRawData(data, static_cast<int>(len)));
        pipe->write(enc);
    }
    return len;
}
