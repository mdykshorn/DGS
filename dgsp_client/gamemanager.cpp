#include "gamemanager.h"

GameManager::GameManager(QObject *parent) : QObject(parent)
{

    curState = INIT;

    m_timer = new QTimer(this);
    m_scoreTimer = new QTimer(this);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(m_scoreTimer, SIGNAL(timeout()), this, SLOT(scoreUpdate()));
    m_scoreTimer->start(1000);

    x_dir = 0;
    y_dir = 0;
    myScore = 0;
    userNumber = 0;
    gameExists = false;
    myShapeUpdate = true;
    myObjectiveUpdate = true;
    scoreUp = true;
}

void GameManager::createMyShape()
{
    QJsonObject tempShape;
    QJsonObject msg;

    switch(userNumber)
    {
    case 1:
        tempShape["LowerRightX"] = -449;
        tempShape["LowerRightY"] = 750;
        tempShape["UpperLeftX"] = -499;
        tempShape["UpperLeftY"] = 700;
        break;
    case 2:
        tempShape["LowerRightX"] = -250;
        tempShape["LowerRightY"] = 999;
        tempShape["UpperLeftX"] = -300;
        tempShape["UpperLeftY"] = 949;
        break;
    case 3:
        tempShape["LowerRightX"] = -1;
        tempShape["LowerRightY"] = 750;
        tempShape["UpperLeftX"] = -51;
        tempShape["UpperLeftY"] = 700;
        break;
    case 4:
        tempShape["LowerRightX"] = -250;
        tempShape["LowerRightY"] = 51;
        tempShape["UpperLeftX"] = -300;
        tempShape["UpperLeftY"] = 1;
        break;
    }

    myShape["ObjOwnerUuid"] = myUserUuid;
    myShape["ObjType"] = QString("ShapePayload");
    myShape["ObjUuid"] = m_uuid.createUuid().toString();;
    myShape["ObjVersion"] = 1;
    myShape["ShapeType"] = QString("Rectangle");
    myShape["ShapeBoundingBox"] = tempShape;

    QJsonObject myobj;
    myobj["UserNumber"] = userNumber;

    myShape["ClientData"] = myobj;

    msg["MsgId"] = m_uuid.createUuid().toString();
    msg["MsgPayload"] = myShape;

    emit sendMessage(msg);
}

void GameManager::update()
{
    if(myShapeUpdate)
    {
        myShapeUpdate = false;
        QJsonObject msg;

        myShape["ObjVersion"] = myShape["ObjVersion"].toInt() + 1;

        QJsonObject shape = myShape["ShapeBoundingBox"].toObject();

        shape["LowerRightX"] = shape["LowerRightX"].toDouble() + (x_dir*10);
        shape["UpperLeftX"] = shape["UpperLeftX"].toDouble() + (x_dir*10);
        shape["LowerRightY"] = shape["LowerRightY"].toDouble()+ (10*y_dir);
        shape["UpperLeftY"] = shape["UpperLeftY"].toDouble() + (10*y_dir);

        myShape["ShapeBoundingBox"] = shape;

        msg["MsgId"] = m_uuid.createUuid().toString();
        msg["MsgPayload"] = myShape;

        emit sendMessage(msg);
    }


}


void GameManager::recieveField(QJsonObject field)
{

    fieldUuid = field["ObjUuid"].toString();
}


void GameManager::gameStatus(bool game)
{
    gameExists = game;
}

void GameManager::determineState()
{
    if(gameExists)
    {
        emit enableJoin(true);
        emit enableStart(false);
    }
    else
    {
        emit enableJoin(false);
        emit enableStart(true);
    }
}

void GameManager::startNewGame()
{
    GameState["ObjOwnerUuid"] = m_users.key("root");
    GameState["ObjType"] = "GameStatePayload";
    GameState["ObjUuid"] = m_uuid.createUuid().toString();
    GameState["ObjVersion"] = 1;
    GameState["Status"] = "WAIT_START";

    QJsonArray gameUserArray;

    QJsonObject user;

    userNumber = 1;

    user["UserNumber"] = userNumber;
    user["UserName"] = m_users[myUserUuid];
    user["Score"] = myScore;

    gameUserArray.append(user);

    GameState["Users"] = gameUserArray;

    emit gameUsers(gameUserArray);
    QJsonObject msg;
    msg["MsgId"] = m_uuid.createUuid().toString();
    msg["MsgPayload"] = GameState;

    emit sendMessage(msg);

    startGame();

}

