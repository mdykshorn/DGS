#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QWidget>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QJsonObject>

class GameDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit GameDisplay(QWidget *parent = 0);

signals:

public slots:

    void updateField(QList<QJsonObject> repository);

private:

    QGraphicsScene *m_scene;

};

#endif // GAMEDISPLAY_H
