#include "gamekey.h"

GameKey::GameKey(QWidget *parent) : QWidget(parent)
{
    m_scene = new QGraphicsScene(0,0, 50, 250, this);

    QGraphicsView *view = new QGraphicsView(m_scene, this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(view);

    setLayout(layout);


}

void GameKey::updateKey(QJsonArray users)
{
    m_scene->clear();
    QGraphicsItem *tempItem;
    tempItem = m_scene->addText("Game Object Key");
    tempItem = m_scene->addText("Object Color   Username        Score");
    tempItem->moveBy(0,15);
    m_scene->addRect(20,35,20,20, QPen(), QBrush(Qt::black));
    tempItem = m_scene->addText("Target Object");
    tempItem->moveBy(50, 35);

    int row = 65;

    for(int i = 0; i<users.size(); i++)
    {
        QJsonObject tempUser = users.at(i).toObject();
        QGraphicsItem *tempItem;

        switch(tempUser["UserNumber"].toInt())
        {
        case 1:
            m_scene->addRect(20,row,20,20, QPen(), QBrush(Qt::red));
            tempItem = m_scene->addText(tempUser["UserName"].toString());
            tempItem->moveBy(50, row);
            tempItem = m_scene->addText(QString::number(tempUser["Score"].toInt()));
            tempItem->moveBy(150, row);
            break;
        case 2:
            m_scene->addRect(20,row,20,20, QPen(), QBrush(Qt::yellow));
            tempItem = m_scene->addText(tempUser["UserName"].toString());
            tempItem->moveBy(50, row);
            tempItem = m_scene->addText(QString::number(tempUser["Score"].toInt()));
            tempItem->moveBy(150, row);
            break;
        case 3:
            m_scene->addRect(20,row,20,20, QPen(), QBrush(Qt::blue));
            tempItem = m_scene->addText(tempUser["UserName"].toString());
            tempItem->moveBy(50, row);
            tempItem = m_scene->addText(QString::number(tempUser["Score"].toInt()));
            tempItem->moveBy(150, row);
            break;
        case 4:
            m_scene->addRect(20,row,20,20, QPen(), QBrush(Qt::magenta));
            tempItem = m_scene->addText(tempUser["UserName"].toString());
            tempItem->moveBy(50, row);
            tempItem = m_scene->addText(QString::number(tempUser["Score"].toInt()));
            tempItem->moveBy(150, row);
            break;
        }
        row = row +25;
    }
}
