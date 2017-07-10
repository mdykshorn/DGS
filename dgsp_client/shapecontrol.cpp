#include "shapecontrol.h"

ShapeControl::ShapeControl(QWidget *parent) : QWidget(parent)
  , left(new QPushButton(tr("LEFT(a)")))
  , right(new QPushButton(tr("RIGHT(d)")))
  , up(new QPushButton(tr("UP(w)")))
  , down(new QPushButton(tr("DOWN(s)")))
{
    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(left,1,0);
    layout->addWidget(right,1,2);
    layout->addWidget(up,0,1);
    layout->addWidget(down,1,1);

    setLayout(layout);

    //set keyboard shortcuts
    left->setShortcut(QKeySequence(Qt::Key_A));
    right->setShortcut(QKeySequence(Qt::Key_D));
    up->setShortcut(QKeySequence(Qt::Key_W));
    down->setShortcut(QKeySequence(Qt::Key_S));

    connect(left, &QPushButton::pressed, this, &ShapeControl::handleClicked);
    connect(right, &QPushButton::pressed, this, &ShapeControl::handleClicked);
    connect(up, &QPushButton::pressed, this, &ShapeControl::handleClicked);
    connect(down, &QPushButton::pressed, this, &ShapeControl::handleClicked);

    connect(left, &QPushButton::released, this, &ShapeControl::handleReleased);
    connect(right, &QPushButton::released, this, &ShapeControl::handleReleased);
    connect(up, &QPushButton::released, this, &ShapeControl::handleReleased);
    connect(down, &QPushButton::released, this, &ShapeControl::handleReleased);

    left->setEnabled(false);
    right->setEnabled(false);
    up->setEnabled(false);
    down->setEnabled(false);
}

void ShapeControl::enableButtons(bool enabled)
{
    if(enabled)
    {
        left->setEnabled(enabled);
        right->setEnabled(enabled);
        up->setEnabled(enabled);
        down->setEnabled(enabled);
    }
}

void ShapeControl::handleClicked()
{
    int x = 0;
    int y = 0;
    if(left->isDown())
    {
        x = -1;
    }
    else if(right->isDown())
    {
        x = 1;
    }
    if(up->isDown())
    {
        y = -1;
    }
    else if(down->isDown())
    {
        y = 1;
    }
    emit moveShape(x,y);
}

void ShapeControl::handleReleased()
{
    int x = 0;
    int y = 0;
    if(left->isDown())
    {
        x = -1;
    }
    else if(right->isDown())
    {
        x = 1;
    }
    if(up->isDown())
    {
        y = -1;
    }
    else if(down->isDown())
    {
        y = 1;
    }
    emit moveShape(x,y);
}
