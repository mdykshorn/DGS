#ifndef REPOCACHE_H
#define REPOCACHE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>

#include <QDebug>

#include "globalvalues.h"

class RepoCache : public QObject
{
    Q_OBJECT
public:
    explicit RepoCache(QObject *parent = 0);

signals:

    //sends the gamestatepayload if it has one
    void sendGameState(QJsonObject state);
    //emitted if the init array has a gamestate
    void hasGameState(bool);
    //emitted for every user found on the server
    void newUser(QJsonObject user);
    //emitted for every field payload
    void newField(QJsonObject field);

    void gotArray();

    void updateDisplay(QList<QJsonObject>repository);

    //send message to clear shapes in the server
    void clearShape(QJsonObject msg);

public slots:

    void recieveInitArray(QJsonObject payload);

    void recieveMessage(QJsonObject payload);

    void clearServer();

    void clearClient();


private:


    QList<QJsonObject>m_repo;

};

#endif // REPOCACHE_H
