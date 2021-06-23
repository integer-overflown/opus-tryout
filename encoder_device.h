#ifndef OPUS_TRYOUT_ENCODER_DEVICE_H
#define OPUS_TRYOUT_ENCODER_DEVICE_H
#include "pipe_device.h"

class EncoderDevice : public PipeDevice {
public:
    using Encoder = std::function<QByteArray(QByteArray)>;
public:
    explicit EncoderDevice(Encoder);
protected:
    qint64 writeData(const char *data, qint64 len) override;
private:
    Encoder encoder_;
};


#endif //OPUS_TRYOUT_ENCODER_DEVICE_H
