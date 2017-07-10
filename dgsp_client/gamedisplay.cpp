#include "gamedisplay.h"

GameDisplay::GameDisplay(QWidget *parent) : QWidget(parent)
{
    m_scene = new QGraphicsScene(this);

    QGraphicsView *view = new QGraphicsView(m_scene, this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(view);

    setLayout(layout);
}

void GameDisplay::updateField(QList<QJsonObject>repository)
{
    m_scene->clear();
    for(int i = 0; i<repository.size(); i++)
    {
        if(repository.at(i)["ObjType"].toString() == "ShapePayload")
        {
            QJsonObject shapeBB = repository.at(i)["ShapeBoundingBox"].toObject();
            QRectF shape(QPointF(shapeBB["UpperLeftX"].toDouble(), shapeBB["UpperLeftY"].toDouble()),
                                              QPointF(shapeBB["LowerRightX"].toDouble(), shapeBB["LowerRightY"].toDouble()));

            QJsonObject tempData = repository.at(i)["ClientData"].toObject();
            switch(tempData["UserNumber"].toInt())
            {
            case 1:
                m_scene->addRect(shape, QPen(), QBrush(Qt::red));
                break;
            case 2:
                m_scene->addRect(shape, QPen(), QBrush(Qt::yellow));
                break;
            case 3:
                m_scene->addRect(shape, QPen(), QBrush(Qt::blue));
                break;
            case 4:
                m_scene->addRect(shape, QPen(), QBrush(Qt::magenta));
                break;
            default:
                m_scene->addRect(shape, QPen(), QBrush(Qt::black));
                break;
            }

        }
        else if(repository.at(i)["ObjType"].toString() == "FieldPayload")
        {
            QJsonObject fieldBB = repository.at(i)["FieldBoundary"].toObject();
            QRectF field(QPointF(fieldBB["UpperLeftX"].toDouble(), fieldBB["UpperLeftY"].toDouble()),
                                              QPointF(fieldBB["LowerRightX"].toDouble(), fieldBB["LowerRightY"].toDouble()));
            m_scene->addRect(field);
        }
    }
}
