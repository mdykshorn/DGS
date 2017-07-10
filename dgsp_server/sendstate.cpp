#include "sendstate.h"

SendState::SendState(QObject *parent) : QObject(parent)
{
    curState = INIT;
}

SendState::~SendState()
{
}

void SendState::responsePayload(QJsonObject obj, QString responseVal)
{
    switch(curState)
    {
    case INIT:

        break;
    case ARRAY_SENT:

        break;
    case WAIT_FOR_USER:
        if(responseVal != ShapeGlobals::MessageProcessed())
        {
            emit initilizationError(ShapeGlobals::Rule4_1(), obj["MsgId"].toString());
            curState = ERROR_STATE;
        }
        else
            curState = SEND_RESPONSE;
        emit completeMessage(obj);
        break;
    case SEND_RESPONSE:
        emit completeMessage(obj);
        break;
    case ERROR_STATE:
        emit completeMessage(obj);
        emit disconnectClient();
        break;
    }
}

void SendState::messagePayload(QJsonObject payload)
{
    if(curState != INIT)
    {
        if(payload["MsgPayload"].toObject()["ObjType"].toString() != "ArrayPayload")
        {
            emit completeMessage(payload);
        }
    }
    else
        emit completeMessage(payload);
}

void SendState::systemInitiized()
{
    if(curState == INIT)
    {
        curState = ARRAY_SENT;
    }

}

void SendState::recievedMessage(QJsonObject obj)
{
    QJsonObject tempObj = obj["MsgPayload"].toObject();
    switch(curState)
    {
    case INIT:
        emit initilizationError(ShapeGlobals::Rule4_2(), obj["MsgId"].toString());
        curState = ERROR_STATE;
        break;
    case ARRAY_SENT:
        if(tempObj["ObjType"] != "UserPayload")
        {
            emit initilizationError(ShapeGlobals::Rule4_3(), obj["MsgId"].toString());
            curState = ERROR_STATE;
        }
        else
        {
            curState = WAIT_FOR_USER;
            emit incommingObject(obj);
        }
        break;
    case WAIT_FOR_USER:
        emit initilizationError(ShapeGlobals::Rule4_4(), obj["MsgId"].toString());
        curState = ERROR_STATE;
        break;
    case SEND_RESPONSE:
        emit incommingObject(obj);
        break;
    case ERROR_STATE:

        break;
    }

}

void SendState::earlyError()
{
    curState = ERROR_STATE;
}
