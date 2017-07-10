#ifndef GAMEKEY_H
#define GAMEKEY_H

#include <QWidget>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class GameKey : public QWidget
{
    Q_OBJECT
public:
    explicit GameKey(QWidget *parent = 0);

signals:

public slots:

    void updateKey(QJsonArray users);

private:

    QGraphicsScene *m_scene;
};

#endif // GAMEKEY_H
