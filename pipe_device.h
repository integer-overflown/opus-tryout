#ifndef OPUS_TRYOUT_PIPE_DEVICE_H
#define OPUS_TRYOUT_PIPE_DEVICE_H
#include <QIODevice>

class PipeDevice : public QIODevice {
public:
    QIODevice* pipe(QIODevice *);
protected:
    QIODevice *getPipe() const;
private:
    QIODevice *pipe_;
};

#endif //OPUS_TRYOUT_PIPE_DEVICE_H
