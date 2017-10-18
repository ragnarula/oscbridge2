#include "qtserverpresenter.h"
#include "mainwindow.h"

QTServerPresenter::QTServerPresenter(MainWindow& MainWindow)
{
    QObject::connect(this, SIGNAL(UpdateServerStatus(const QString&)),
                     &MainWindow, SLOT(UpdateServerStatus(const QString&)), Qt::QueuedConnection);
}

void QTServerPresenter::PresentServerState(std::string ServerState)
{
    emit UpdateServerStatus(QString::fromStdString(ServerState));
}
