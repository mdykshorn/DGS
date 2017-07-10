#ifndef SENDSTATE_H
#define SENDSTATE_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>

#include "globalvalues.h"


class UserState : public QObject
{
    Q_OBJECT
public:
    explicit UserState(QObject *parent = 0);
    ~UserState();

signals:

    void initArray(QJsonObject initArray);

    //send incoming object to repo manager
    void incommingObject(QJsonObject obj);
    //emitted when a response payload is recieved
    void incommingResponse(QJsonObject response);
    void incommingMessage(QJsonObject message);

    void createUser(QString);

    //emitted once the game can be started
    void startGameManager(bool);



public slots:

    //new messge recieved from the connection manager
    void recievedMessage(QJsonObject obj);
    //recieves errors from the parsing and validating
    void earlyError();

    void hasArray();

    void sentUser();

    void normalOperation();

    void getUsername(QString username);

    void serverConnection(bool connected);

    void disconnectServer();

private:

    enum CurrentState { INIT, ARRAY_RECIEVED, USER_SENT, READY_TO_OPERATE, ERROR_STATE };
    CurrentState curState;

    QString m_username;

};

#endif // SENDSTATE_H
