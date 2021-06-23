#ifndef OPUS_TRYOUT_TRANSFORMING_DEVICE_H
#define OPUS_TRYOUT_TRANSFORMING_DEVICE_H
#include "pipe_device.h"

class TransformingDevice : public PipeDevice {
public:
    using Transform = std::function<QByteArray(QByteArray)>;
public:
    explicit TransformingDevice(Transform);
protected:
    qint64 writeData(const char *data, qint64 len) override;
private:
    Transform transform_;
};


#endif //OPUS_TRYOUT_TRANSFORMING_DEVICE_H
