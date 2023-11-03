#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QLineSeries>
#include <QFile>
#include <QTextStream>
#include <QPointF>

class FileManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<int> spectrumLocal READ spectrumLocal WRITE setSpectrum NOTIFY spectrumChanged)

public:
    explicit FileManager(QObject *parent = nullptr);

    Q_INVOKABLE QLineSeries* loadCSVFile(const QString &filePath);

    QVector<int> spectrumLocal() const { return spectrumData; }

private:
    QVector<int> spectrumData;

signals:

    //slow method
    void getSpectrumToQML(QVector<int>);

    void spectrumChanged();

public slots:

    void setSpectrum(const QVector<int> &spectrum) {
        if (spectrumData != spectrum) {
            spectrumData = spectrum;
            emit spectrumChanged();
        }
    }

};

inline FileManager::FileManager(QObject *parent){
    Q_UNUSED(parent);
}

inline QLineSeries *FileManager::loadCSVFile(const QString &filePath)
{
    QLineSeries* series = new QLineSeries;
    QString line;
    spectrumData.clear();
    QVector<int> spectrumVec;
    int iter = 0;

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            line = in.readLine();
            iter++;
            series->append(QPointF(iter, line.toInt()));
            spectrumVec.append(line.toInt());
        }
        file.close();
    }
    //emit getSpectrumToQML(spectrumVec);

    spectrumData = spectrumVec;  // Set spectrumData using direct assignment
    emit spectrumChanged(); // Emit the signal to notify QML about the change

    return series;
}



#endif // FILEMANAGER_H
