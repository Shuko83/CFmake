#pragma once

#include <QHeaderView>
#include <QPushButton>
#include <QDialog>

class QsTreeViewHeader : public QHeaderView
{
	Q_OBJECT
public:
	explicit QsTreeViewHeader(QWidget * parent = 0);
	virtual ~QsTreeViewHeader();

	bool sectionsFilterable();
	void setSectionsFilterable(bool filterable);

	bool sectionsHideable();
	void setSectionsHideable(bool hideable);

	bool sectionComboFiltered(int logicalIndex);
	void setSectionComboFiltered(int logicalIndex, bool comboFiltered);

	virtual void setModel(QAbstractItemModel *model);
	virtual QSize sizeHint() const;

signals:
	void columnFilterTextChanged(int columnIndex, const QString& filterText);
	void cornerButtonClicked();

private slots:
	void onSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
	void onSectionResized(int logicalIndex, int oldSize, int newSize);

	void onTextChanged(const QString &);

	void initFilters();
	void updateFiltersCount();
	void updateFiltersGeometry();
	void updateComboFilters();

	void updateSettingsButtonGeometry();

	virtual void showEvent(QShowEvent *);
	virtual void resizeEvent(QResizeEvent *e);

private:
	QAbstractItemModel* _comboModel;

	bool _sectionsFilterable;
	bool _sectionsHideable;
	QList<bool> _sectionsComboFiltered;

	QList<QWidget*> _filters;
	QPushButton* _settingsButton;
};
