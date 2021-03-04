#ifndef COMPORTWORKER_H
#define COMPORTWORKER_H
#include "deviceconnectinfo.h"

class COMPortWorker
{
public:
    COMPortWorker();

    void addActiveComPorts();
    void connectToPort(DeviceConnectInfo devicrconnectinfo);
    void ok_write_toPort(QByteArray command);
    void convert(const char *src, uint8_t **dst, size_t *dst_size);
    uint16_t crc(uint8_t *data, uint8_t size);
};

#endif // COMPORTWORKER_H
