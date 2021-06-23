#include "pipe_device.h"

QIODevice *PipeDevice::pipe(QIODevice *to) {
    return pipe_ = to;
}

QIODevice *PipeDevice::getPipe() const {
    return pipe_;
}
