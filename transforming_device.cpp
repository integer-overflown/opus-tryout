#include "transforming_device.h"
#include <QDebug>
#include <limits>

TransformingDevice::TransformingDevice(Transform transform) : transform_(std::move(transform)) {}

qint64 TransformingDevice::writeData(const char *data, qint64 len) {
    Q_ASSERT(len <= std::numeric_limits<int>::max());
    if (auto pipe = getPipe(); pipe && transform_) {
        auto enc = transform_(QByteArray::fromRawData(data, static_cast<int>(len)));
        pipe->write(enc);
    }
    return len;
}
