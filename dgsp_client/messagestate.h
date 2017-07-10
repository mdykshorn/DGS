#ifndef MESSAGESTATE_H
#define MESSAGESTATE_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>

class MessageState : public QObject
{
    Q_OBJECT
public:
    explicit MessageState(QObject *parent = 0);

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

    //emitted when the message was sent a response was recieved
    void done();
    //send message to the output parser
    void sendObject(QJsonObject);

public slots:

    void handleResponse(QJsonObject response);

    void handlePayload(QJsonObject payload);

    void start(QJsonObject msg);

private:

    enum CurrentState { INIT, MSG_SENT, WAIT_FOR_RESPONSE, MSG_COMPLETE, ERROR_STATE };
    CurrentState curState;

    QString m_messageID;
    QString m_objectID;
    int m_objectVersion;

    bool isDone;
};

#endif // MESSAGESTATE_H
