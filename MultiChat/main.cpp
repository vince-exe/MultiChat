#include "main_dialog.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    qDebug() << "App path : " << qApp->applicationDirPath();

    MainDialog w;
    w.show();

    return a.exec();
}
