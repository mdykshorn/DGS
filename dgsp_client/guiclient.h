#ifndef GUICLIENT_H
#define GUICLIENT_H

#include <QWidget>

#include "networkclient.h"
#include "gamecontrol.h"
#include "gamestats.h"
#include "gamedisplay.h"
#include "connectionstate.h"
#include "userstate.h"
#include "repocache.h"
#include "gamemanager.h"
#include "sendhandler.h"
#include "shapecontrol.h"

class GuiClient : public QWidget
{
    Q_OBJECT
public:
    explicit GuiClient(QWidget *parent = 0);

    ~GuiClient();

signals:

public slots:

private:
};

#endif // GUICLIENT_H
