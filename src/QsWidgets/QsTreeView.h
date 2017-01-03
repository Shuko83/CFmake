#ifndef QS_WIDGETS_QSTREEVIEW_H
#define QS_WIDGETS_QSTREEVIEW_H

#include <QTreeView>
#include <QsWidgets_Global.h>

class QsTreeViewHeader;
class QsTreeViewProxy;
class QsTreeViewDialog;
class QItemSelectionProxyModel;

class QSWIDGETS_EXPORT QsTreeView : public QTreeView
{
	Q_OBJECT;
 	Q_PROPERTY(QString ClassCSS READ getClassCSS WRITE setClassCSS);
 	Q_PROPERTY(bool movingEnabled READ sectionsMovable WRITE setSectionsMovable);
 	Q_PROPERTY(bool filteringEnabled READ sectionsFilterable WRITE setSectionsFilterable);
 	Q_PROPERTY(bool hidingEnabled READ sectionsHideable WRITE setSectionsHideable);
	
public:
	explicit QsTreeView(QWidget *parent = 0);
	virtual ~QsTreeView();

	void setSelectionModel(QItemSelectionModel * selectionModel) override;
	void setModel(QAbstractItemModel * model) override;

 	QString getClassCSS() const;
 	void setClassCSS(QString val);

	QAbstractItemView::EditTriggers sectionEditTrigger(int section);
	void setSectionEditTrigger(int section, QAbstractItemView::EditTriggers trigger);
	 
	void setSectionEditTriggerSingleClick(int section, bool triggerOnSingleClick = true);
 
	bool sectionsMovable();
	void setSectionsMovable(bool movable);

	bool sectionsFilterable();
	void setSectionsFilterable(bool filterable);

	bool sectionsHideable();
	void setSectionsHideable(bool hideable);

	bool sectionComboFiltered(int index);
	void setSectionComboFiltered(int index, bool comboFiltered);

protected:
	void hideEvent(QHideEvent * event) override;
	void resizeEvent(QResizeEvent * event) override;
	void mousePressEvent(QMouseEvent *event) override;

	bool edit(const QModelIndex & index, EditTrigger trigger, QEvent * event) override;

	void onModelReset();
	void onColumnsInserted(const QModelIndex & parent, int first, int last);
	void onColumnsMoved(const QModelIndex & parent, int start, int end, const QModelIndex & destination, int column);
	void onColumnsRemoved(const QModelIndex & parent, int first, int last);

signals:
	void contextMenuRequested(QItemSelection selection, QMenu* menu);

private slots:
	void showColumnDialog();
	void hideColumnDialog();

private:
	QString _classCSS;
 	QsTreeViewHeader* _header;
 	QsTreeViewProxy* _proxy;
 	QsTreeViewDialog* _columnDialog;
 	QItemSelectionProxyModel* _selection;

	QList<int> _editTriggers;
};

#endif // QS_WIDGETS_QSTREEVIEW_H
