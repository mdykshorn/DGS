#include "gameserver.h"
#include "serverthread.h"

#include <stdlib.h>

GameServer::GameServer(QObject *parent)
    : QTcpServer(parent)
{
    m_rManager = new RepositoryManager;

}

GameServer::~GameServer()
{
    delete m_rManager;
    delete m_sendManager;
}


void GameServer::incomingConnection(qintptr socketDescriptor)
{
    ServerThread *thread = new ServerThread(m_rManager, socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
