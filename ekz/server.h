#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class EchoServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit EchoServer(QObject *parent = nullptr);

    int getClientCount() const;
    void decreaseClientCount();

signals:
    void clientDisconnected();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    int clientCount;
};

#endif // SERVER_H
