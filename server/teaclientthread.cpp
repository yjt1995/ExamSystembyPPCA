/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "teaclientthread.h"

#include <QtNetwork>

//! [0]
TeaClientThread::TeaClientThread(QString ipAdr, QString comm, QString arg, int num, QObject *parent)
    : QThread(parent), arg(arg),ipAd(ipAdr),command(comm),num(num)
{
    //ipAd=ipAdr;
    //command=comm;
}
//! [0]

//! [1]
void TeaClientThread::run()
{

    QTcpSocket *tcpSocket=new QTcpSocket;
    tcpSocket->connectToHost(ipAd,14423);

    if(!tcpSocket->waitForConnected()){
        qDebug("Fuck! Connection Failed!");
        qDebug()<<tcpSocket->errorString();
        emit this->error(num);
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << command;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
//! [3] //! [4]

    tcpSocket->write(block);
    if(command=="sendFile"){
        if(!tcpSocket->waitForBytesWritten()){
            emit this->error(num);
            return;
        }

        //FileReceiver *fr=new FileReceiver(tcpSocket->socketDescriptor(),arg,this);
        //connect(fr,SIGNAL(finished()),fr,SLOT(deleteLater()));
        //fr->run();
        QString dict(arg);
        if(!dict.endsWith('\\')){
            dict.append('\\');
        }
        QString path(dict);
        QDir tmp;
        tmp.mkpath(dict);
        int n;
        if(!tcpSocket->waitForReadyRead()){
            emit this->error(num);
            return;
        }
        QDataStream in(tcpSocket);
        in.setVersion(QDataStream::Qt_4_0);
        in >>n;
        for(int i=0;i<n;++i){
            if(!tcpSocket->waitForReadyRead()){
                //emit this->error(num);
                //return;
            }
            quint16 size;
            in >> size;
            QString fileName(dict);
            QString name;
            in >> name;
            qDebug()<<name;
            //fileName.append('\\');
            fileName.append(name);
            QFile file(fileName);
            qDebug()<<fileName;
            if(file.open(QIODevice::WriteOnly)){
                quint32 sz;
                in>>sz;
                if(!tcpSocket->waitForReadyRead()){
                    //emit this->error(num);
                }
                QByteArray d(tcpSocket->read(sz));
                file.write(d);
                file.close();
            }else{
                qDebug()<<"File Not Opened";
                emit this->error(num);
            }
            if(!tcpSocket->waitForReadyRead()){
                //emit this->error(num);
                //return;
            }
        }

    }
    tcpSocket->disconnectFromHost();
    tcpSocket->waitForDisconnected();
    emit this->succeed(num,command);
}
//! [4]
