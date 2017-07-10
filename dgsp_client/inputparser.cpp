#include "inputparser.h"

InputParser::InputParser(QIODevice *input, QObject *parent) : QObject(parent)
{
    m_input = input;
    connect(this, SIGNAL(readNext()), this, SLOT(readJSONobject()));
    connect(this, SIGNAL(errorOffset(int)), this, SLOT(readSize(int)));
}

InputParser::~InputParser()
{
}

void InputParser::readJSONobject()
{

      m_input->startTransaction();

      QByteArray tempBytes = m_input->readAll();
      QJsonDocument docCreator;
      QJsonParseError error;
      int offset;

    docCreator = QJsonDocument::fromJson(tempBytes, &error);

    if(error.error == QJsonParseError::NoError)
    {
        emit recievedJSON(docCreator.object());
        m_input->commitTransaction();
    }
    else if(error.offset >= tempBytes.size())
    {
        if((error.error == QJsonParseError::DocumentTooLarge) || (error.error == QJsonParseError::DeepNesting)
            || (error.error == QJsonParseError::IllegalValue))
        {
            QUuid uuid;
            emit parsingError();
            emit parserError(ShapeGlobals::Rule0_2(error.errorString()), uuid.toString());
        }
        m_input->rollbackTransaction();
        return;
    }
    else
    {
        if((error.error == QJsonParseError::DocumentTooLarge) || (error.error == QJsonParseError::DeepNesting)
            || (error.error == QJsonParseError::IllegalValue))
        {
            QUuid uuid;
            emit parsingError();
            emit parserError(ShapeGlobals::Rule0_2(error.errorString()), uuid.toString());
        }
        offset = error.offset;
        m_input->rollbackTransaction();
        emit errorOffset(offset);

    }

}

void InputParser::readSize(int size)
{
    m_input->startTransaction();
    QByteArray tempBytes = m_input->read(size);
    QJsonDocument docCreator;
    QJsonParseError error;

    docCreator = QJsonDocument::fromJson(tempBytes, &error);

    if(error.error == QJsonParseError::NoError)
    {
        emit recievedJSON(docCreator.object());
        m_input->commitTransaction();
        emit readNext();
    }
    else
    {
        if((error.error == QJsonParseError::DocumentTooLarge) || (error.error == QJsonParseError::DeepNesting))
        {
            QUuid uuid;
            emit parsingError();
            emit parserError(ShapeGlobals::Rule0_2(error.errorString()), uuid.toString());
        }
        m_input->rollbackTransaction();
    }

}


