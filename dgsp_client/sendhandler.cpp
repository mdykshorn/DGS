#include "sendhandler.h"

SendHandler::SendHandler(QObject *parent) : QObject(parent)
{

}

void SendHandler::sendMessage(QJsonObject msg)
{
    MessageState *state = new MessageState(this);
    connect(state, &MessageState::done, state, &MessageState::deleteLater);
    connect(state, &MessageState::fieldCollisionError, this, &SendHandler::fieldCollisionError);
    connect(state, &MessageState::serverFilledError, this, &SendHandler::serverFilledError);
    connect(state, &MessageState::rectangleIntersects, this, &SendHandler::rectangleIntersects);
    connect(state, &MessageState::otherIntersects, this, &SendHandler::otherIntersects);
    connect(state, &MessageState::success, this, &SendHandler::success);
    connect(state, &MessageState::userSuccess, this, &SendHandler::userSuccess);
    connect(state, &MessageState::shapeSuccess, this, &SendHandler::shapeSuccess);
    connect(state, &MessageState::gamePayloadSuccess, this, &SendHandler::gamePayloadSuccess);
    connect(state, &MessageState::sendObject, this, &SendHandler::sendObject);

    connect(this, &SendHandler::responseSend, state, &MessageState::handleResponse);
    connect(this, &SendHandler::payloadSend, state, &MessageState::handlePayload);

    state->start(msg);
}

void SendHandler::handleResponse(QJsonObject response)
{
    emit responseSend(response);
}

void SendHandler::handlePayload(QJsonObject payload)
{
    emit payloadSend(payload);
}
