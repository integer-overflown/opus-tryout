#ifndef OPUS_TRYOUT_BUFFERED_DEVICE_H
#define OPUS_TRYOUT_BUFFERED_DEVICE_H
#include <QBuffer>

#include "pipe_device.h"

class BufferedDevice : public PipeDevice {
public:
    BufferedDevice();
    void setChunkSize(int);
protected:
    qint64 writeData(const char *data, qint64 len) override;
private:
    int chunk_size_;
    QByteArray buffer_;
};

#endif //OPUS_TRYOUT_BUFFERED_DEVICE_H
