#ifndef SHAPECONTROL_H
#define SHAPECONTROL_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QKeySequence>

class ShapeControl : public QWidget
{
    Q_OBJECT
public:
    explicit ShapeControl(QWidget *parent = 0);

signals:

    //emitted to move the shape
    void moveShape(int x, int y);

public slots:

    void enableButtons(bool enabled);

    void handleClicked();

    void handleReleased();

private:

    QPushButton *left;
    QPushButton *right;
    QPushButton *up;
    QPushButton *down;
};

#endif // SHAPECONTROL_H
