#include <extrarowproxymodel.h>

#include <KDebug>

ExtraRowProxyModel::ExtraRowProxyModel(QObject* parent)
: QAbstractListModel(parent)
, m_model(0)
{}

int ExtraRowProxyModel::appendRow()
{
    Row row(rowCount());
    m_rows.append(row);
    return m_rows.count() - 1;
}

void ExtraRowProxyModel::setRowData(int id, int column, const QVariant& value, int role)
{
    Q_ASSERT(id < m_rows.count());
    m_rows[id][column][role] = value;
}

void ExtraRowProxyModel::setRowText(int id, int column, const QVariant& value)
{
    setRowData(id, column, value, Qt::DisplayRole);
}

void ExtraRowProxyModel::setSourceModel(QAbstractItemModel* model)
{
    m_model = model;
    reset();
    setRoleNames(m_model->roleNames());
}

int ExtraRowProxyModel::rowCount(const QModelIndex &) const
{
    return m_model->rowCount() + m_rows.count();
}

QVariant ExtraRowProxyModel::data(const QModelIndex &index, int role) const
{
    int count = m_model->rowCount();
    if (index.row() < count) {
        return m_model->data(index, role);
    }

    int extraRow = index.row() - count;
    if (extraRow >= m_rows.count()) {
        return QVariant();
    }

    const Item& item = m_rows[extraRow].at(index.column());
    return item.value(role);
}
