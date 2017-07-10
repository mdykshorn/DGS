#include "repositorymanager.h"

RepositoryManager::RepositoryManager(QObject *parent) : QObject(parent)
{
    m_uuid = new QUuid;
    rootUuid = m_uuid->createUuid().toString();
    GameMasterUuid = m_uuid->createUuid().toString();

    QJsonObject rootUser;
    QJsonObject GameMasterUser;
    QJsonObject FieldPayload;

    QJsonObject tempField;

    tempField["LowerRightX"] = 0.0;
    tempField["LowerRightY"] = 1000.0;
    tempField["UpperLeftX"] = -500.0;
    tempField["UpperLeftY"] = 500.0;

    rootUser["ObjOwnerUuid"] = rootUuid;
    rootUser["ObjType"] = QString("UserPayload");
    rootUser["ObjUuid"] = rootUuid;
    rootUser["ObjVersion"] = 1;
    rootUser["UserName"] = QString("root");
    rootUser["UserAuthorizationLevel"] = ShapeGlobals::AllPermissions();

    GameMasterUser["ObjOwnerUuid"] = rootUuid;
    GameMasterUser["ObjType"] = QString("UserPayload");
    GameMasterUser["ObjUuid"] = GameMasterUuid;
    GameMasterUser["ObjVersion"] = 1;
    GameMasterUser["UserName"] = QString("GameMaster");
    GameMasterUser["UserAuthorizationLevel"] = ShapeGlobals::GMPermissions();

    FieldPayload["ObjOwnerUuid"] = GameMasterUuid;
    FieldPayload["ObjType"] = QString("FieldPayload");
    FieldPayload["ObjUuid"] = m_uuid->createUuid().toString();
    FieldPayload["ObjVersion"] = 1;
    FieldPayload["FieldName"] = QString("Field");
    FieldPayload["FieldBoundary"] = tempField;

    QMap<QString, QVariant>Map = rootUser.toVariantMap();
    QMap<QString, QVariant>Map1 = GameMasterUser.toVariantMap();
    QMap<QString, QVariant>Map2 = FieldPayload.toVariantMap();
    m_repo.append(Map);
    m_repo.append(Map1);
    m_repo.append(Map2);

    connect(this, &RepositoryManager::responseMsg, this, &RepositoryManager::generateResponse);
}

RepositoryManager::~RepositoryManager()
{
    delete m_uuid;
}

QString RepositoryManager::getGameMasterUuid()
{
    return GameMasterUuid;
}

QString RepositoryManager::getRootUuid()
{
    return rootUuid;
}

void RepositoryManager::newObject(QJsonObject obj)
{
    QMap<QString, QVariant>tempMap = obj.toVariantMap();
    QString MsgId = tempMap["MsgId"].toString();
    QMap<QString, QVariant>objectMap = tempMap["MsgPayload"].toMap();
    QJsonObject payloadObject = QJsonObject::fromVariantMap(objectMap);

    bool found = false;
    bool ownerExists = false;

    for(int i=0; i<m_repo.size(); i++)
    {
        if(payloadObject["ObjOwnerUuid"].toString() == m_repo.at(i)["ObjOwnerUuid"].toString())
        {
            ownerExists = true;
        }
        else if(m_repo.at(i)["ObjType"].toString() == "UserPayload")
        {
            if(payloadObject["ObjOwnerUuid"].toString() == m_repo.at(i)["ObjUuid"].toString())
            {
                ownerExists = true;
            }
        }
        if(m_repo.at(i)["ObjUuid"].toString() == payloadObject["ObjUuid"].toString())
        {
            found = true;
            int version = payloadObject["ObjVersion"].toInt();
            if(version == 1)
            {
                emit responseMsg(ShapeGlobals::Rule1_1(), MsgId);
                return;
            }
            else
            {

                if(((m_repo.at(i)["ObjVersion"].toInt()+1) != version) && (version !=0))
                {
                    emit responseMsg(ShapeGlobals::Rule1_3(), MsgId);
                    return;
                }
            }
        }
    }
    if(!ownerExists)
    {
        emit responseMsg(ShapeGlobals::Rule1_8(), MsgId);
        return;
    }
    if(!found && (payloadObject["ObjVersion"].toInt() != 1))
    {
        emit responseMsg(ShapeGlobals::Rule1_2(), MsgId);
        return;
    }

    //decide what to do with object
    if(objectMap["ObjType"] == QString("UserPayload"))
    {
        handleUser(payloadObject,MsgId);
    }
    else if(objectMap["ObjType"] == QString("FieldPayload"))
    {
        handleField(payloadObject, MsgId);
    }
    else if(objectMap["ObjType"] == QString("ShapePayload"))
    {
        handleShape(payloadObject, MsgId);
    }
    else
    {
        addObject(payloadObject);
    }

}

