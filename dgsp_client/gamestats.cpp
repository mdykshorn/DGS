#include <QtWidgets>
#include "gamestats.h"

GameStats::GameStats(QWidget *parent) : QWidget(parent)
{
    statusLabel = new QLabel(tr("Not connected to server"));
    errorMessage = new QLabel(tr(""));

    keyBox = new GameKey(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(statusLabel, 1, 0);
    layout->addWidget(errorMessage, 0, 0);
    layout->addWidget(keyBox, 2, 0);

    setLayout(layout);

    connect(this, &GameStats::updateKey, keyBox, &GameKey::updateKey);
}

void GameStats::connectionStatus(bool connected)
{
    if(connected)
        statusLabel->setText("Connected to Server");
    else
        statusLabel->setText("Not connected to Server");
}


void GameStats::showError(QString error)
{
    errorMessage->setText(error);
}

void GameStats::updateUsers(QJsonArray users)
{
    emit updateKey(users);
}
