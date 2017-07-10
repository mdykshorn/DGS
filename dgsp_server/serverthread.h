#ifndef FORTUNETHREAD_H
#define FORTUNETHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDataStream>


#include "inputparser.h"
#include "inputvalidator.h"
#include "payloadsender.h"
#include "sendstate.h"
#include "repositorymanager.h"
#include "tcphandler.h"


class ServerThread : public QThread
{
    Q_OBJECT

public:
    ServerThread(RepositoryManager *repo, int socketDescriptor, QObject *parent);

    ~ServerThread();

    void run() Q_DECL_OVERRIDE;


signals:

    void newConnection();

private:
    int socketDescriptor;
    RepositoryManager *m_repository;
};

#endif
