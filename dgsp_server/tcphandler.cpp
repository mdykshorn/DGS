#include "tcphandler.h"

TcpHandler::TcpHandler(int socketDescriptor, QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket;

    if (!m_socket->setSocketDescriptor(socketDescriptor)) {
        emit error(m_socket->error());
        return;
    }
}

TcpHandler::~TcpHandler()
{
    delete m_socket;
}

QTcpSocket *TcpHandler::getSocket()
{
    return m_socket;
}

void TcpHandler::disconnect()
{
    m_socket->disconnectFromHost();
}
