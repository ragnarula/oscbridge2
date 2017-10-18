#ifndef QTSERVERPRESENTER_H
#define QTSERVERPRESENTER_H

#include <QObject>
#include "serverinteractor.h"

class MainWindow;

class QTServerPresenter : public QObject, public ServerPresenter
{
    Q_OBJECT
public:
    QTServerPresenter(MainWindow& MainWindow);
    void PresentServerState(std::string ServerState) override;

signals:
    void UpdateServerStatus(const QString& ServerStatus);
};

#endif // QTSERVERPRESENTER_H
