#include "inputvalidator.h"

InputValidator::InputValidator(QObject *parent) : QObject(parent)
{

}

InputValidator::~InputValidator()
{
}

void InputValidator::validateJSON(QJsonObject doc)
{
    QUuid uuid;
    if(!doc.contains("MsgId"))
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), uuid.toString());
        return;
    }
    if(!doc["MsgId"].isString())
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), uuid.toString());
        return;
    }
    if(!doc.contains("MsgPayload"))
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
        return;
    }
    if(!doc["MsgPayload"].isObject())
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
        return;
    }
    QJsonObject payloadObj = doc["MsgPayload"].toObject();
    if(!payloadObj.contains("ObjType"))
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
        return;
    }
    if(!payloadObj["ObjType"].isString())
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
        return;
    }
    if(!payloadObj.contains("ObjUuid"))
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
        return;
    }
    if(!payloadObj["ObjUuid"].isString())
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
        return;
    }
    if(!payloadObj.contains("ObjOwnerUuid"))
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
        return;
    }
    if(!payloadObj["ObjOwnerUuid"].isString())
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
        return;
    }
    if(!payloadObj.contains("ObjVersion"))
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
        return;
    }
    if(payloadObj["ObjVersion"].toInt() < 0)
    {
        emit formatError();
        emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
        return;
    }
    //validation for additional objects
    if(payloadObj["ObjType"].toString() == "UserPayload")
    {
        if(!payloadObj.contains("UserName"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!payloadObj["UserName"].isString())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!payloadObj.contains("UserAuthorizationLevel"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(payloadObj["ObjVersion"].toInt() < 0)
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
    }
    else if(payloadObj["ObjType"].toString() == "ShapePayload")
    {
        if(!payloadObj.contains("ShapeType"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!payloadObj["ShapeType"].isString())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!payloadObj.contains("ShapeBoundingBox"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!payloadObj["ShapeBoundingBox"].isObject())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        QJsonObject boundingBox = payloadObj["ShapeBoundingBox"].toObject();
        if(!boundingBox.contains("LowerRightX"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox["LowerRightX"].isDouble())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox.contains("LowerRightY"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox["LowerRightY"].isDouble())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox.contains("UpperLeftX"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox["UpperLeftX"].isDouble())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox.contains("UpperLeftY"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox["UpperLeftY"].isDouble())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
    }
    else if(payloadObj["ObjType"].toString() == "FieldPayload")
    {
        if(!payloadObj.contains("FieldName"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!payloadObj["FieldName"].isString())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!payloadObj.contains("FieldBoundary"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!payloadObj["FieldBoundary"].isObject())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        QJsonObject boundingBox = payloadObj["FieldBoundary"].toObject();
        if(!boundingBox.contains("LowerRightX"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox["LowerRightX"].isDouble())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox.contains("LowerRightY"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox["LowerRightY"].isDouble())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox.contains("UpperLeftX"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox["UpperLeftX"].isDouble())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox.contains("UpperLeftY"))
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
        if(!boundingBox["UpperLeftY"].isDouble())
        {
            emit formatError();
            emit validatorError(ShapeGlobals::Rule0_1(), doc["MsgId"].toString());
            return;
        }
    }

    emit validJSON(doc);
}
