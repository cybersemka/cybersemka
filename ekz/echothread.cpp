#include "echothread.h"

EchoThread::EchoThread(qintptr socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
}

void EchoThread::run()
{
    QTcpSocket socket;
    if (!socket.setSocketDescriptor(socketDescriptor))
    {
        emit error(socket.error());
        return;
    }

    // обработка сообщений от клиента
    while (socket.isOpen() && socket.isValid())
    {
        if (socket.waitForReadyRead(-1))
        {
            QByteArray requestData = socket.readAll();
            QString request = QString::fromUtf8(requestData).trimmed();

            // обработка запроса "min_price"
            if (request == "min_price")
            {
                processMinPriceRequest(&socket);
            }
            // обработка запроса "expensive_in_type&type"
            else if (request.startsWith("expensive_in_type&"))
            {
                QString type = request.mid(18);
                processExpensiveInTypeRequest(type, &socket);
            }
            // обработка запроса "min_price_in_type&type"
            else if (request.startsWith("min_price_in_type&"))
            {
                processMinPriceInTypeRequest(request.mid(18), &socket);
            }
            // обработка запроса "expensive"
            else if (request == "expensive")
            {
                processExpensiveRequest(&socket);
            }
            else
            {
                // неизвестный запрос, отправить ошибку
                QString response = "Unknown request";
                socket.write(response.toUtf8());
                socket.waitForBytesWritten();
            }
        }
    }

    socket.disconnectFromHost();
    socket.waitForDisconnected();
}

void EchoThread::processMinPriceRequest(QTcpSocket* socket)
{
    // здесь можно добавить код для обработки запроса "min_price"
    // например, можно получить самые дешевые товары из базы данных или из текстового файла

    QString response = "самые дешевые товары: продукт 1, продукт 2, продукт 3";
    socket->write(response.toUtf8());
    socket->waitForBytesWritten();
}

void EchoThread::processExpensiveInTypeRequest(const QString& request, QTcpSocket* socket)
{
    // разбиваем запрос на две части: тип товара и среднюю цену
    QStringList parts = request.split("&");
    if (parts.size() != 2)
    {
        // ошибка в запросе, отправляем сообщение об ошибке
        QString response = "Invalid request format";
        socket->write(response.toUtf8());
        socket->waitForBytesWritten();
        return;
    }

    QString type = parts[0];
    qreal averagePrice = parts[1].toDouble();

    // здесь можно добавить код для обработки запроса "expensive_in_type&type"
    // например, можно получить товары заданного типа, которые дороже средней цены

    QString response = "Expensive items of type " + type + ": item 1, item 2, item 3";
    socket->write(response.toUtf8());
    socket->waitForBytesWritten();
}

void EchoThread::processMinPriceInTypeRequest(const QString& request, QTcpSocket* socket)
{
    // разбиваем запрос на две части: тип товара и среднюю цену
    QStringList parts = request.split("&");
    if (parts.size() != 2)
    {
        // ошибка в запросе, отправляем сообщение об ошибке
        QString response = "Invalid request format";
        socket->write(response.toUtf8());
        socket->waitForBytesWritten();
        return;
    }

    QString type = parts[0];
    qreal averagePrice = parts[1].toDouble();

    // здесь можно добавить код для обработки запроса "min_price_in_type&type"
    // например, можно получить самые дешевые товары заданного типа из базы данных или из текстового файла

    QString response = "Min price items of type " + type + ": item 1, item 2, item 3";
    socket->write(response.toUtf8());
    socket->waitForBytesWritten();
}

void EchoThread::processExpensiveRequest(QTcpSocket* socket)
{
    // здесь можно добавить код для обработки запроса "expensive"
    // например, можно получить товары, которые дороже средней цены из базы данных или из текстового файла

    QString response = "expensive items: item 1, item 2, item 3";
    socket->write(response.toUtf8());
    socket->waitForBytesWritten();
}
