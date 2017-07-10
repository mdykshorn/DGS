#include <QtWidgets>
#include "gamecontrol.h"

GameControl::GameControl(QWidget *parent) : QWidget(parent)
  , connectButton(new QPushButton(tr("Connect")))
  , disconnectButton(new QPushButton(tr("Disconnect")))
  , quitButton(new QPushButton(tr("Quit")))
  , instructionsButton(new QPushButton(tr("View Instructions")))
  , startButton(new QPushButton(tr("Start Game")))
  , joinButton(new QPushButton(tr("Join Game")))
  , Instructions(new QMessageBox)
{


     QGridLayout *layout = new QGridLayout(this);
     layout->addWidget(connectButton, 0, 0, Qt::AlignRight);
     layout->addWidget(disconnectButton, 1, 0, Qt::AlignRight);
     layout->addWidget(quitButton, 2, 0, Qt::AlignRight);
     layout->addWidget(instructionsButton, 2, 1, Qt::AlignRight);
     layout->addWidget(startButton, 0, 1, Qt::AlignRight);
     layout->addWidget(joinButton, 1, 1, Qt::AlignRight);

     setLayout(layout);

     connectButton->setEnabled(false);
     disconnectButton->setEnabled(false);
     startButton->setEnabled(false);
     joinButton->setEnabled(false);

     connect(connectButton, &QPushButton::clicked, this, &GameControl::connectToServer);
     connect(disconnectButton, &QPushButton::clicked, this, &GameControl::disconnectFromServer);
     connect(quitButton, &QPushButton::clicked, this, &GameControl::quitClient);
     connect(instructionsButton, &QPushButton::clicked, this, &GameControl::showInstructions);
     connect(startButton, &QPushButton::clicked, this, &GameControl::startGame);
     connect(joinButton, &QPushButton::clicked, this, &GameControl::joinGame);


}

void GameControl::enableConnect(bool enabled)
{
    connectButton->setEnabled(enabled);
}

void GameControl::enableStart(bool enabled)
{
    startButton->setEnabled(enabled);
}

void GameControl::enableJoin(bool enabled)
{
    joinButton->setEnabled(enabled);
}

void GameControl::enableDisconnect(bool enabled)
{
    disconnectButton->setEnabled(enabled);
}

void GameControl::showInstructions()
{
    Instructions->setText("Instructions for Stay Alive");
    Instructions->setInformativeText("goal is to have multiple destroyer rectangles that move around randomly\n"
                                     "\n each user starts with 3 lives and if they get hit with a destroyer they lose a life and get reset\n"
                                     "\n rectangles will appear at random places throughout the game and if a user intersects with one of these they will gain a life\n"
                                     "\n Last user standing wins, or for single player, the ammount of time you last in the game is how you score yourself");
    Instructions->setStandardButtons(QMessageBox::Ok);

    Instructions->exec();
}
