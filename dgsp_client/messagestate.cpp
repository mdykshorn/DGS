#include "messagestate.h"
#include "globalvalues.h"
MessageState::MessageState(QObject *parent) : QObject(parent)
{
    isDone = false;
}

void MessageState::handleResponse(QJsonObject response)
{

    //match to message
    if(response["ResponseToMsgId"].toString() == m_messageID)
    {
        //emit correct signal depending on response
        if(response["ErrorMessage"].toString() == "")
        {
            emit success(m_objectID);
            if(isDone)
                emit done();
            else
                isDone = true;
        }
        else if(response["ErrorMessage"].toString() == ShapeGlobals::Rule3_1_1() )  //creating outside boundary
        {
            emit done();
        }
        else if(response["ErrorMessage"].toString() == ShapeGlobals::Rule3_1_2() )  //updated to not contained in boundary
        {
            emit fieldCollisionError(m_objectID);
            emit done();
        }
        else if(response["ErrorMessage"].toString() == ShapeGlobals::Rule3_2_1())   //create intersecting shape
        {
                emit done();
        }
        else if(response["ErrorMessage"].toString() == ShapeGlobals::Rule3_2_2_Rectangle() )    //update to intersecting location
        {
            emit rectangleIntersects(m_objectID);
            emit done();
        }
        else if(response["ErrorMessage"].toString() == ShapeGlobals::Rule3_2_2_Default() )  //update to intersecting location
        {
            emit otherIntersects(m_objectID);
            emit done();
        }
    }
}

void MessageState::handlePayload(QJsonObject payload)
{
    QJsonObject tempPay = payload["MsgPayload"].toObject();
    //make sure object matches
    if(tempPay["ObjUuid"].toString() == m_objectID)
    {
        if(tempPay["ObjType"].toString() == "UserPayload")
        {
            emit userSuccess(tempPay);
        }
        else if(tempPay["ObjType"].toString() == "ShapePayload")
        {
            emit shapeSuccess(tempPay);
        }
        else if(tempPay["ObjType"].toString() == "GameStatePayload")
        {
            emit gamePayloadSuccess(tempPay);
        }
        if(isDone)
            emit done();
        else
            isDone = true;
    }
}

void MessageState::start(QJsonObject msg)
{
    m_messageID = msg["MsgId"].toString();
    m_objectID = msg["MsgPayload"].toObject()["ObjUuid"].toString();
    m_objectVersion = msg["MsgPayload"].toObject()["ObjVersion"].toInt();

    emit sendObject(msg);
}

