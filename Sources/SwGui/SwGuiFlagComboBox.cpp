#include "SwGuiFlagComboBox.h"

#include "SwGuiFlagComboBoxModel.h"

#include <QItemDelegate>
#include <QStylePainter>

using namespace StreamWork::SwGui;

//-------------------------------------------------------------------------
SwGuiFlagComboBox::SwGuiFlagComboBox(const QMetaEnum & metaEnum, QWidget *parent)
    : QComboBox(parent), _metaEnum(metaEnum)
{
	_model = new SwGuiFlagComboBoxModel(_metaEnum);
	setModel(_model);

	setFrame(false);

    setEditable(false);

	this->setToolTip("This property is a QEnum");

	connect(this, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &SwGuiFlagComboBox::onActivated);
}

//---------------------------------------------------------------------------------
SwGuiFlagComboBox::~SwGuiFlagComboBox()
{
}

//---------------------------------------------------------------------------------
void SwGuiFlagComboBox::setValue(int value)
{
	_model->setValue(value);
}

//---------------------------------------------------------------------------------
int SwGuiFlagComboBox::getValue() const
{
	return _model->getValue();
}

//---------------------------------------------------------------------------------
void SwGuiFlagComboBox::paintEvent(QPaintEvent *)
{
	QStylePainter painter(this);
	painter.setPen(palette().color(QPalette::Text));

	// draw the combobox frame, focusrect and selected etc.
	QStyleOptionComboBox opt;
	initStyleOption(&opt);
	opt.currentText = _metaEnum.valueToKeys(_model->getValue());
	painter.drawComplexControl(QStyle::CC_ComboBox, opt);

	// draw the icon and text
	painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}

//-------------------------------------------------------------------------
void SwGuiFlagComboBox::onActivated(int index)
{
	QVariant value = itemData(index, Qt::CheckStateRole);
	Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
	setItemData(index, (state == Qt::Checked ? Qt::Unchecked : Qt::Checked), Qt::CheckStateRole);

	showPopup();
}
