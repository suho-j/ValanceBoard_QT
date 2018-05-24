#include "bluetooththread.h"


void BluetoothThread::run()
{
    bool onceConnected = false;
    try
    {
        std::unique_ptr<DeviceINQ> inq(DeviceINQ::Create());
        std::string addr = "98:d3:32:30:92:0c";
        auto serial = BTSerialPortBinding::Create(addr, inq->SdpSearch(addr));
        serial->Connect();

        onceConnected = true;
        emit connected();

        char buf[1024];
        int pos = 0;
        while (1)
        {
            if(stopLoop == 1)
            {
                serial->Close();
                break;
            }

            int len = serial->Read(buf + pos, 256);
            if (len == 0)
                break;
            pos += len;
            bool found_newline;
            do
            {
                found_newline = false;
                for (int i = 0; i < pos; i++)
                {
                    if (buf[i] == '\n')
                    {
                        char copy[256];
                        buf[i - 1] = 0;
                        strcpy(copy, buf);
                        found_newline = true;
                        for (int j = i + 1; j < pos; j++)
                            buf[j - (i + 1)] = buf[j];
                        pos -= i + 1;

                        emit sensorData(copy);
                        break;
                    }
                }
            } while (found_newline);
        }
        std::cout << "connection was lost" << std::endl;
    }
    catch (const BluetoothException& e)
    {
        std::cout << e.what() << std::endl;
    }

    emit disconnected(onceConnected);
}
