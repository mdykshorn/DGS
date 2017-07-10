#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QGridLayout>


class GameControl : public QWidget
{
    Q_OBJECT
public:
    explicit GameControl(QWidget *parent = 0);

signals:

    void connectToServer();
    void disconnectFromServer();
    void quitClient();

    void startGame();
    void joinGame();


public slots:

    void enableConnect(bool enabled);
    void enableStart(bool enabled);
    void enableJoin(bool enabled);
    void enableDisconnect(bool enabled);

    void showInstructions();

private:

    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QPushButton *quitButton;
    QPushButton *instructionsButton;
    QPushButton *startButton;
    QPushButton *joinButton;


    QMessageBox *Instructions;
};

#endif // GAMECONTROL_H
