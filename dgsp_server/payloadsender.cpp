#include "payloadsender.h"

PayloadSender::PayloadSender(QIODevice *output, QObject *parent) : QObject(parent)
{
    m_output = output;
}

PayloadSender::~PayloadSender()
{
}

void PayloadSender::sendJSON(QJsonObject doc)
{
    QJsonDocument docSender(doc);
    QByteArray tempArray;
    tempArray = docSender.toJson();

    m_output->write(tempArray);
}
