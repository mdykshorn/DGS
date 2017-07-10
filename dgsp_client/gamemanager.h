#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>

#include "globalvalues.h"
#include "messagestate.h"


class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = 0);

    void createMyShape();

signals:

    void sendMessage(QJsonObject msg);

    //emitted once the user payload has been sent
    void userSent();

    //emitted with the current users and their scores
    void gameUsers(QJsonArray users);

    //enable the start game and join game buttons
    void enableStart(bool);
    void enableJoin(bool);

    //clear data
    void clearServerData();
    //clear client
    void clearClient();

public slots:

    void update();

    void createClientUser(QString username);
    //createObject

    void recieveField(QJsonObject field);
    //recieves the signal that determines if there is an existing gamestate
    void gameStatus(bool hasGame);

    void determineState();

    //starts a new Game as the game manager
    void startNewGame();
    //determines state of existing game
    void existingGame();

    //slots for getting message responses
    void collidesWithField(QString objID);
    void clearServer();
    void collidesWithRect(QString objID);
    void collidesWithOther(QString objID);

    void updateUser(QJsonObject obj);
    void updateShape(QJsonObject obj);
    void updateGame(QJsonObject obj);

    //handle moving the shape
    void moveMyObject(int x, int y);

    //generate rectangles when one is gone
    void generateObjectives();
    //start the game
    void startGame();

    //allow the score to update
    void scoreUpdate();


private:


    QString fieldUuid;
    QString myUserUuid;
    QString GameStateUuid;

    QJsonObject myShape;
    bool myShapeUpdate;
    QJsonObject myObjective;
    bool myObjectiveUpdate;

    QJsonObject GameState;
    bool gameExists;

    QUuid m_uuid;

    QMap<QString, QString>m_users;

    int myScore;
    //stores this clients user number - decides colors
    int userNumber;

    enum CurrentState { INIT, NEW_GAME, EXISTING_GAME, READY_TO_OPERATE, ERROR_STATE };
    CurrentState curState;

    QTimer *m_scoreTimer;
    bool scoreUp;
    QTimer *m_timer;

    int x_dir;
    int y_dir;

};

#endif // GAMEMANAGER_H
