#include "tcphandler.h"

tcpHandler::tcpHandler(QObject *parent) : QObject(parent)
    , m_socket(new QTcpSocket(this))
{

}

QTcpSocket *tcpHandler::getSocket()
{
    return m_socket;
}

void tcpHandler::connectToServer(QString host, qint16 port)
{
    m_socket->abort();
    m_socket->connectToHost(host, port);

    if(m_socket->waitForConnected())
    {
        emit connected();
    }
}
