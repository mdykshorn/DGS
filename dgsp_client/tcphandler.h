#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QObject>
#include <QTcpSocket>

class tcpHandler : public QObject
{
    Q_OBJECT
public:
    explicit tcpHandler(QObject *parent = 0);

    QTcpSocket * getSocket();

signals:
    void connected();

public slots:

    void connectToServer(QString host, qint16 port);

private:
    QTcpSocket *m_socket;
};

#endif // TCPHANDLER_H
