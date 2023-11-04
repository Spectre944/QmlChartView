#ifndef SPECTRUMSOURCE_H
#define SPECTRUMSOURCE_H

#include <QAreaSeries>
#include <QQuickItem>
#include <QQuickView>
#include <QRandomGenerator>
#include <QtMath>
#include <QXYSeries>
#include <QValueAxis>

#include "filemanager.h"

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

class SpectrumSource : public QObject
{
    Q_OBJECT
public:

    explicit SpectrumSource(QObject *parent = nullptr);


public slots:
    void generateData(int type, int rowCount, int colCount);
    void update(QAbstractSeries *series);

    void updateFromFile(QAbstractSeries *series, const QString &path);

private:
    FileManager fm;


    QQuickView *m_appViewer = nullptr;
    QList<QList<QPointF>> m_data;
    int m_index = -1;
};


inline SpectrumSource::SpectrumSource(QObject *parent)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    //generateData(0, 1, 1024);
}

inline void SpectrumSource::generateData(int type, int rowCount, int colCount)
{
    // Remove previous data
    m_data.clear();

    // Append the new data depending on the type
    for (int i(0); i < rowCount; i++) {
        QList<QPointF> points;
        points.reserve(colCount);
        for (int j(0); j < colCount; j++) {
            qreal x(0);
            qreal y(0);
            switch (type) {
            case 0:
                // data with sin + random component
                y = qSin(M_PI / 50 * j) + 0.5 + QRandomGenerator::global()->generateDouble();
                x = j;
                break;
            case 1:
                // linear data
                x = j;
                y = (qreal) i / 10;
                break;
            default:
                // unknown, do nothing
                break;
            }
            points.append(QPointF(x, y));
        }
        m_data.append(points);
    }
}

inline void SpectrumSource::update(QAbstractSeries *series)
{
    if (series) {
        auto xySeries = static_cast<QXYSeries *>(series);
        m_index++;
        if (m_index > m_data.count() - 1)
            m_index = 0;

        QList<QPointF> points = m_data.at(m_index);
        // Use replace instead of clear + append, it's optimized for performance
        xySeries->replace(points);
    }
}

inline void SpectrumSource::updateFromFile(QAbstractSeries *series, const QString &path)
{
    if (series) {
        auto xySeries = static_cast<QXYSeries *>(series);

        QList<QPointF> points = fm.loadSpectrum(path);
        xySeries->replace(points);

        // adjust axis
        auto axes = xySeries->attachedAxes();

        for (QAbstractAxis* axis : axes) {
            if (qobject_cast<QValueAxis*>(axis)) {
                QValueAxis* valueAxis = qobject_cast<QValueAxis*>(axis);

                if (valueAxis) {
                    if (valueAxis->orientation() == Qt::Horizontal) {
                        valueAxis->setMin(0);
                        valueAxis->setMax(points.size() - 1);
                    } else if (valueAxis->orientation() == Qt::Vertical) {
                        double maxSpectrumValue = 0;
                        for (const auto& point : points) {
                            if (point.y() > maxSpectrumValue) {
                                maxSpectrumValue = point.y();
                            }
                        }

                        valueAxis->setMin(0);
                        valueAxis->setMax(maxSpectrumValue * 1.1); // Увеличьте максимальное значение оси Y на 10% (может потребоваться настройка)
                    }
                }
            }
        }
    }
}



#endif // SPECTRUMSOURCE_H
