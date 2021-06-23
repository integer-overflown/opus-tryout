#ifndef OPUS_TRYOUT_ENCODER_DEVICE_H
#define OPUS_TRYOUT_ENCODER_DEVICE_H
#include "pipe_device.h"

class EncoderDevice : public PipeDevice {
protected:
    qint64 writeData(const char *data, qint64 len) override;
};


#endif //OPUS_TRYOUT_ENCODER_DEVICE_H
