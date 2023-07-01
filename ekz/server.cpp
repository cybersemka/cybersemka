#include "server.h"
#include "echothread.h"

EchoServer::EchoServer(QObject *parent)
    : QTcpServer(parent), clientCount(0)
{
}

int EchoServer::getClientCount() const
{
    return clientCount;
}

void EchoServer::decreaseClientCount()
{
    clientCount--;
    emit clientDisconnected();
}

void EchoServer::incomingConnection(qintptr socketDescriptor)
{
    if (clientCount >= 7)
    {
        qDebug() << "Rejected incoming connection. Maximum client count reached.";
        QTcpSocket socket;
        socket.setSocketDescriptor(socketDescriptor);
        socket.disconnectFromHost();
        return;
    }

    EchoThread* thread = new EchoThread(socketDescriptor, this);
    connect(thread, &EchoThread::finished, thread, &EchoThread::deleteLater);
    thread->start();

    clientCount++;
    qDebug() << "Client connected. Total clients:" << clientCount;
}