void RepositoryManager::newConnection()
{
    QJsonObject initObject;
    QJsonObject msgPayload;
    QJsonArray initArray;

    for(int i = 0; i < m_repo.size(); i++)
    {
        QJsonObject tmpObj = QJsonObject::fromVariantMap(m_repo.at(i));
        QJsonValue tmpVal(tmpObj);
        initArray.append(tmpVal);
    }
    msgPayload["InitArray"] = initArray;
    msgPayload["ObjOwnerUuid"] = getGameMasterUuid();
    msgPayload["ObjType"] = QString("ArrayPayload");
    msgPayload["ObjUuid"] = m_uuid->createUuid().toString();
    msgPayload["ObjVersion"] = 1;

    initObject["MsgId"] = m_uuid->createUuid().toString();
    initObject["MsgPayload"] = msgPayload;

    emit dataPayload(initObject);
    emit sysInitilized();

}

void RepositoryManager::handleUser(QJsonObject userObj, QString MsgId)
{

    for(int i = 0; i< m_repo.size(); i++)
    {
        if(m_repo.at(i)["ObjType"] == "UserPayload")
        {
            if(m_repo.at(i)["UserName"].toString() == userObj["UserName"].toString())
            {
                if(!(m_repo.at(i)["ObjUuid"].toString() == userObj["ObjUuid"].toString()))
                {
                    emit responseMsg(ShapeGlobals::Rule1_4(), MsgId);
                    return;
                }

            }
        }
    }


    emit responseMsg(QString(""), MsgId);
    addObject(userObj);
}

void RepositoryManager::handleShape(QJsonObject shapeObj, QString MsgId)
{
    if(shapeObj["ObjVersion"].toInt() == 1)
    {
        if(!checkShapeContains(shapeObj))
        {
            emit responseMsg(ShapeGlobals::Rule3_1_1(), MsgId);
            return;
        }
        if(shapeObj["ShapeType"] == "Destroyer")
        {
            checkDestroyer(shapeObj, MsgId);
        }
        else
        {
            if(!checkIntersects(shapeObj))
            {
                emit responseMsg(ShapeGlobals::Rule3_2_1(), MsgId);
                return;
            }
        }
    }
    else if(shapeObj["ObjVersion"].toInt() > 1)
    {
        if(!checkShapeContains(shapeObj))
        {
            emit responseMsg(ShapeGlobals::Rule3_1_2(), MsgId);
            return;
        }
        if(shapeObj["ShapeType"].toString() == "Destroyer")
        {
            checkDestroyer(shapeObj, MsgId);
        }
        else if(shapeObj["ShapeType"].toString() == "Rectangle")
        {
            if(!checkIntersects(shapeObj))
            {
                emit responseMsg(ShapeGlobals::Rule3_2_2_Rectangle(), MsgId);
                return;
            }
        }
        else
        {
            if(!checkIntersects(shapeObj))
            {
                emit responseMsg(ShapeGlobals::Rule3_2_2_Default(), MsgId);
                return;
            }
        }
    }
    emit responseMsg(QString(""), MsgId);
    addObject(shapeObj);
}

