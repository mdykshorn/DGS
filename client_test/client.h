#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QTextEdit>

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = Q_NULLPTR);

    ~Client();

private slots:
        void connectToServer();
        void serverDisconnect();
        void enableConnectButton();
        void sessionOpened();
        void sendMessage();
        void readRecieved();

private:
    QComboBox *hostCombo;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QPushButton *connectButton;
    QPushButton *sendButton;

    QTextEdit *sendTextBox;
    QTextEdit *recieveTextBox;

    QTcpSocket *tcpSocket;
    QByteArray *m_block;

    QNetworkSession *networkSession;
};

#endif
