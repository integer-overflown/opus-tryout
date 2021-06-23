#include "buffered_device.h"
#include <QDebug>

qint64 BufferedDevice::writeData(const char *data, qint64 len) {
    Q_ASSERT(len <= std::numeric_limits<int>::max());
    buffer_.append(data, static_cast<int>(len));

    if (auto pipe = getPipe()) {
        const auto count = buffer_.size() / chunk_size_;
        for (auto i = 0; i < count; ++i) {
            auto chunk = QByteArray::fromRawData(buffer_.constData() + i * chunk_size_, chunk_size_);
            pipe->write(chunk);
        }

        buffer_.remove(0, count * chunk_size_);
    }

    return len;
}

BufferedDevice::BufferedDevice() : chunk_size_{std::numeric_limits<int>::max()} {
}

void BufferedDevice::setChunkSize(int size) {
    chunk_size_ = size;
}
