#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class TcpHandler : public QObject
{
    Q_OBJECT
public:
    explicit TcpHandler(int socketDescriptor, QObject *parent = 0);

    ~TcpHandler();

    QTcpSocket *getSocket();

signals:
    void error(QTcpSocket::SocketError socketError);


public slots:

    void disconnect();

private:

    QTcpSocket *m_socket;
};

#endif // TCPHANDLER_H
