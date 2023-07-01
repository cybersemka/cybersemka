#include <QtNetwork>
#include <QtCore>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void socketReadyRead();
    void socketDisconnected();

private:
    QList<QTcpSocket*> clients;
    QMap<QString, int> prices;
    QMap<QString, QString> types;
};

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    // Заполнение данных о товарах
    prices["Товар1"] = 100;
        prices["Товар2"] = 200;
        prices["Товар3"] = 300;

        types["Товар1"] = "Тип1";
    types["Товар2"] = "Тип2";
    types["Товар3"] = "Тип1";
}

void Server::startServer()
{
    if (!listen(QHostAddress::Any, 12345)) {
        qDebug() << "Failed to start server:" << errorString();
    } else {
        qDebug() << "Server started";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "New incoming connection";

    // Создаем новый сокет для подключившегося клиента
    QTcpSocket *clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);

    // Добавляем сокет клиента в список
    clients.append(clientSocket);

    // Соединяем сигналы сокета с соответствующими слотами
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::socketReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::socketDisconnected);
}

void Server::socketReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    QString request = QString::fromUtf8(clientSocket->readAll().trimmed());

    qDebug() << "Received request:" << request;

    QString response;

    // Обработка запроса
    if (request == "min_price") {
        // Обработка запроса на самые дешевые товары
        // ... реализация ...
        response = "Response to min_price request";
    } else if (request.startsWith("expensive_in_type")) {
        // Обработка запроса на товары заданного типа, которые дороже средней цены
        // ... реализация ...
        response = "Response to expensive_in_type request";
    } else if (request.startsWith("min_price_in_type")) {
        // Обработка запроса на самые дешевые товары заданного типа
        // ... реализация ...
        response = "Response to min_price_in_type request";
    } else if (request == "expensive") {
        // Обработка запроса на товары, которые дороже средней цены
        // ... реализация ...
        response = "Response to expensive request";
    } else {
        // Неизвестный запрос
        response = "Unknown request";
    }

    // Отправка ответа клиенту
    clientSocket->write(response.toUtf8());
    clientSocket->flush();
}

void Server::socketDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Client disconnected";

    clients.removeOne(clientSocket);
    clientSocket->deleteLater();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.startServer();

    return a.exec();
}

#include "main.moc"
