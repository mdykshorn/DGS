#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <QObject>
#include <QIODevice>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>
#include <QDebug>

#include "globalvalues.h"

class InputParser : public QObject
{
    Q_OBJECT
public:
    explicit InputParser(QIODevice *input, QObject *parent = 0);

    ~InputParser();

signals:
    //complete document that was read in from the IO Device
    void recievedJSON(QJsonObject doc);

    //sent to keep reading JSON objects if possible, similar to readyRead signal
    void readNext();

    //errors
    void errorOffset(int offset);
    //emitted specific to parsing errors
    void parserError(QString errorMsg, QString MsgId);
    //emitted when an error occurs
    void parsingError();

public slots:
    //connect to ready read signal to attempt to read in a JSON object
    void readJSONobject();

    //read up to the point of an error in a JSON object
    void readSize(int size);

private:
    QIODevice *m_input;
};

#endif // INPUTPARSER_H
