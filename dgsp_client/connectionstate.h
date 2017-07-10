#ifndef CONNECTIONSTATE_H
#define CONNECTIONSTATE_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>

#include "inputparser.h"
#include "inputvalidator.h"
#include "tcphandler.h"
#include "payloadsender.h"

class ConnectionState : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionState(QObject *parent = 0);

    ~ConnectionState();

signals:

    void payload(QJsonObject payload);

    void enableConnect(bool);

    void connectedToServer(bool);

    void serverError(QString);

    void sendUsername(QString);

public slots:

    void infoEntered();

    void attemptConnection();

    void connected();

    //new messge recieved in the TCP socket
    void recievedMessage(QJsonObject obj);

    //send message
    void sendMessage(QJsonObject obj);

    void parseError();

    void disconnectedFromServer();

    void attemptDisconnect();

    void getInfo(QString host, quint16 port, QString username);

private:

    enum CurrentState { INIT, WAIT_FOR_INFO, READY_CONNECT, CONNECTING, CONNECTED, ERROR_STATE };
    CurrentState curState;

    InputParser *m_input;
    InputValidator *m_validator;
    tcpHandler *m_handler;
    PayloadSender *m_sender;

    QString m_host;
    QString m_username;
    quint16 m_port;
};

#endif // CONNECTIONSTATE_H