void RepositoryManager::handleField(QJsonObject fieldObj, QString MsgId)
{
    if(fieldObj["ObjVersion"] == 1)
    {
        for(int i = 0; i< m_repo.size(); i++)
        {
            if(m_repo.at(i)["ObjType"] == "FieldPayload")
            {
                emit responseMsg(ShapeGlobals::Rule1_7(), MsgId);
                return;
            }
        }
    }
    if(!checkFieldContains(fieldObj))
    {
        emit responseMsg(ShapeGlobals::Rule1_6(), MsgId);
        return;
    }
    emit responseMsg(QString(""), MsgId);
    addObject(fieldObj);
}

void RepositoryManager::generateResponse(QString errorMessage, QString MsgId)
{
    QJsonObject responseObj;
    QJsonObject responsePayload;
    QString result;
    if(errorMessage == QString(""))
        result = ShapeGlobals::MessageProcessed();
    else
        result = ShapeGlobals::MessageRejected();

    responsePayload["ObjOwnerUuid"] = rootUuid;
    responsePayload["ObjType"] = QString("ResponsePayload");
    responsePayload["ObjUuid"] = m_uuid->createUuid().toString();
    responsePayload["ObjVersion"] = 1;
    responsePayload["ErrorMessage"] = errorMessage;
    responsePayload["ResponseToMsgId"] = MsgId;
    responsePayload["Result"] = result;

    responseObj["MsgId"] = m_uuid->createUuid().toString();
    responseObj["MsgPayload"] = responsePayload;

    emit dataResponse(responseObj, result);
}

bool RepositoryManager::checkShapeContains(QJsonObject checkObj)
{
    for(int i = 0; i<m_repo.size(); i++)
    {
        if (m_repo.at(i)["ObjType"].toString() == "FieldPayload")
        {
            QJsonObject fieldBB = QJsonObject::fromVariantMap(m_repo.at(i)["FieldBoundary"].toMap());
            QJsonObject shapeBB = checkObj["ShapeBoundingBox"].toObject();
            QRectF field(QPointF(fieldBB["UpperLeftX"].toDouble(), fieldBB["UpperLeftY"].toDouble()),
                                  QPointF(fieldBB["LowerRightX"].toDouble(), fieldBB["LowerRightY"].toDouble()));
            QRectF shape(QPointF(shapeBB["UpperLeftX"].toDouble(), shapeBB["UpperLeftY"].toDouble()),
                                  QPointF(shapeBB["LowerRightX"].toDouble(), shapeBB["LowerRightY"].toDouble()));
            return field.contains(shape);
        }
    }
    return false;
}

