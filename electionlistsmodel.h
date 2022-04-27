#ifndef ELECTIONLISTSMODEL_H
#define ELECTIONLISTSMODEL_H

//#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QObject>

class ElectionListsModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ElectionListsModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index,
                      const QVariant & value, int role);

};

#endif // ELECTIONLISTSMODEL_H
