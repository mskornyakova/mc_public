#include "plotmodel.h"


PlotModel::PlotModel(QexGraphicsEntity *parentEntity, quint8 tail, QObject *parent)
    : QAbstractTableModel(parent)
    , m_plotSize(0)
    , m_parentEntity(parentEntity)
{
//    qDebug("new plot model, tail = %i", tail);
    tail = tail==0 ? 1 : tail;
    m_vplots.resize(tail);

    for(int i=0; i<m_vplots.count(); i++)
    {
        m_vplots[i]=new PlotsEntity(m_parentEntity);
    }
}

// -------------------------------------------------------------------
PlotModel::~PlotModel()
{
  //  qDebug("%s:", __func__);
    if (rowCount()-1<0)
        return;

    beginRemoveRows(QModelIndex(),0, this->rowCount()-1); // ASSERT: "last >= first" in file itemmodels\qabstractitemmodel.cpp, line 2786
    qDeleteAll(m_vplots);
    m_vplots.clear();
    endRemoveRows();
}

// -------------------------------------------------------------------
int PlotModel::rowCount(const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
   // return m_plots.count();

    int len=0;
    foreach (PlotsEntity* list, m_vplots)
    {
        len+=list->count();
    }
    return len;
}

// -------------------------------------------------------------------
int PlotModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return columnType::INTIME+1;
}

// -------------------------------------------------------------------
QVariant PlotModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::FontRole==role)
    {
        return QFont(QString::fromUtf8("Lato Regular"),8);
    }

    if (role==Qt::TextAlignmentRole){
        // все столбцы кроме последнего центруются
        if (orientation == Qt::Vertical || section!=INTIME)
            return Qt::AlignCenter;
        // последний столбец выравнивается по левого краю
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        //return QString(QString::fromUtf8("201%1")).arg(section);
    {
        switch (section )
        {
        case SSRCODE:
            return QString::fromUtf8("ssrcode");
        case HEIGHT:
            return QString::fromUtf8("высота");
        case PLOTTYPE:
            return QString::fromUtf8("тип");
        case INTIME:
            return QString::fromUtf8("время");
        default:
            return QString::fromUtf8(":)");
        }
    }
    else

    if (orientation == Qt::Vertical)
    {
        return QString(QString::fromUtf8("%1")).arg(section + 1);
    }

    return QVariant();
}

// -------------------------------------------------------------------
QVariant PlotModel::data(const QModelIndex &index, int role) const
{
    //qDebug("data index.isValid=%i",index.isValid());
    if ( !index.isValid() )
        return QVariant();

    PlotShape* plot=static_cast<PlotShape*>(index.internalPointer());
    if (plot==nullptr)
        return QString::fromUtf8("error");

    if (Qt::DisplayRole==role && plot!=nullptr)
    {
      //  qDebug("role=%i  index(%i,%i)  pointer=%p, %s",role,index.row(),index.column(),plot, plot->ssrcode.toUtf8().data());
        switch (index.column()) {
        case SSRCODE:
            return plot->ssrcode.toUtf8();
        case PLOTTYPE:
            return plot->rlType;
        case HEIGHT:
            return plot->height;
        case INTIME:
            return plot->inTime;
        default:
            return QVariant();
        }
    }
    else if (Qt::ForegroundRole==role)
    {
        if (plot->ssrcode.contains(QString::fromUtf8("north")))
            return QColor(0,150,255); // север в синий цвет

        if (index.column()==SSRCODE) // ssrcode в цвет отметки
            return plot->m_shape.pen().color();
        // остальные столбцы в цвет палитры

        return qApp->palette().windowText();
    }
    else if (Qt::FontRole==role)
    {
        QFont font(QString::fromUtf8("Lato Regular"),8);
        return font;
    }
    else if (Qt::TextAlignmentRole==role)
    {
        if (index.column()==INTIME)
            return QVariant(Qt::AlignLeft|Qt::AlignVCenter);
        return Qt::AlignCenter;
    }
    else if (Qt::ToolTipRole==role)
    {
        QString str;
        switch (index.column())
        {
        case INTIME:
            return QString::fromUtf8("время пришествия");
        }
    }

    return QVariant();
}

// -------------------------------------------------------------------
//bool PlotModel::setData(const QModelIndex &index, const QVariant&, int role)
//{
//    Q_UNUSED(role);
// //   qDebug()<<"setData role="<<role;

//    if (!index.isValid())// || index.column()!=N )
//        return false;

//    return false;
//}

