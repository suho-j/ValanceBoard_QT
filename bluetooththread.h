#ifndef BLUETOOTHTHREAD_H
#define BLUETOOTHTHREAD_H

#include "DeviceINQ.h"
#include "Enums.h"
#include "BluetoothException.h"
#include "BTSerialPortBinding.h"
#include <iostream>
#include <exception>
#include <vector>
#include <memory>
#include <QThread>
#include <QAtomicInt>

class BluetoothThread : public QThread
{
    Q_OBJECT

public:
    QAtomicInt stopLoop = 0;

signals:
    void sensorData(QString data);

    void connected();

    void disconnected(bool onceConnected);

protected:
    virtual void run();
};

#endif // BLUETOOTHTHREAD_H