void GameManager::existingGame()
{
    QJsonArray gameUserArray = GameState["Users"].toArray();

    QJsonObject user;

    userNumber = gameUserArray.size()+1;

    user["UserNumber"] = userNumber;
    user["UserName"] = m_users[myUserUuid];
    user["Score"] = myScore;

    gameUserArray.append(user);

    GameState["Users"] = gameUserArray;

    QJsonArray tempArray;
    tempArray = GameState["Users"].toArray();

    emit gameUsers(tempArray);

    if(GameState["Status"].toString() == "PLAY")
    {
        createMyShape();
        m_timer->start(20);
    }
    GameState["ObjVersion"] =  GameState["ObjVersion"].toInt() +1;

    QJsonObject msg;
    msg["MsgId"] = m_uuid.createUuid().toString();
    msg["MsgPayload"] = GameState;

    emit sendMessage(msg);
}

void GameManager::collidesWithField(QString objID)
{
    if(objID == myShape["ObjUuid"].toString())
    {
        if(scoreUp)
        {
            myScore--;
            scoreUp = false;
        }
        myShapeUpdate = true;
        myShape["ObjVersion"] = myShape["ObjVersion"].toInt() - 1;

        QJsonArray userArray = GameState["Users"].toArray();
        for(int i = 0; i<userArray.size(); i++)
        {
            QJsonObject tempUser = userArray.at(i).toObject();
            if(tempUser["UserName"].toString() == m_users[myUserUuid])
            {
                tempUser["Score"] = myScore;
                userArray.replace(i, tempUser);
                break;
            }
        }
        GameState["Users"] = userArray;
        GameState["ObjVersion"] =  GameState["ObjVersion"].toInt() +1;
        emit gameUsers(userArray);
        QJsonObject msg1;
        msg1["MsgId"] = m_uuid.createUuid().toString();
        msg1["MsgPayload"] = GameState;

        emit sendMessage(msg1);

    }
}

void GameManager::clearServer()
{
    m_users.clear();
    QJsonArray clear;
    if(userNumber == 1)
    {
        emit gameUsers(clear);
        emit clearServerData();
    }
    else
    {
        emit gameUsers(clear);
        emit clearClient();
    }
    x_dir = 0;
    y_dir = 0;
    myScore = 0;
    userNumber = 0;
    gameExists = false;
    myShapeUpdate = true;
    myObjectiveUpdate = true;
    scoreUp = true;
}

void GameManager::collidesWithRect(QString objID)
{
    if(objID == myShape["ObjUuid"].toString())
    {
        myShapeUpdate = true;
        myShape["ObjVersion"] = myShape["ObjVersion"].toInt() - 1;
        if(scoreUp)
        {
            myScore++;
            scoreUp = false;
        }

        QJsonArray userArray = GameState["Users"].toArray();
        for(int i = 0; i<userArray.size(); i++)
        {
            QJsonObject tempUser = userArray.at(i).toObject();
            if(tempUser["UserName"].toString() == m_users[myUserUuid])
            {
                tempUser["Score"] = myScore;
                userArray.replace(i, tempUser);
                break;
            }
        }
        GameState["Users"] = userArray;

        if(userNumber == 1)
        {
            generateObjectives();
        }
        else
        {
            GameState["Collision"] = true;
        }
        emit gameUsers(userArray);
        GameState["ObjVersion"] =  GameState["ObjVersion"].toInt() +1;
        QJsonObject msg1;
        msg1["MsgId"] = m_uuid.createUuid().toString();
        msg1["MsgPayload"] = GameState;
        emit sendMessage(msg1);

    }
    else if(objID == myObjective["ObjUuid"].toString())
    {
        myObjectiveUpdate = true;
        myObjective["ObjVersion"] = myObjective["ObjVersion"].toInt() - 1;
    }


}

void GameManager::collidesWithOther(QString objID)
{

    if(objID == myObjective["ObjUuid"].toString())
    {
        myObjectiveUpdate = true;
        myObjective["ObjVersion"] = myObjective["ObjVersion"].toInt() - 1;
        generateObjectives();
    }
}

void GameManager::updateUser(QJsonObject obj)
{
    m_users[obj["ObjUuid"].toString()] = obj["UserName"].toString();
}

