#include "userstate.h"

UserState::UserState(QObject *parent) : QObject(parent)
{
    curState = INIT;
}

UserState::~UserState()
{
}


void UserState::recievedMessage(QJsonObject obj)
{
    QJsonObject payload = obj["MsgPayload"].toObject();
    switch(curState)
    {
    case INIT:
        emit initArray(payload);
        break;
    case ARRAY_RECIEVED:
        if(payload["ObjType"].toString() != "ResponsePayload")
            emit incommingObject(payload);
        break;
    case USER_SENT:
        if(payload["ObjType"].toString() == "ResponsePayload")
            emit incommingResponse(payload);
        else
        {
            emit incommingObject(payload);
            emit incommingMessage(obj);
        }
        break;
    case READY_TO_OPERATE:
        if(payload["ObjType"].toString() == "ResponsePayload")
            emit incommingResponse(payload);
        else
        {
            emit incommingObject(payload);
            emit incommingMessage(obj);
        }
    case ERROR_STATE:

        break;
    }

}

void UserState::earlyError()
{
    curState = ERROR_STATE;
}

void UserState::hasArray()
{
    curState = ARRAY_RECIEVED;
    emit createUser(m_username);
}

void UserState::sentUser()
{
    curState = USER_SENT;
}

void UserState::normalOperation()
{
    curState = READY_TO_OPERATE;
    emit startGameManager(true);
}

void UserState::getUsername(QString username)
{
    m_username = username;
}

void UserState::serverConnection(bool connected)
{
    if(!connected)
        curState = INIT;
}

void UserState::disconnectServer()
{
    curState = INIT;
}