// -------------------------------------------------------------------
QModelIndex PlotModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex( row, column, parent ) )
        return QModelIndex();
    // parent should be invalid in this model

    int k=0;
    int i=row;
    while (m_vplots.count() > k && m_vplots.at(k)->count() <= i)
    {
        i-=m_vplots.at(k)->count();
        k++;
    }
    // k - искомый PlotsEntity, i - элемент в PlotsEntity

    if (m_vplots.count()>k && m_vplots.at(k)->count()>i)
    {
        return createIndex(row,column, static_cast<void*>(m_vplots.at(k)->plot(i)));
    }

    return QModelIndex();
}

// -------------------------------------------------------------------
Qt::ItemFlags PlotModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsEnabled;
}

// -------------------------------------------------------------------
void PlotModel::onClicked(const QModelIndex &)
{
   // qDebug("%s: %i, plot=%p, ssrcode=%s", __func__, index.row(), plot, plot->ssrcode().toUtf8().data());
   // QPen pen(QColor(255,255,255));
   // plot->setPen(pen);
}

// -------------------------------------------------------------------
void PlotModel::onDoubleClicked(const QModelIndex &)
{
 //   qDebug("%s: %i", __func__,index.row());
}

// -------------------------------------------------------------------
// перекрасить отметки plot в цвет color
void PlotModel::setPlotColor(const QColor& color)
{
  //  qDebug("PlotModel::%s: color(%i,%i,%i)",__func__,color.red(),color.green(),color.blue());
    foreach(PlotsEntity* pPlots, m_vplots)
    {
        pPlots->setColor(color);
    }
}

// -------------------------------------------------------------------
void PlotModel::setRadarShortName(QString& shortName)
{
    foreach (PlotsEntity* entity, m_vplots) {
        entity->setRadarShortName(shortName);
    }
}

// -------------------------------------------------------------------
void PlotModel::setFontSize(int fs)
{
    if (m_plotSize == fs)
        return;

    m_plotSize = fs;
    foreach (PlotsEntity* entity, m_vplots) {
       entity->setFontSize(fs);
    }
}


// -------------------------------------------------------------------
PlotShape* PlotModel::addPlot(QJsonObject& json, float x_, float y_)
{
  //  qDebug("PlotModel::%s plotSize=%i",__func__, m_plotSize);
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    endInsertRows();
    PlotsEntity*& plots=m_vplots.last();
    return plots->addPlot(json,m_plotSize, x_,y_);
}

// -------------------------------------------------------------------
void PlotModel::addNorth(QJsonObject& json)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    endInsertRows();
    PlotsEntity*& plots=m_vplots.last();
    plots->addNorth(json);
}

// -------------------------------------------------------------------
// очищает элемент i, указатели следующих за i элементов смещает
// циклически на одну позицию вперед
// элемент i становится последним
void PlotModel::removePlotsEntity()
{
    if (m_vplots.isEmpty())
        return;

  //  qDebug("%s: всего отметок плот у первого элемента PlotsEntity хвоста = %i", __func__, m_vplots[i]->count());
    // первый элемент PlotsEntity в векторе уже не пуст, это означает что хвост достиг максимальной длины
    if (m_vplots[0]->count()-1 >= 0)
    {
        // удалить из начала таблицы отметки плот и север
        beginRemoveRows(QModelIndex(), 0, m_vplots[0]->count()-1); // ASSERT: "last >= first" in file itemmodels\qabstractitemmodel.cpp, line 2786
        endRemoveRows();
    }

    // почистить содержимое элемента под номером НУЛЬ
    PlotsEntity* entity=m_vplots[0];
   // qDebug("%s:: plots count = %i", __func__, entity->count());
    entity->clear();

    // сместить указатели последующих элементов в векторе на один вперед
    int j=0;
    while(j < m_vplots.count()-1)
    {
        m_vplots[j]=m_vplots[j+1];
        j++;
    }
    m_vplots[j]=entity; // последний элемент в векторе пуст

}

