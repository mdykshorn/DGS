#ifndef FORTUNESERVER_H
#define FORTUNESERVER_H

#include <QStringList>
#include <QTcpServer>

#include "repositorymanager.h"
#include "sendstate.h"

class GameServer : public QTcpServer
{
    Q_OBJECT

public:
    GameServer(QObject *parent = 0);

    ~GameServer();


signals:
        void returnCommand();


protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private:

    RepositoryManager *m_rManager;
    SendState *m_sendManager;

};

#endif
