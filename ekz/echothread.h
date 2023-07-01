// echothread.h
#ifndef ECHOTHREAD_H
#define ECHOTHREAD_H

#include <QThread>
#include <QTcpSocket>

class EchoThread : public QThread
{
    Q_OBJECT
public:
    explicit EchoThread(qintptr socketDescriptor, QObject *parent = nullptr);

protected:
    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    qintptr socketDescriptor;

    void processMinPriceRequest(QTcpSocket* socket);
    void processExpensiveInTypeRequest(const QString& request, QTcpSocket* socket);
    void processMinPriceInTypeRequest(const QString& request, QTcpSocket* socket);
    void processExpensiveRequest(QTcpSocket* socket);
};

#endif // ECHOTHREAD_H
