#ifndef PAYLOADSENDER_H
#define PAYLOADSENDER_H

#include <QObject>
#include <QIODevice>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

class PayloadSender : public QObject
{
    Q_OBJECT
public:
    explicit PayloadSender(QIODevice *output, QObject *parent = 0);

    ~PayloadSender();

public slots:
    //send JSON object created in server
    void sendJSON(QJsonObject doc);

private:
        QIODevice *m_output;
};

#endif // PAYLOADSENDER_H
