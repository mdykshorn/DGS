#include <QtWidgets>
#include "guiclient.h"

GuiClient::GuiClient(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);

    ConnectionState *connectState = new ConnectionState(this);
    UserState *userState = new UserState(this);
    SendHandler *sender = new SendHandler(this);

    RepoCache *repository = new RepoCache(this);
    GameManager *manager = new GameManager(this);

    NetworkClient *network = new NetworkClient(this);
    GameControl *control = new GameControl(this);
    GameStats *stats = new GameStats(this);
    GameDisplay *display = new GameDisplay(this);
    ShapeControl *shapeCont = new ShapeControl(this);

    layout->addWidget(network,0,0);
    layout->addWidget(control, 0,1);
    layout->addWidget(stats, 1,1);
    layout->addWidget(display, 1, 0);
    layout->addWidget(shapeCont, 2, 0);


    setLayout(layout);

    //update the drawing
    connect(repository, &RepoCache::updateDisplay, display, &GameDisplay::updateField);

    //handle server connections
    connect(control, &GameControl::quitClient, this, &QWidget::close);
    connect(control, &GameControl::connectToServer, connectState, &ConnectionState::attemptConnection);
    connect(control, &GameControl::disconnectFromServer, connectState, &ConnectionState::attemptDisconnect);
    connect(control, &GameControl::disconnectFromServer, manager, &GameManager::clearServer);
    connect(manager, &GameManager::clearServerData, repository, &RepoCache::clearServer);
    connect(manager, &GameManager::clearClient, repository, &RepoCache::clearClient);
    connect(control, &GameControl::quitClient, control, &GameControl::disconnectFromServer);

    //connection state and userstate
    connect(network, &NetworkClient::sendInfo, connectState, &ConnectionState::getInfo);

    connect(connectState, &ConnectionState::payload, userState, &UserState::recievedMessage);
    connect(userState, &UserState::initArray, repository, &RepoCache::recieveInitArray);
    connect(userState, &UserState::incommingObject, repository, &RepoCache::recieveMessage);
    connect(connectState, &ConnectionState::connectedToServer, userState, &UserState::serverConnection);


    connect(connectState, &ConnectionState::sendUsername, userState, &UserState::getUsername);
    connect(userState, &UserState::createUser, manager, &GameManager::createClientUser);

    connect(repository, &RepoCache::gotArray, userState, &UserState::hasArray);

    connect(manager, &GameManager::userSent, userState, &UserState::sentUser);

    connect(shapeCont, &ShapeControl::moveShape, manager, &GameManager::moveMyObject);

    //connect send handler
    connect(userState, &UserState::incommingResponse, sender, &SendHandler::handleResponse);
    connect(userState, &UserState::incommingMessage, sender, &SendHandler::handlePayload);
    connect(sender, &SendHandler::sendObject, connectState, &ConnectionState::sendMessage);
    connect(repository, &RepoCache::clearShape, connectState, &ConnectionState::sendMessage);
    connect(manager, &GameManager::sendMessage, sender, &SendHandler::sendMessage);
    connect(sender, &SendHandler::fieldCollisionError, manager, &GameManager::collidesWithField);
    connect(sender, &SendHandler::serverFilledError, manager, &GameManager::clearServer);
    connect(sender, &SendHandler::rectangleIntersects, manager, &GameManager::collidesWithRect);
    connect(sender, &SendHandler::otherIntersects, manager, &GameManager::collidesWithOther);
    connect(sender, &SendHandler::userSuccess, manager, &GameManager::updateUser);
    connect(sender, &SendHandler::shapeSuccess, manager, &GameManager::updateShape);
    connect(sender, &SendHandler::gamePayloadSuccess, manager, &GameManager::updateGame);

    connect(sender, &SendHandler::userSuccess, userState, &UserState::normalOperation);

    //connect repo to manager
    connect(repository, &RepoCache::sendGameState, manager, &GameManager::updateGame);
    connect(repository, &RepoCache::hasGameState, manager, &GameManager::gameStatus);
    connect(repository, &RepoCache::newUser, manager, &GameManager::updateUser);
    connect(repository, &RepoCache::newField, manager, &GameManager::recieveField);


    connect(userState, &UserState::startGameManager, manager, &GameManager::determineState);
    connect(control, &GameControl::startGame, manager, &GameManager::startNewGame);
    connect(control, &GameControl::joinGame, manager, &GameManager::existingGame);
    connect(manager, &GameManager::enableStart, control, &GameControl::enableStart);
    connect(manager, &GameManager::enableJoin, control, &GameControl::enableJoin);

    //enabling the connect button
    connect(network, &NetworkClient::enableConnect, connectState, &ConnectionState::infoEntered);
    connect(connectState, &ConnectionState::enableConnect, control, &GameControl::enableConnect);

    //enabling disconnect button
    connect(connectState, &ConnectionState::connectedToServer, control, &GameControl::enableDisconnect);
    connect(connectState, &ConnectionState::connectedToServer, shapeCont, &ShapeControl::enableButtons);

    //display
    connect(connectState, &ConnectionState::connectedToServer, stats, &GameStats::connectionStatus);
    connect(connectState, &ConnectionState::serverError, stats, &GameStats::showError);
    connect(manager, &GameManager::gameUsers, stats, &GameStats::updateUsers);
}

GuiClient::~GuiClient()
{

}
