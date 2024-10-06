#include "worker.h"



qint64 Worker::dirSize(const QString& dirPath) {
    qint64 size = 0;
    QDir dir(dirPath);
    //calculate total size of current directories' files
    for(const auto& filePath : dir.entryList(QDir::Files|QDir::System|QDir::Hidden)) {
        size += QFileInfo(dir, filePath).size();
    }
    //add size of child directories recursively
    for(const auto& childDirPath : dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::System|QDir::Hidden| QDir::NoSymLinks)) {
        size += dirSize(dirPath + QDir::separator() + childDirPath);
    }
    return size;
}

void Worker::run(){
    // Вычисление в отдельном потоке
    qint64 sizeDirQint64 = dirSize(directory);
    QString dirSizeQString = convert(sizeDirQint64);
    emit addDirSizeSignals(directory, dirSizeQString);
}

QString Worker::convert(qint64 size){
    // Перевод из байтов
    qint16 i = 0;
    double covertSize = size;
    QList<QString> NAMESIZE {"байты","KB", "MB", "GB", "TB"};
    while (covertSize > 1024){
        i++;
        covertSize /= 1024.0;
    }
    covertSize = round(covertSize);
    QString convertSize = QString::number(covertSize) + " " + NAMESIZE[i];

    return convertSize;
}

