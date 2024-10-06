#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include "QDir"
#include <cmath>

class Worker: public QObject
{
    QString directory;
    Q_OBJECT
    public:
    Worker(QString directory) : directory(directory) {}
    qint64 dirSize(const QString& dirPath);
    QString convert(qint64 size);
    public slots:
        void run();
    signals:
        void addDirSizeSignals(QString dirPath, QString dirSize);

};

#endif // WORKER_H
