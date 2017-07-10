#include "serverthread.h"

#include <QtNetwork>

ServerThread::ServerThread(RepositoryManager *repo, int socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor), m_repository(repo)
{

}


ServerThread::~ServerThread()
{
}

void ServerThread::run()
{
    SendState sendManager;

    TcpHandler tcpHandler(socketDescriptor);

    InputParser parser(tcpHandler.getSocket());
    InputValidator validator;
    PayloadSender sender(tcpHandler.getSocket());



    connect(tcpHandler.getSocket(), &QTcpSocket::disconnected, this, &QThread::quit);
    connect(&sendManager, &SendState::disconnectClient, &tcpHandler, &TcpHandler::disconnect);



    //repo and sendstate connections
    connect(&sendManager, &SendState::incommingObject, m_repository, &RepositoryManager::newObject);
    connect(m_repository, &RepositoryManager::dataResponse, &sendManager, &SendState::responsePayload);
    connect(m_repository, &RepositoryManager::dataPayload, &sendManager, &SendState::messagePayload);
    connect(m_repository, &RepositoryManager::sysInitilized, &sendManager, &SendState::systemInitiized);
    connect(&sendManager, &SendState::initilizationError, m_repository, &RepositoryManager::generateResponse);

    //connections for responses outside of the main repo
    connect(&parser, SIGNAL(parserError(QString,QString)), m_repository, SLOT(generateResponse(QString,QString)));
    connect(&validator, SIGNAL(validatorError(QString,QString)), m_repository, SLOT(generateResponse(QString,QString)));
    connect(&parser, SIGNAL(parsingError()), &sendManager, SLOT(earlyError()));
    connect(&validator, SIGNAL(formatError()), &sendManager, SLOT(earlyError()));

    //connect socket read signals to parser
    connect(tcpHandler.getSocket(), &QTcpSocket::readyRead, &parser, &InputParser::readJSONobject);
    //connect the inputs
    connect(&parser, SIGNAL(recievedJSON(QJsonObject)), &validator, SLOT(validateJSON(QJsonObject)));
    connect(&validator, SIGNAL(validJSON(QJsonObject)), &sendManager, SLOT(recievedMessage(QJsonObject)));
    //connect the output
    connect(&sendManager, SIGNAL(completeMessage(QJsonObject)), &sender, SLOT(sendJSON(QJsonObject)));


    connect(this, &ServerThread::newConnection, m_repository, &RepositoryManager::newConnection);

    emit newConnection();
    exec();
}

