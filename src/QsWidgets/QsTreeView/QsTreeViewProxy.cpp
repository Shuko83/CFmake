#include "QsTreeViewProxy.h"

//---------------------------------------------------------------------------------
QsTreeViewProxy::QsTreeViewProxy(QObject * parent /*= 0*/)
	: QSortFilterProxyModel(parent)
{

}

//---------------------------------------------------------------------------------
QsTreeViewProxy::~QsTreeViewProxy()
{
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::setSourceModel(QAbstractItemModel * sourceModel)
{
	beginResetModel();

	if (this->sourceModel())
	{
		disconnect(this->sourceModel(), &QAbstractItemModel::columnsAboutToBeInserted, this, &QsTreeViewProxy::onColumnsAboutToBeInserted);
		disconnect(this->sourceModel(), &QAbstractItemModel::columnsAboutToBeMoved, this, &QsTreeViewProxy::onColumnsAboutToBeMoved);
		disconnect(this->sourceModel(), &QAbstractItemModel::columnsAboutToBeRemoved, this, &QsTreeViewProxy::onColumnsAboutToBeRemoved);
		disconnect(this->sourceModel(), &QAbstractItemModel::columnsInserted, this, &QsTreeViewProxy::onColumnsInserted);
		disconnect(this->sourceModel(), &QAbstractItemModel::columnsMoved, this, &QsTreeViewProxy::onColumnsMoved);
		disconnect(this->sourceModel(), &QAbstractItemModel::columnsRemoved, this, &QsTreeViewProxy::onColumnsRemoved);
		disconnect(this->sourceModel(), &QAbstractItemModel::dataChanged, this, &QsTreeViewProxy::onDataChanged);
		disconnect(this->sourceModel(), &QAbstractItemModel::headerDataChanged, this, &QsTreeViewProxy::onHeaderDataChanged);
		disconnect(this->sourceModel(), &QAbstractItemModel::modelAboutToBeReset, this, &QsTreeViewProxy::onModelAboutToBeReset);
		disconnect(this->sourceModel(), &QAbstractItemModel::modelReset, this, &QsTreeViewProxy::onModelReset);
		disconnect(this->sourceModel(), &QAbstractItemModel::rowsAboutToBeInserted, this, &QsTreeViewProxy::onRowsAboutToBeInserted);
		disconnect(this->sourceModel(), &QAbstractItemModel::rowsAboutToBeMoved, this, &QsTreeViewProxy::onRowsAboutToBeMoved);
		disconnect(this->sourceModel(), &QAbstractItemModel::rowsAboutToBeRemoved, this, &QsTreeViewProxy::onRowsAboutToBeRemoved);
		disconnect(this->sourceModel(), &QAbstractItemModel::rowsInserted, this, &QsTreeViewProxy::onRowsInserted);
		disconnect(this->sourceModel(), &QAbstractItemModel::rowsMoved, this, &QsTreeViewProxy::onRowsMoved);
		disconnect(this->sourceModel(), &QAbstractItemModel::rowsRemoved, this, &QsTreeViewProxy::onRowsRemoved);
	}

	QAbstractProxyModel::setSourceModel(sourceModel);

	_filtersRegExp.clear();

	if (this->sourceModel())
	{
		for (int i = 0; i < this->sourceModel()->columnCount(); i++)
		{
			_filtersRegExp.append(QRegularExpression());
			_filtersRegExp[i].setPatternOptions(QRegularExpression::CaseInsensitiveOption);
		}
	}

	if (this->sourceModel())
	{
		connect(this->sourceModel(), &QAbstractItemModel::columnsAboutToBeInserted, this, &QsTreeViewProxy::onColumnsAboutToBeInserted);
		connect(this->sourceModel(), &QAbstractItemModel::columnsAboutToBeMoved, this, &QsTreeViewProxy::onColumnsAboutToBeMoved);
		connect(this->sourceModel(), &QAbstractItemModel::columnsAboutToBeRemoved, this, &QsTreeViewProxy::onColumnsAboutToBeRemoved);
		connect(this->sourceModel(), &QAbstractItemModel::columnsInserted, this, &QsTreeViewProxy::onColumnsInserted);
		connect(this->sourceModel(), &QAbstractItemModel::columnsMoved, this, &QsTreeViewProxy::onColumnsMoved);
		connect(this->sourceModel(), &QAbstractItemModel::columnsRemoved, this, &QsTreeViewProxy::onColumnsRemoved);
		connect(this->sourceModel(), &QAbstractItemModel::dataChanged, this, &QsTreeViewProxy::onDataChanged);
		connect(this->sourceModel(), &QAbstractItemModel::headerDataChanged, this, &QsTreeViewProxy::onHeaderDataChanged);
		connect(this->sourceModel(), &QAbstractItemModel::modelAboutToBeReset, this, &QsTreeViewProxy::onModelAboutToBeReset);
		connect(this->sourceModel(), &QAbstractItemModel::modelReset, this, &QsTreeViewProxy::onModelReset);
		connect(this->sourceModel(), &QAbstractItemModel::rowsAboutToBeInserted, this, &QsTreeViewProxy::onRowsAboutToBeInserted);
		connect(this->sourceModel(), &QAbstractItemModel::rowsAboutToBeMoved, this, &QsTreeViewProxy::onRowsAboutToBeMoved);
		connect(this->sourceModel(), &QAbstractItemModel::rowsAboutToBeRemoved, this, &QsTreeViewProxy::onRowsAboutToBeRemoved);
		connect(this->sourceModel(), &QAbstractItemModel::rowsInserted, this, &QsTreeViewProxy::onRowsInserted);
		connect(this->sourceModel(), &QAbstractItemModel::rowsMoved, this, &QsTreeViewProxy::onRowsMoved);
		connect(this->sourceModel(), &QAbstractItemModel::rowsRemoved, this, &QsTreeViewProxy::onRowsRemoved);
	}

	endResetModel();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::setSectionFilter(int index, QString filter)
{
	QStringList keyWords = filter.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);

	if (!keyWords.size())
	{
		_filtersRegExp[index].setPattern("");
	}
	else
	{ // regexp qui dit tous ces mot dans n'importe quel ordre
		QString pattern ("^");
		for (QString keyWord : keyWords)
		{
			pattern.append(QString("(?=.*\\Q%1\\E)").arg(keyWord));
		}
		pattern.append(".+");
		_filtersRegExp[index].setPattern(pattern);
	}
	invalidateFilter();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onColumnsAboutToBeInserted(const QModelIndex & parent, int first, int last)
{
	beginInsertColumns(this->mapFromSource(parent), first, last);
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onColumnsAboutToBeMoved(const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationColumn)
{
	beginMoveColumns(this->mapFromSource(sourceParent), sourceStart, sourceEnd, this->mapFromSource(destinationParent), destinationColumn);
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onColumnsAboutToBeRemoved(const QModelIndex & parent, int first, int last)
{
	beginRemoveColumns(mapFromSource(parent), first, last);
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onModelReset()
{
	if (sourceModel())
	{
		while (_filtersRegExp.size() > sourceModel()->columnCount())
		{
			_filtersRegExp.removeLast();
		}
		while (_filtersRegExp.size() < sourceModel()->columnCount())
		{
			_filtersRegExp.append(QRegularExpression());
			_filtersRegExp.last().setPatternOptions(QRegularExpression::CaseInsensitiveOption);
		}
	}
	else
	{
		_filtersRegExp.clear();
	}

	endResetModel();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onRowsAboutToBeInserted(const QModelIndex &, int, int)
{
	beginResetModel();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onRowsAboutToBeMoved(const QModelIndex &, int, int, const QModelIndex &, int)
{
	beginResetModel();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onRowsAboutToBeRemoved(const QModelIndex &, int, int)
{
	beginResetModel();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onRowsInserted(const QModelIndex &, int, int)
{
	endResetModel();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onRowsMoved(const QModelIndex &, int, int, const QModelIndex &, int)
{
	endResetModel();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onRowsRemoved(const QModelIndex &, int, int)
{
	endResetModel();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onColumnsInserted(const QModelIndex &, int first, int last)
{
	for (int i = first; i <= last; i++)
		_filtersRegExp.insert(i, QRegularExpression());

	endInsertColumns();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onColumnsMoved(const QModelIndex &, int start, int end, const QModelIndex &, int column)
{
	for (int i = 0; i <= end - start; i++)
		_filtersRegExp.move(i + start, i + column);

	endMoveColumns();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onColumnsRemoved(const QModelIndex &, int first, int last)
{
	for (int i = first; i <= last; i++)
		_filtersRegExp.removeAt(i);

	endRemoveColumns();
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles /*= QVector<int>()*/)
{
	emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight), roles);
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onHeaderDataChanged(Qt::Orientation orientation, int first, int last)
{
	emit headerDataChanged(orientation, first, last);
}

//---------------------------------------------------------------------------------
void QsTreeViewProxy::onModelAboutToBeReset()
{
	beginResetModel();
}

//---------------------------------------------------------------------------------
bool QsTreeViewProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
	for (int col = 0; col < sourceModel()->columnCount(); col++)
	{
		QString cell = sourceModel()->data(sourceModel()->index(source_row, col, source_parent)).toString();
		if (!_filtersRegExp[col].match(cell).hasMatch())
			return false;
	}
	return true;
}
