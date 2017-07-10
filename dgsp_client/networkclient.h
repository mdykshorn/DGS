#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QWidget>
#include <QIODevice>
#include <QDataStream>

class QComboBox;
class QLineEdit;
class QNetworkSession;

class NetworkClient : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkClient(QWidget *parent = Q_NULLPTR);

    ~NetworkClient();

signals:

    void enableConnect();

    void sendInfo(QString host, quint16 port, QString username);

private slots:

        void enableConnectButton();
        void sessionOpened();


private:
    QComboBox *hostCombo;
    QLineEdit *portLineEdit;
    QLineEdit *usernameEdit;


    QNetworkSession *networkSession;
};

#endif
