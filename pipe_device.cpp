#include "pipe_device.h"
#include <QDebug>

PipeDevice *PipeDevice::pipe(PipeDevice *to) {
    pipe_ = to;
    return to;
}

void PipeDevice::redirect(QIODevice *to) {
    pipe_ = to;
}

QIODevice *PipeDevice::getPipe() const {
    return pipe_;
}

bool PipeDevice::open(QIODevice::OpenMode mode) {
    if (mode & WriteOnly) return QIODevice::open(mode);
    qWarning() << "Pipe devices can only be opened in write-only mode";
    return false;
}

qint64 PipeDevice::readData(char *data, qint64 maxlen) {
    return 0;
}
