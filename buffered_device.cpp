//
// Created by overflown on 6/23/21.
//

#include "buffered_device.h"
#include <QDebug>

qint64 BufferedDevice::readData(char *data, qint64 maxlen) {
    return 0;
}

qint64 BufferedDevice::writeData(const char *data, qint64 len) {
    Q_ASSERT(len <= std::numeric_limits<int>::max());
    buffer_.append(data, static_cast<int>(len));
    if (!pipe_) return len;

    const auto count = buffer_.size() / chunk_size_;
    for (auto i = 0; i < count; ++i) {
        auto chunk = QByteArray::fromRawData(buffer_.constData() + i, chunk_size_);
        pipe_->write(chunk);
    }

    buffer_.remove(0, count * chunk_size_);

    return len;
}

BufferedDevice::BufferedDevice() : chunk_size_{std::numeric_limits<int>::max()}, pipe_{} {
}

void BufferedDevice::setChunkSize(int size) {
    chunk_size_ = size;
}

QIODevice *BufferedDevice::pipe(QIODevice *to) {
    return pipe_ = to;
}

bool BufferedDevice::open(QIODevice::OpenMode mode) {
    if (mode & WriteOnly) return QIODevice::open(mode);
    qWarning() << "Buffered device can only be opened in write-only mode";
    return false;
}
