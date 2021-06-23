#include "encoder_device.h"
#include <QDebug>

qint64 EncoderDevice::writeData(const char *data, qint64 len) {
    if (auto pipe = getPipe()) pipe->write(data, len);
    return len;
}
