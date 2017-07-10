#ifndef SENDHANDLER_H
#define SENDHANDLER_H

#include <QObject>

#include "messagestate.h"

class SendHandler : public QObject
{
    Q_OBJECT
public:
    explicit SendHandler(QObject *parent = 0);

signals:

    //emitted if a shape hits the field boundary
    void fieldCollisionError(QString);
    //emitted if server isn't clean of shapes
    void serverFilledError();
    //emitted if a rectangle intersects with another rectangle
    void rectangleIntersects(QString);
    //emitted if another type of shape intersects
    void otherIntersects(QString);
    //emitted when a message was succesfully sent
    void success(QString);
    //emitted when a user payload was succesfully sent
    void userSuccess(QJsonObject);
    //emitted when a shape was sucesfully updated or created
    void shapeSuccess(QJsonObject);
    //emitted when a gameStatePayload is updated or created
    void gamePayloadSuccess(QJsonObject);

    //sends out message
    void sendObject(QJsonObject);

    //sends out responses it gets
    void responseSend(QJsonObject);
    void payloadSend(QJsonObject);

public slots:

    //create a new message handler
    void sendMessage(QJsonObject msg);

    void handleResponse(QJsonObject response);

    void handlePayload(QJsonObject payload);

private:

};

#endif // SENDHANDLER_H
