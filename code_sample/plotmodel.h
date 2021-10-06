#ifndef PLOTMODEL_H
#define PLOTMODEL_H

#include <QAbstractTableModel>
#include "plotsEntity.h"

class PlotModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PlotModel(QexGraphicsEntity* parentEntity, quint8 tail, QObject *parent = 0);
    virtual ~PlotModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    PlotShape *addPlot(QJsonObject& json, float x_, float y_);
    void addNorth(QJsonObject& json);

    void setPlotColor(const QColor &color);
    void setRadarShortName(QString& shortName);
    void setFontSize(int fs);

    PlotsEntity*& last()
    {
        return m_vplots.last();
    }

    void removePlotsEntity();
    int removeRLDataMulty_0(int count);

    void deleteAll();
    void resizeTail(quint8 sz);

    enum columnType
    {
        SSRCODE,
        HEIGHT,
        PLOTTYPE,
        INTIME,
    };

    int                  m_plotSize;

public slots:
    void onClicked(const QModelIndex &);
    void onDoubleClicked(const QModelIndex &index);

private:
    // Длина вектора m_vplots - есть длина хвоста отметок "плот", заданная пользователем.
    // В каждом PlotsEntity все отметки одного радара за один оборот
    // и только один север. Север замыкает список отметок за оборот
    // иначе, ищи семантическую ошибку
    // Добавление отметки происходит всегда в последний элемент PlotsEntity
    // При прохождении через север, содержимое первого элемента PlotsEntity очищается
    // и все элементы вектора m_vplots смещаются циклически на одну позицию вперед
    // а последним элементом вектора становится первый.
    QVector<PlotsEntity*>   m_vplots;
    QexGraphicsEntity*      m_parentEntity;

};


#endif
