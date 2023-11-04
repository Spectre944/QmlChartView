#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QLineSeries>
#include <QFile>
#include <QTextStream>
#include <QPointF>

class FileManager : public QObject
{

public:
    explicit FileManager(QObject *parent = nullptr);

    QList<QPointF> loadSpectrum(const QString &);

private:


signals:


public slots:


};

inline FileManager::FileManager(QObject *parent){
    Q_UNUSED(parent);
}

inline QList<QPointF> FileManager::loadSpectrum(const QString &filePath)
{
    QString line;
    QList<QPointF> spectrumData;

    int iter = 0;

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            line = in.readLine();
            spectrumData.append(QPointF(iter++, line.toInt()));
        }
        file.close();
    }

    return spectrumData;
}



#endif // FILEMANAGER_H
