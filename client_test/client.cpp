#include <QtWidgets>
#include <QtNetwork>

#include "client.h"

#include <QDebug>
Client::Client(QWidget *parent)
    : QDialog(parent)
    , hostCombo(new QComboBox)
    , portLineEdit(new QLineEdit)
    , connectButton(new QPushButton(tr("Get Connected")))
    , tcpSocket(new QTcpSocket(this))
    , networkSession(Q_NULLPTR)
    ,sendButton(new QPushButton(tr("Send Message")))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    hostCombo->setEditable(true);
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        hostCombo->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    QLabel *hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(hostCombo);
    QLabel *portLabel = new QLabel(tr("S&erver port:"));
    portLabel->setBuddy(portLineEdit);

    statusLabel = new QLabel(tr("Not connected to server"));

    connectButton->setDefault(true);
    connectButton->setEnabled(false);

    sendButton->setDefault(true);
    sendButton->setEnabled(true);

    QPushButton *quitButton = new QPushButton(tr("Quit"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    QDialogButtonBox *buttonBox1 = new QDialogButtonBox;
    buttonBox1->addButton(connectButton, QDialogButtonBox::ActionRole);
    buttonBox1->addButton(sendButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    sendTextBox = new QTextEdit(this);
    recieveTextBox = new QTextEdit(this);

    connect(hostCombo, &QComboBox::editTextChanged,
            this, &Client::enableConnectButton);
    connect(portLineEdit, &QLineEdit::textChanged,
            this, &Client::enableConnectButton);
    connect(connectButton, &QAbstractButton::clicked,
            this, &Client::connectToServer);
    connect(sendButton, &QAbstractButton::clicked,
            this, &Client::sendMessage);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);



    QGridLayout *mainLayout = Q_NULLPTR;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        QVBoxLayout *outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        QHBoxLayout *outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        QGroupBox *groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QGridLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QGridLayout(this);
    }
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(statusLabel, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 1, Qt::AlignRight);
    mainLayout->addWidget(buttonBox1, 3, 0, Qt::AlignLeft);
    mainLayout->addWidget(sendTextBox, 4,0);
    mainLayout->addWidget(recieveTextBox, 4,1);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    portLineEdit->setFocus();

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

        connectButton->setEnabled(false);
        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::readRecieved);
}

Client::~Client()
{

}

void Client::connectToServer()
{
    connectButton->setEnabled(false);


    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(),
                             portLineEdit->text().toInt());

    if(tcpSocket->waitForConnected())
    {
        statusLabel->setText("Connected To Server");
    }

}

void Client::serverDisconnect()
{
    statusLabel->setText("Not connected to Server");
}



void Client::enableConnectButton()
{
    connectButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !hostCombo->currentText().isEmpty() &&
                                 !portLineEdit->text().isEmpty());

}

void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();


    enableConnectButton();
}

void Client::sendMessage()
{
    QString tempString = sendTextBox->toPlainText();
    QJsonDocument tempDoc = QJsonDocument::fromJson(tempString.toUtf8());
    QByteArray tempArray = tempDoc.toJson();

    tcpSocket->write(tempArray);

}

void Client::readRecieved()
{
    tcpSocket->startTransaction();
    QByteArray tempBytes = tcpSocket->readAll();

    QString recString(tempBytes);

    recieveTextBox->insertPlainText(recString);

    tcpSocket->commitTransaction();
}
