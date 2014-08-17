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

#include "stuserverthread.h"

#include <QtNetwork>

//! [0]
StuServerThread::StuServerThread(int socketDescriptor, QStringList fns, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor), fileNames(fns)
{
}
//! [0]

//! [1]
void StuServerThread::run()
{
    QTcpSocket tcpSocket;
//! [1] //! [2]
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }
    emit ipgot(tcpSocket.peerAddress().toString());
    if(!tcpSocket.waitForReadyRead()){
        emit error(tcpSocket.error());
        return;
    }
    QDataStream in(&tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    quint16 blockSize;
    in>>blockSize;
    QString command;
    in>>command;
    if(command=="lockScreen"){
        emit lockScreen();
        tcpSocket.disconnectFromHost();
        tcpSocket.waitForDisconnected();
        return;
    }
    if(command=="unlockScreen"){
        emit unlockScreen();
        tcpSocket.disconnectFromHost();
        tcpSocket.waitForDisconnected();
        return;
    }
    if(command=="testStart"){
        emit testStart();
        tcpSocket.disconnectFromHost();
        tcpSocket.waitForDisconnected();
        return;
    }
    if(command=="testFinished"){
        emit testFinished();
        tcpSocket.disconnectFromHost();
        tcpSocket.waitForDisconnected();
        return;
    }
    if (command=="checkLink")
    {
        tcpSocket.disconnectFromHost();
        tcpSocket.waitForDisconnected();
        return;
    }
    if(command=="sendFile"){
        //emit sendFile(tcpSocket.socketDescriptor());
        int n=fileNames.size();
        //QTcpSocket tcpSocket;
        /*if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
            emit error(tcpSocket.error());
            return;
        }*/
        /*if(!tcpSocket.waitForConnected()){
            emit error(tcpSocket.error());
            return;
        }*/
        QByteArray block0;
        QDataStream out0(&block0, QIODevice::WriteOnly);
        out0.setVersion(QDataStream::Qt_4_0);
        out0 << n;
        tcpSocket.write(block0);
        tcpSocket.waitForBytesWritten();

        for(int i=0;i<n;++i){
            QByteArray block1;
            QDataStream out1(&block1, QIODevice::WriteOnly);
            out1.setVersion(QDataStream::Qt_4_0);
            out1 << (quint16)0;
            QFileInfo fi(fileNames.at(i));
            QString name=fi.fileName();
            qDebug() << name;
            out1 << name;
            //out1 << fileNames.at(i).right(fileNames.at(i).length()-fileNames.at(i).lastIndexOf('\\'));
            out1.device()->seek(0);
            out1 << (quint16)(block1.size() - sizeof(quint16));
            tcpSocket.write(block1);
            tcpSocket.waitForBytesWritten();

            QFile file(fileNames.at(i));
            if(file.open(QIODevice::ReadOnly)){
                QByteArray block2;
                QDataStream out2(&block2, QIODevice::WriteOnly);
                out2.setVersion(QDataStream::Qt_4_0);
                //out2 << (quint64)0;

                QByteArray data(file.readAll());
                out2 << data;
                //out2.device()->seek(0);
                //out2 << (quint64)(block2.size() - sizeof(quint64));
                tcpSocket.write(block2);
            }
            tcpSocket.waitForBytesWritten();

        }

        //tcpSocket.write();
        tcpSocket.disconnectFromHost();
        tcpSocket.waitForDisconnected();
        return;
    }
    emit sendMessage(command);
//! [2] //! [3]

    /*QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << text;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
//! [3] //! [4]

    tcpSocket.write(block);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();*/

}
//! [4]
