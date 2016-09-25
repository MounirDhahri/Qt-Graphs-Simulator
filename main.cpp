#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "assistant.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    Assistant wizard;
    wizard.show();
    return app.exec();
}
