#include "repocache.h"

RepoCache::RepoCache(QObject *parent) : QObject(parent)
{

}

void RepoCache::recieveInitArray(QJsonObject payload)
{
    bool hasGamePayload = false;

    if(payload["ObjType"].toString() != "ArrayPayload")
        qDebug() << "not initial array";
    else
    {

        QJsonArray array = payload["InitArray"].toArray();

        for(int i = 0; i<array.size(); i++)
        {
            QJsonObject tempObj = array.at(i).toObject();
            if(tempObj["ObjType"].toString() == "UserPayload")
            {
                emit newUser(tempObj);
            }
            else if(tempObj["ObjType"].toString() == "FieldPayload")
            {
                emit newField(tempObj);
            }
            else if(tempObj["ObjType"].toString() == "GameStatePayload")
            {
                hasGamePayload = true;
                emit sendGameState(tempObj);
            }
            m_repo.append(tempObj);
        }
        emit hasGameState(hasGamePayload);
        emit gotArray();
        emit updateDisplay(m_repo);
    }
}


void RepoCache::recieveMessage(QJsonObject payload)
{

    bool found = false;

    if(payload["ObjType"].toString() == "UserPayload")
    {
        emit newUser(payload);
    }
    else if(payload["ObjType"].toString() == "GameStatePayload")
    {
        emit sendGameState(payload);
    }

    for(int i = 0; i< m_repo.size(); i++)
    {
        if(payload["ObjUuid"].toString() == m_repo.at(i)["ObjUuid"].toString())
        {
            found = true;
            if(payload["ObjVersion"].toInt() == 0)
                m_repo.removeAt(i);
            else
                m_repo.replace(i, payload);

            break;
        }
    }
    if(!found)
    {
        m_repo.append(payload);
    }
    emit updateDisplay(m_repo);
}

void RepoCache::clearServer()
{
    QJsonObject myObj;
    QUuid uuid;
    for(int i = 0; i<m_repo.size(); i++)
    {
        if((m_repo.at(i)["ObjType"].toString() == "ShapePayload") ||
                (m_repo.at(i)["ObjType"].toString() == "GameStatePayload"))
        {
            myObj = m_repo.at(i);
            myObj["ObjVersion"] = 0;

            QJsonObject msg1;
            msg1["MsgId"] = uuid.createUuid().toString();
            msg1["MsgPayload"] = myObj;
            emit clearShape(msg1);
        }
    }
    m_repo.clear();
    emit updateDisplay(m_repo);
}

void RepoCache::clearClient()
{
    m_repo.clear();
    emit updateDisplay(m_repo);
}