void GameManager::updateShape(QJsonObject obj)
{
    if(obj["ObjUuid"].toString() == myShape["ObjUuid"].toString())
    {
        myShape["ObjVersion"] = obj["ObjVersion"].toInt();
        myShapeUpdate = true;
    }
    else if(obj["ObjUuid"].toString() == myObjective["ObjUuid"].toString())
    {
        myObjective["ObjVersion"] = obj["ObjVersion"].toInt();
        myObjectiveUpdate = true;
    }

}

void GameManager::updateGame(QJsonObject obj)
{
    GameStateUuid = obj["ObjUuid"].toString();
    GameState = obj;

    QJsonArray userArray = GameState["Users"].toArray();
    emit gameUsers(userArray);
    if(GameState["Collision"].toBool())
    {
        generateObjectives();
        GameState["Collision"] = false;
        QJsonObject msg1;
        GameState["ObjVersion"] =  GameState["ObjVersion"].toInt() +1;
        msg1["MsgId"] = m_uuid.createUuid().toString();
        msg1["MsgPayload"] = GameState;

        emit sendMessage(msg1);
    }

}

void GameManager::moveMyObject(int x, int y)
{
    x_dir = x;
    y_dir = y;
}

void GameManager::generateObjectives()
{
    if(myObjectiveUpdate)
    {
        myObjectiveUpdate = false;
        if(userNumber == 1)
        {
            qsrand(m_scoreTimer->remainingTime());
            QJsonObject msg1;
            qreal xpos = qrand()%(481) - 480;
            qreal ypos = qrand()%(499) + 500;
            QJsonObject tempShape;

            tempShape["LowerRightX"] = xpos;
            tempShape["LowerRightY"] = ypos;
            tempShape["UpperLeftX"] = (xpos - 20);
            tempShape["UpperLeftY"] = (ypos + 20);


            myObjective["ObjVersion"] = (myObjective["ObjVersion"].toInt()+1);
            myObjective["ShapeBoundingBox"] = tempShape;

            msg1["MsgId"] = m_uuid.createUuid().toString();
            msg1["MsgPayload"] = myObjective;

            emit sendMessage(msg1);
        }
    }

}

void GameManager::startGame()
{
    if(userNumber == 1)
    {
        QJsonObject msg1;
        QJsonObject tempShape;

        tempShape["LowerRightX"] = -150;
        tempShape["LowerRightY"] = 720;
        tempShape["UpperLeftX"] = -170;
        tempShape["UpperLeftY"] = 700;

        myObjective["ObjOwnerUuid"] = myUserUuid;
        myObjective["ObjType"] = QString("ShapePayload");
        myObjective["ObjUuid"] = m_uuid.createUuid().toString();
        myObjective["ObjVersion"] = 1;
        myObjective["ShapeType"] = QString("Objective");
        myObjective["ShapeBoundingBox"] = tempShape;

        msg1["MsgId"] = m_uuid.createUuid().toString();
        msg1["MsgPayload"] = myObjective;

        emit sendMessage(msg1);
    }

    createMyShape();

    GameState["ObjVersion"] = GameState["ObjVersion"].toInt() + 1;
    GameState["Objective"] = myObjective;
    GameState["Status"] = "PLAY";

    QJsonObject msg1;
    msg1["MsgId"] = m_uuid.createUuid().toString();
    msg1["MsgPayload"] = GameState;

    emit sendMessage(msg1);

    m_timer->start(20);
}

void GameManager::scoreUpdate()
{
    scoreUp = true;
}


void GameManager::createClientUser(QString username)
{
    QJsonObject userMsg;
    QJsonObject UserPayload;

    myUserUuid = m_uuid.createUuid().toString();

    m_users[myUserUuid] = username;

    UserPayload["ObjOwnerUuid"] = m_users.key("root");
    UserPayload["ObjType"] = QString("UserPayload");
    UserPayload["ObjUuid"] = myUserUuid;
    UserPayload["ObjVersion"] = 1;
    UserPayload["UserName"] = username;
    UserPayload["UserAuthorizationLevel"] = ShapeGlobals::DefaultUserPermissions();

    userMsg["MsgId"] = m_uuid.createUuid().toString();
    userMsg["MsgPayload"] = UserPayload;

    emit sendMessage(userMsg);
    emit userSent();
}
