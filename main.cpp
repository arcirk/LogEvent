#include "mainwindow.h"
#include <QStyleFactory>
#include <QApplication>

#include "Database/ConnectionManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon appIcon(":/img/log_viewer.ico");
    qApp->setStyle(QStyleFactory::create("Fusion"));

    Database::ConnectionManager *mgr = Database::ConnectionManager::createInstance();
    mgr->setType("QSQLITE");

    MainWindow w(mgr);
    w.show();

    int ret = a.exec();

    Database::ConnectionManager::destroyInstance();

    return ret;
}
