#include "encoder_device.h"
#include <QDebug>

qint64 EncoderDevice::writeData(const char *data, qint64 len) {
    qDebug() << len;
    return len;
}

qint64 EncoderDevice::readData(char *data, qint64 maxlen) {
    return 0;
}
