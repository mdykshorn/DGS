#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>
#include <QDebug>

#include "globalvalues.h"

class InputValidator : public QObject
{
    Q_OBJECT
public:
    explicit InputValidator(QObject *parent = 0);

    ~InputValidator();

signals:

    void validJSON(QJsonObject doc);

    //emits errors specific to the document validity per spec
    void validatorError(QString errorMsg, QString MsgId);
    //emitted upon error occurance
    void formatError();

public slots:
    //takes in a complete JSON object for validation
    void validateJSON(QJsonObject doc);



};

#endif // INPUTVALIDATOR_H
