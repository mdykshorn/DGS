#ifndef GAMESTATS_H
#define GAMESTATS_H

#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>

#include "gamekey.h"


class GameStats : public QWidget
{
    Q_OBJECT
public:
    explicit GameStats(QWidget *parent = 0);

signals:

    void updateKey(QJsonArray);

public slots:

    void connectionStatus(bool connected);

    void showError(QString error);

    void updateUsers(QJsonArray users);

private:
    QLabel *statusLabel;
    QLabel *errorMessage;

    GameKey *keyBox;
};

#endif // GAMESTATS_H