bool RepositoryManager::checkIntersects(QJsonObject checkObj)
{
    QJsonObject shapeBB = checkObj["ShapeBoundingBox"].toObject();
    QRectF shape(QPointF(shapeBB["UpperLeftX"].toDouble(), shapeBB["UpperLeftY"].toDouble()),
                          QPointF(shapeBB["LowerRightX"].toDouble(), shapeBB["LowerRightY"].toDouble()));
    for(int i = 0; i<m_repo.size(); i++)
    {
        if (m_repo.at(i)["ObjType"].toString() == "ShapePayload")
        {
            if(m_repo.at(i)["ObjUuid"].toString() != checkObj["ObjUuid"].toString())
            {
                QJsonObject otherShapeBB = QJsonObject::fromVariantMap(m_repo.at(i)["ShapeBoundingBox"].toMap());
                QRectF otherShape(QPointF(otherShapeBB["UpperLeftX"].toDouble(), otherShapeBB["UpperLeftY"].toDouble()),
                                      QPointF(otherShapeBB["LowerRightX"].toDouble(), otherShapeBB["LowerRightY"].toDouble()));
                if(otherShape.intersects(shape))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void RepositoryManager::checkDestroyer(QJsonObject checkObj, QString MsgId)
{
    if(checkObj["ObjVersion"].toInt() == 1)
    {
        if(!checkIntersects(checkObj))
        {
            emit responseMsg(ShapeGlobals::Rule3_2_1(), MsgId);
            return;
        }
    }
    else
    {
        QJsonObject shapeBB = checkObj["ShapeBoundingBox"].toObject();
        QRectF shape(QPointF(shapeBB["UpperLeftX"].toDouble(), shapeBB["UpperLeftY"].toDouble()),
                              QPointF(shapeBB["LowerRightX"].toDouble(), shapeBB["LowerRightY"].toDouble()));
        for(int i = 0; i<m_repo.size(); i++)
        {
            if (m_repo.at(i)["ObjType"].toString() == "ShapePayload")
            {
                if(m_repo.at(i)["ShapeType"].toString() != "Destroyer")
                {
                    QJsonObject otherShapeBB = QJsonObject::fromVariantMap(m_repo.at(i)["ShapeBoundingBox"].toMap());
                    QRectF otherShape(QPointF(otherShapeBB["UpperLeftX"].toDouble(), otherShapeBB["UpperLeftY"].toDouble()),
                                          QPointF(otherShapeBB["LowerRightX"].toDouble(), otherShapeBB["LowerRightY"].toDouble()));
                    if(otherShape.intersects(shape))
                    {
                        shapesDestroyed.append(QJsonObject::fromVariantMap(m_repo.at(i)));
                    }
                }
            }
        }
        return;
    }
}

bool RepositoryManager::checkFieldContains(QJsonObject checkObj)
{
    for(int i = 0; i<m_repo.size(); i++)
    {
        if (m_repo.at(i)["ObjType"].toString() == "FieldPayload")
        {
            QJsonObject originalFieldBB = QJsonObject::fromVariantMap(m_repo.at(i)["FieldBoundary"].toMap());
            QJsonObject fieldBB = checkObj["FieldBoundary"].toObject();
            QRectF originalField(QPointF(originalFieldBB["UpperLeftX"].toDouble(), originalFieldBB["UpperLeftY"].toDouble()),
                                  QPointF(originalFieldBB["LowerRightX"].toDouble(), originalFieldBB["LowerRightY"].toDouble()));
            QRectF field(QPointF(fieldBB["UpperLeftX"].toDouble(), fieldBB["UpperLeftY"].toDouble()),
                                  QPointF(fieldBB["LowerRightX"].toDouble(), fieldBB["LowerRightY"].toDouble()));
            return field.contains(originalField);
        }
    }
    return false;
}

void RepositoryManager::addObject(QJsonObject obj)
{
    QMap<QString, QVariant>tempMap = obj.toVariantMap();

    if(obj["ObjVersion"].toInt() == 0)
    {
        m_repo.removeOne(tempMap);
    }
    else if(obj["ObjVersion"].toInt() == 1)
    {
        m_repo.append(tempMap);
    }
    else
    {
        for(int i=0;i<m_repo.size();i++)
        {
            if(m_repo.at(i)["ObjUuid"].toString() == obj["ObjUuid"].toString())
            {
                m_repo.replace(i, tempMap);
                break;
            }
        }
        if(shapesDestroyed.size()>0)
        {
            //send shapes that were destroyed
            for(int i = 0; i<shapesDestroyed.size(); i++)
            {
                QJsonObject shapeToDelete = shapesDestroyed.at(i);
                QMap<QString, QVariant>tempDelMap = shapeToDelete.toVariantMap();
                m_repo.removeOne(tempDelMap);
                shapeToDelete["ObjVersion"] = 0;

                QJsonObject outgoingDelMsg;
                outgoingDelMsg["MsgId"] = m_uuid->createUuid().toString();
                outgoingDelMsg["MsgPayload"] = shapeToDelete;
                emit dataPayload(outgoingDelMsg);

            }
        }
        shapesDestroyed.clear();
    }


    //send message containing added object
    QJsonObject outgoingMsg;
    outgoingMsg["MsgId"] = m_uuid->createUuid().toString();
    outgoingMsg["MsgPayload"] = obj;
    emit dataPayload(outgoingMsg);

}
