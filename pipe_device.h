#ifndef OPUS_TRYOUT_PIPE_DEVICE_H
#define OPUS_TRYOUT_PIPE_DEVICE_H
#include <QIODevice>

class PipeDevice : public QIODevice {
public:
    void pipe(QIODevice *);
    bool open(OpenMode mode) override;
protected:
    qint64 readData(char *data, qint64 maxlen) override;
protected:
    [[nodiscard]] QIODevice *getPipe() const;
private:
    QIODevice *pipe_;
};

#endif //OPUS_TRYOUT_PIPE_DEVICE_H
