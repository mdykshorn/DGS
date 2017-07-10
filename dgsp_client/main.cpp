#include <QApplication>
#include "guiclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(GuiClient::tr("Gui Client"));
    GuiClient gui;
    gui.show();
    return app.exec();
}
