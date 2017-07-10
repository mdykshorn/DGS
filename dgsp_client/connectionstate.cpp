#include "connectionstate.h"

ConnectionState::ConnectionState(QObject *parent) : QObject(parent)
{
    m_handler = new tcpHandler;

    m_input = new InputParser(m_handler->getSocket());
    m_validator = new InputValidator;
    m_sender = new PayloadSender(m_handler->getSocket());

    //make necesary connections
    connect(m_handler->getSocket(), &QTcpSocket::readyRead, m_input, &InputParser::readJSONobject);

    connect(m_input, &InputParser::recievedJSON, m_validator, &InputValidator::validateJSON);
    connect(m_validator, &InputValidator::validJSON, this, &ConnectionState::recievedMessage);

    connect(m_handler, &tcpHandler::connected, this, &ConnectionState::connected);
    connect(m_handler->getSocket(), &QTcpSocket::disconnected, this, &ConnectionState::disconnectedFromServer);

    curState = INIT;
}

ConnectionState::~ConnectionState()
{
    delete m_input;
    delete m_validator;
    delete m_sender;
    delete m_handler;
}

void ConnectionState::infoEntered()
{
    curState = READY_CONNECT;
    emit enableConnect(true);
}

void ConnectionState::attemptConnection()
{
    curState = CONNECTING;
    m_handler->connectToServer(m_host, m_port);
}

void ConnectionState::connected()
{
    curState = CONNECTED;
    emit enableConnect(false);
    emit connectedToServer(true);
    emit sendUsername(m_username);
}


void ConnectionState::recievedMessage(QJsonObject obj)
{
    switch(curState)
    {
    case INIT:

        break;
    case WAIT_FOR_INFO:

        break;
    case READY_CONNECT:

        break;
    case CONNECTING:

        break;
    case CONNECTED:
        emit payload(obj);
        break;
    case ERROR_STATE:

        break;
    }
}

void ConnectionState::sendMessage(QJsonObject obj)
{
    m_sender->sendJSON(obj);
}

void ConnectionState::parseError()
{
    curState = ERROR_STATE;
}

void ConnectionState::disconnectedFromServer()
{
    curState = WAIT_FOR_INFO;
    emit connectedToServer(false);
    emit enableConnect(false);
}

void ConnectionState::attemptDisconnect()
{
    m_handler->getSocket()->disconnectFromHost();
}

void ConnectionState::getInfo(QString host, quint16 port, QString username)
{
    m_host = host;
    m_port = port;
    m_username = username;
}
