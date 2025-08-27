/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <string>

#include <CSerialPort/SerialPort.h>

#include "Link_p.h"
#include "SerialPort.h"

class SerialPortPrivate : public LinkPrivate
{
public:
    std::string portName;
    int baudRate;
    itas109::FlowControl flowControl;
    itas109::Parity parity;
    itas109::StopBits stopBits;
    itas109::DataBits dataBits;

public:
    void Poll() override
    {
        auto q = GetQ<SerialPort *>();
        auto *sp = new itas109::CSerialPort();
        sp->init(portName.c_str(), baudRate, parity, dataBits, stopBits, flowControl);
        if (sp->open()) {
            DoQueueLinkOpened();
            while (!q->TestDestroy()) {
                // Read data....
                ReadBytes(sp);

                // Write data...
                WriteBytes(sp);

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            sp->close();
            DoQueueLinkClosed();
        } else {
            wxtInfo() << sp->getLastErrorMsg();
            DoQueueError(sp->getLastErrorMsg());
        }

        delete sp;
        wxtInfo() << "Serial port loop exit.";
    }

public:
    void ReadBytes(itas109::CSerialPort *sp)
    {
        char data[wxtDataSize] = {0};
        int len = sp->readAllData(&data);
        if (len > 0) {
            std::shared_ptr<char> bytes(new char[len], std::default_delete<char[]>());
            memcpy(bytes.get(), data, len);
            DoQueueRxBytes(bytes, len, portName);
        }
    }

    void WriteBytes(itas109::CSerialPort *sp)
    {
        txBytesLock.lock();
        for (std::pair<std::shared_ptr<char>, int> &ctx : txBytes) {
            int len = sp->writeData(ctx.first.get(), ctx.second);
            if (len == -1) {
                DoQueueError(_("Write data failed."));
            } else {
                std::shared_ptr<char> bytes(new char[len], std::default_delete<char[]>());
                memcpy(bytes.get(), ctx.first.get(), len);
                DoQueueTxBytes(bytes, len, portName);
            }
        }

        txBytes.clear();
        txBytesLock.unlock();
    }
};
