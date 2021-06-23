#ifndef OPUS_TRYOUT_BUFFERED_DEVICE_H
#define OPUS_TRYOUT_BUFFERED_DEVICE_H
#include <QIODevice>
#include <QBuffer>

class BufferedDevice : public QIODevice {
public:
    BufferedDevice();
    void setChunkSize(int);
    QIODevice* pipe(QIODevice*);
    bool open(OpenMode mode) override;
protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
private:
    int chunk_size_;
    QIODevice *pipe_;
    QByteArray buffer_;
};

#endif //OPUS_TRYOUT_BUFFERED_DEVICE_H
