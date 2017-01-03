#pragma once

#include <QSortFilterProxyModel>
#include <QRegularExpression>

class QsTreeViewProxy : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	explicit QsTreeViewProxy(QObject * parent = 0);
	virtual ~QsTreeViewProxy();

	// QAbstractProxyModel
	virtual void setSourceModel(QAbstractItemModel * sourceModel);

	// QSortFilterProxyModel
	virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

public slots:
	void setSectionFilter(int index, QString filter);

private slots:
	void onColumnsAboutToBeInserted(const QModelIndex & parent, int first, int last);
	void onColumnsAboutToBeMoved(const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationColumn);
	void onColumnsAboutToBeRemoved(const QModelIndex & parent, int first, int last);
	void onColumnsInserted(const QModelIndex & parent, int first, int last);
	void onColumnsMoved(const QModelIndex & parent, int start, int end, const QModelIndex & destination, int column);
	void onColumnsRemoved(const QModelIndex & parent, int first, int last);
	void onDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int>());
	void onHeaderDataChanged(Qt::Orientation orientation, int first, int last);
	void onModelAboutToBeReset();
	void onModelReset();
	void onRowsAboutToBeInserted(const QModelIndex & parent, int start, int end);
	void onRowsAboutToBeMoved(const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationRow);
	void onRowsAboutToBeRemoved(const QModelIndex & parent, int first, int last);
	void onRowsInserted(const QModelIndex & parent, int first, int last);
	void onRowsMoved(const QModelIndex & parent, int start, int end, const QModelIndex & destination, int row);
	void onRowsRemoved(const QModelIndex & parent, int first, int last);

private:
	QList<QRegularExpression> _filtersRegExp;
};
