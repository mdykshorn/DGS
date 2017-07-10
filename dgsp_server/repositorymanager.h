#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QUuid>
#include <QRectF>
#include <QPointF>

#include "globalvalues.h"

class RepositoryManager : public QObject
{
    Q_OBJECT
public:
    explicit RepositoryManager(QObject *parent = 0);

    ~RepositoryManager();

    QString getGameMasterUuid();

    QString getRootUuid();

signals:
    //recieves outgoiong response
    void dataResponse(QJsonObject obj, QString responseType);
    //recieves outgoing payload
    void dataPayload(QJsonObject obj);

    //sent once initial array has been sent
    void sysInitilized();
    //sends the response error message
    void responseMsg(QString msg, QString MsgId);



public slots:
    //when a new object is received
    void newObject(QJsonObject obj);
    //new connection created, need to send init array
    void newConnection();
    //handle user payload objects
    void handleUser(QJsonObject userObj, QString MsgId);
    //handle shape payload objects
    void handleShape(QJsonObject shapeObj, QString MsgId);
    //handle field payload objects
    void handleField(QJsonObject fieldObj, QString MsgId);
    //generates a response for each recieved message
    void generateResponse(QString errorMessage, QString MsgId);

private slots:



private:

    //shapepayload routines
    //check if shape is in playing field
    bool checkShapeContains(QJsonObject checkObj);
    //check if shape intersects with another shape
    bool checkIntersects(QJsonObject checkObj);
    //handle the destroyer shape behavior
    void checkDestroyer(QJsonObject checkObj, QString MsgId);
    //fieldpayload routines
    //check if the new field contains the old field
    bool checkFieldContains(QJsonObject checkObj);

    //add or update object to repository
    void addObject(QJsonObject obj);

    QList<QJsonObject>shapesDestroyed;

    QList<QMap<QString, QVariant>>m_repo;

    QString rootUuid;
    QString GameMasterUuid;
    QUuid *m_uuid;

};

#endif // REPOSITORYMANAGER_H
