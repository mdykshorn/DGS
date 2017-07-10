#ifndef SENDSTATE_H
#define SENDSTATE_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>

#include "globalvalues.h"

class SendState : public QObject
{
    Q_OBJECT
public:
    explicit SendState(QObject *parent = 0);
    ~SendState();
signals:
    void error();
    void success();
    //sends message to output parser
    void completeMessage(QJsonObject msg);
    //send incoming object to repo manager
    void incommingObject(QJsonObject obj);
    //error during initilization
    void initilizationError(QString errorMsg, QString MsgId);
    //disconnect client
    void disconnectClient();


public slots:
    //recieves the response message to send
    void responsePayload(QJsonObject obj, QString responseVal);
    //recieves the payload to send
    void messagePayload(QJsonObject payload);
    //waits for initial array send
    void systemInitiized();
    //new messge recieved in the TCP socket
    void recievedMessage(QJsonObject obj);
    //recieves errors from the parsing and validating
    void earlyError();

private:

    enum CurrentState { INIT, ARRAY_SENT, WAIT_FOR_USER, SEND_RESPONSE, ERROR_STATE };
    CurrentState curState;

};

#endif // SENDSTATE_H