// -------------------------------------------------------------------
// Очистить первые count PlotsEntity в массиве
int PlotModel::removeRLDataMulty_0(int count)
{
    qDebug("%s: count=%i", __func__, count);

    if (count > m_vplots.count())
        count=m_vplots.count();

    int k=0;
    int last_=0;
    while (k < count && k<m_vplots.count())
        last_+=m_vplots[k++]->count();

   // qDebug("PlotModel::removeRLDataMulty_0: last_-1=%i", last_-1);
    if (last_-1 >= 0) // иначе ASSERT: "last >= first" in file itemmodels\qabstractitemmodel.cpp, line 2786
    {
        beginRemoveRows(QModelIndex(), 0, last_-1);
        endRemoveRows();
    }

    int count_=count;
    int index_=0;
    while (count_>0)
    {
        PlotsEntity* entity=m_vplots[index_];
        entity->clear();
        count_--;
        index_++;
    }

    QVector<PlotsEntity*> v_;
    for (int i=0; i<count; i++)
        v_.push_back(m_vplots[i]);

    k=0;
    while (k+count < m_vplots.count())
    {
        m_vplots[k]=m_vplots[k+count];
        k++;
    }

    for(int j=0; j<v_.count();j++)
    {
        m_vplots[k]=v_[j];
        k++;
    }

    return count;
}

// -------------------------------------------------------------------
// Очищает элементы в массиве. Сами элементы при этом не удаляются
void PlotModel::deleteAll()
{
  //  qDebug("PlotModel::%s: m_vplots.count()=%i, rowCount=%i",__func__, m_vplots.count(), rowCount());
    if (rowCount()-1 < 0)
        return;

    beginRemoveRows(QModelIndex(),0, this->rowCount()-1); // ASSERT: "last >= first" in file itemmodels\qabstractitemmodel.cpp, line 2786
    for (int i = 0; i < m_vplots.count(); i++)
    {
        PlotsEntity* list=m_vplots[i];
        list->clear();
    }
    endRemoveRows();
}

// -------------------------------------------------------------------
// Изменение длины хвоста для отметок plot.
// Если новая длина больше предыдушей, то создаются новые элементы и их указатели добавляются в начало массива
// Если новая длина меньше предыдущей, то лишние элементы очищаются и удаляются
void PlotModel::resizeTail(quint8 sz)
{
    qDebug("\n%s: m_vlplot.count=%i, new size=%i", __func__, m_vplots.count(), sz);
//    for (int i=0; i< m_vplots.count(); i++){
//        qDebug(" m_vplots[%i]=%p  - %i",i, m_vplots[i], m_vplots[i]->count());}

    // увеличиваем на единицу количество объектов PlotsEntity,
    // поскольку в последний элемент добавляются отметки за текущий оборот и данный объект не является частью хвоста
    sz+=1;

    if (sz==m_vplots.count())
        return;

    if (sz<1){
        // TODO:
        return;
    }

    if (sz < m_vplots.count())
    {
        // уменьшение хвоста
        int rm=removeRLDataMulty_0(m_vplots.count()-sz);
        qDebug(" удалено PlotsEntity: %i", rm);
//        QVector<PlotsEntity*> _test; // TEST вектор удаленных объектов PlotsEntity*
        for(int i=sz; i<m_vplots.count(); i++)
        {
//            _test.push_back(m_vplots[i]); // TEST
            delete m_vplots[i];
            //m_vplots[i]=nullptr;
            rm--;
        }
        if (rm!=0)
            qDebug("PlotModel::%s: memory leak, rm=%i",__func__,rm);

        // TEST PRINT BEGIN - - - - - - - -
//      qDebug("      : m_vplots.count=%i, ------- ", m_vplots.count());
//      for (int i =0; i<m_vplots.count(); i++)
//      {
//          if (!_test.contains(m_vplots[i]))  //(m_vplots[i]!=nullptr) // объект PlotsEntity* не удален
//              qDebug("  m_vplots[%i]=%p - %i",i,m_vplots[i],m_vplots[i]->count());
//          else
//            qDebug("  m_vplots[%i]=%p - null",i, m_vplots[i]); // объект PlotsEntity* в списке удаленных
//      }
//      // TEST PRINT END - - - - - - - -

        m_vplots.resize(sz);

//        // TEST PRINT BEGIN - - - -
//        for (int i=0; i<_test.count(); i++)
//            qDebug(" v_[%i]=%p", i, _test[i]);
//        // TEST PRINT END - - - -
    }

    if (sz > m_vplots.count())
    {
       // qDebug("   grouth");
        // увеличение
        while (m_vplots.count()<sz)
            m_vplots.push_front(new PlotsEntity(m_parentEntity));
    }

//    qDebug("PlotModel::resizeTail: sz=%i count=%i",sz,m_vplots.count());

//    qDebug("      : m_vplots.count=%i, result", m_vplots.count());
//    for (int i=0; i< m_vplots.count(); i++){
//        qDebug(" m_vlplot[%i]=%p  - %i",i, m_vplots[i], m_vplots[i]->count());}
}
