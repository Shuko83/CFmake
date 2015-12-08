/*!
 \file SwGuiDefaultItemDelegate.cpp
 \brief ItemDelegate par defaut pour StreamWork
 \date 17/01/07 16:04:34
 \author F.Bighelli
 */

#include "SwGuiDefaultItemDelegate.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include <QLocale>
#include <QMetaType>
#include <QFileDialog>
#include <SwEnum.h>
#include <SwIntegerEnum.h>
#include <SwInteger.h>
#include <SwString.h>
#include <SwDouble.h>
#include <SwIpv4Address.h>
#include <SwUUID.h>
#include "SwGuiEnumComboBox.h"
#include "SwGuiIntegerSpinBox.h"
#include "SwGuiDoubleSpinBox.h"
#include "SwGuiEnumIntComboBox.h"
#include "SwGuiStringLineEdit.h"
#include "_QRcViewer.h"
#include "..\SwCore\Main\SwMacros.h"

using namespace StreamWork::SwGui;
using namespace StreamWork::SwCore;

//------------------------------------------------------------------------------
SwGuiDefaultItemDelegate::SwGuiDefaultItemDelegate(QObject *parent) : QDynamicStyledItemDelegate(parent)
{
	currentWidgetIcon = NULL;
	boolExp.setPattern("true|false");
	boolExp.setCaseSensitivity(Qt::CaseInsensitive);
	currentWidgetFileDescriptor = 0;
	byteArrayExp.setPattern("[\\x00-\\xff]*");
	charExp.setPattern(".");
	ipV4Exp.setPattern("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})");
	//ipV4Exp.setPatternSyntax(QRegExp::Wildcard);
	doubleExp.setPattern("");
	pointExp.setPattern("\\((-?[0-9]*),(-?[0-9]*)\\)");
	rectExp.setPattern("\\((-?[0-9]*),(-?[0-9]*),(-?[0-9]*),(-?[0-9]*)\\)");
	rectFExp.setPattern("\\((-?[0-9]*(?:[.][0-9]*)?),(-?[0-9]*(?:[.][0-9]*)?),(-?[0-9]*(?:[.][0-9]*)?),(-?[0-9]*(?:[.][0-9]*)?)\\)");
	signedIntegerExp.setPattern("-?[0-9]*");
	sizeExp = pointExp;
	unsignedIntegerExp.setPattern("[0-9]*");

	dateExp.setPattern("([0-9]{,4})-([0-9]{,2})-([0-9]{,2})");
	timeExp.setPattern("([0-9]{,2}):([0-9]{,2}):([0-9]{,2})");
	uuidExp.setPattern("-?[0-9]*,-?[0-9]*");
	dateTimeExp.setPattern(dateExp.pattern() + "T" + timeExp.pattern());
	_fdialog = new QFileDialog(0, "Select");
	_fdialog->setModal(true);
	_iconDialog = 0;
	//_fdialog->setWindowFlags(Qt::WindowStaysOnTopHint);
	QObject::connect(_fdialog, SIGNAL(fileSelected(const QString &)), this, SLOT(onFileLoad(const QString &)));

}

//------------------------------------------------------------------------------
SwGuiDefaultItemDelegate::~SwGuiDefaultItemDelegate()
{
	delete _fdialog;
	if ( _iconDialog != 0 )
	{
		delete _iconDialog;
	}
}

//------------------------------------------------------------------------------
void SwGuiDefaultItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QDynamicStyledItemDelegate::paint(painter, option, index);
	return;
}

//------------------------------------------------------------------------------
QWidget *SwGuiDefaultItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & /* option */, const QModelIndex &index) const
{

	QVariant originalValue = index.model()->data(index, Qt::EditRole);

	QVariant displayRole = index.model()->data(index, Qt::DisplayRole);
	QString displayValue = "";
	if ( displayRole.isValid() )
		displayValue = displayRole.value<QString>();

	if ( !originalValue.isValid() )
		return 0;

	if ( qMetaTypeId<SwFileDescriptor>() == originalValue.userType() )
	{
		QWidget * w = new QWidget(parent);
		QHBoxLayout * hl = new QHBoxLayout(w);
		hl->setSpacing(0);
		hl->setMargin(0);
		QLineEdit * l = new QLineEdit(parent);
		//l->setAutoFillBackground(true);
		l->setText(displayValue);
		l->setObjectName("fileText");
		hl->addWidget(l);
		QPushButton * b = new QPushButton("...", w);
		b->setMaximumWidth(30);
		hl->addWidget(b);
		connect(b, SIGNAL(clicked(bool)), this, SLOT(onFileClick(bool)));
		currentFileDescriptor = originalValue.value<SwFileDescriptor>();
		_fdialog->setDirectory(QFileInfo(currentFileDescriptor.getDoubleDottedPath()).absoluteDir());
		_fdialog->selectFile(currentFileDescriptor.getDoubleDottedPath());
		currentWidgetFileDescriptor = w;
		oldFileDescriptor = currentFileDescriptor;
		return w;
	}
	if ( originalValue.type() == QVariant::Font )
	{
		QWidget * w = new QWidget(parent);
		QHBoxLayout * hl = new QHBoxLayout(w);
		hl->setSpacing(0);
		hl->setMargin(0);
		QLabel * l = new QLabel(w);
		l->setAutoFillBackground(true);
		l->setText(displayValue);
		hl->addWidget(l);
		QPushButton * b = new QPushButton("...", w);
		b->setMaximumWidth(30);
		hl->addWidget(b);
		connect(b, SIGNAL(clicked(bool)), this, SLOT(onFontClick(bool)));
		currentFont = originalValue.value<QFont>();
		currentWidgetFont = w;
		return w;
	}

	if ( originalValue.type() == QVariant::Color )
	{
		QWidget * w = new QWidget(parent);
		QHBoxLayout * hl = new QHBoxLayout(w);
		hl->setSpacing(0);
		hl->setMargin(0);
		QLabel * l = new QLabel(w);
		l->setAutoFillBackground(true);
		l->setText(displayValue);
		hl->addWidget(l);
		QPushButton * b = new QPushButton("...", w);
		b->setMaximumWidth(30);
		hl->addWidget(b);
		connect(b, SIGNAL(clicked(bool)), this, SLOT(onColorClick(bool)));
		currentColor = originalValue.value<QColor>();
		currentWidgetColor = w;
		return w;
	}

	if ( originalValue.type() == QVariant::Bool )
	{
		QComboBox * cbox = new QComboBox(parent);
		cbox->insertItem(0, "false");
		cbox->insertItem(1, "true");
		cbox->setCurrentIndex(originalValue.toBool() ? 1 : 0);
		cbox->setFrame(false);

		return cbox;
	}

	if ( qMetaTypeId<SwIntegerEnum>() == originalValue.userType() )
	{
		SwIntegerEnum venum = originalValue.value<SwIntegerEnum>();
		SwGuiEnumIntComboBox * cbox = new SwGuiEnumIntComboBox(venum, parent);
		return cbox;
	}

	if ( qMetaTypeId<SwEnum>() == originalValue.userType() )
	{
		SwEnum venum = originalValue.value<SwEnum>();
		SwGuiEnumComboBox * cbox = new SwGuiEnumComboBox(venum, parent);
		return cbox;
	}

	if ( qMetaTypeId<SwInteger>() == originalValue.userType() )
	{
		SwInteger vinteger = originalValue.value<SwInteger>();
		SwGuiIntegerSpinBox * cbox = new SwGuiIntegerSpinBox(vinteger, parent);
		return cbox;
	}

	if ( qMetaTypeId<SwString>() == originalValue.userType() )
	{
		SwString vstring = originalValue.value<SwString>();
		SwGuiStringLineEdit * cbox = new SwGuiStringLineEdit(vstring, parent);
		return cbox;
	}

	if ( qMetaTypeId<SwDouble>() == originalValue.userType() )
	{
		SwDouble vdouble = originalValue.value<SwDouble>();
		SwGuiDoubleSpinBox * cbox = new SwGuiDoubleSpinBox(vdouble, parent);
		return cbox;
	}

	if ( qMetaTypeId<SwIconDescriptor>() == originalValue.userType() )
	{
		QWidget * w = new QWidget(parent);
		QHBoxLayout * hl = new QHBoxLayout(w);
		hl->setSpacing(0);
		hl->setMargin(0);
		QLineEdit * l = new QLineEdit(w);

		l->setObjectName("iconText");
		l->setText(displayValue);
		hl->addWidget(l);
		QPushButton * b = new QPushButton("...", w);
		b->setMaximumWidth(30);
		hl->addWidget(b);
		connect(b, SIGNAL(clicked(bool)), this, SLOT(onIconClick(bool)));
		currentIconDesc = originalValue.value<SwIconDescriptor>();
		currentWidgetIcon = w;
		return w;
	}




	QLineEdit *lineEdit = new QLineEdit(parent);
	lineEdit->setFrame(false);

	QRegExp regExp;

	if ( qMetaTypeId<SwIpV4Address>() == originalValue.userType() )
	{
		//QValidator *validator = new QRegExpValidator(regExp, lineEdit);
		//lineEdit->setValidator(validator);
		return lineEdit;
	}
	if ( qMetaTypeId<SwUUID>() == originalValue.userType() )
	{
		regExp = uuidExp;
	}

	switch ( originalValue.type() )
	{
		case QVariant::Bool:
			regExp = boolExp;
			break;
		case QVariant::ByteArray:
			regExp = byteArrayExp;
			break;
		case QVariant::Char:
			regExp = charExp;
			break;
		case QVariant::Date:
			regExp = dateExp;
			break;
		case QVariant::DateTime:
			regExp = dateTimeExp;
			break;
		case QVariant::Double:
			regExp = doubleExp;
			break;
		case QVariant::Int:
		case QVariant::LongLong:
			regExp = signedIntegerExp;
			break;
		case QVariant::Point:
			regExp = pointExp;
			break;
		case QVariant::Rect:
			regExp = rectExp;
			break;
		case QVariant::RectF:
			regExp = rectFExp;
			break;
		case QVariant::Size:
			regExp = sizeExp;
			break;
		case QVariant::Time:
			regExp = timeExp;
			break;
		case QVariant::UInt:
		case QVariant::ULongLong:
			regExp = unsignedIntegerExp;
			break;
		default:
			;
	}

	if ( !regExp.isEmpty() )
	{
		QValidator *validator = new QRegExpValidator(regExp, lineEdit);
		lineEdit->setValidator(validator);
	}

	return lineEdit;
}

//------------------------------------------------------------------------------
void SwGuiDefaultItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QVariant value = index.model()->data(index, Qt::EditRole);

	QVariant displayRole = index.model()->data(index, Qt::DisplayRole);
	QString displayValue = "";
	if ( displayRole.isValid() )
		displayValue = displayRole.value<QString>();

	if ( value.type() == QVariant::Bool )
	{
		QComboBox * cbox = qobject_cast<QComboBox *>(editor);
		cbox->setCurrentIndex(value.toBool() ? 1 : 0);
		return;
	}

	if ( qMetaTypeId<SwIntegerEnum>() == value.userType() )
	{
		SwGuiEnumIntComboBox * cbox = qobject_cast<SwGuiEnumIntComboBox *>(editor);
		SwIntegerEnum venum = value.value<SwIntegerEnum>();
		cbox->SetEnum(venum);
		return;
	}
	if ( qMetaTypeId<SwEnum>() == value.userType() )
	{
		SwGuiEnumComboBox * cbox = qobject_cast<SwGuiEnumComboBox *>(editor);
		SwEnum venum = value.value<SwEnum>();
		cbox->SetEnum(venum);
		return;
	}

	if ( qMetaTypeId<SwInteger>() == value.userType() )
	{
		SwGuiIntegerSpinBox * cbox = qobject_cast<SwGuiIntegerSpinBox *>(editor);
		SwInteger vInteger = value.value<SwInteger>();
		cbox->SetInteger(vInteger);
		return;
	}

	if ( qMetaTypeId<SwString>() == value.userType() )
	{
		SwGuiStringLineEdit * cbox = qobject_cast<SwGuiStringLineEdit *>(editor);
		SwString vstring = value.value<SwString>();
		cbox->SetString(vstring);
		return;
	}

	if ( qMetaTypeId<SwDouble>() == value.userType() )
	{
		SwGuiDoubleSpinBox * cbox = qobject_cast<SwGuiDoubleSpinBox *>(editor);
		SwDouble vdouble = value.value<SwDouble>();
		cbox->SetDouble(vdouble);
		return;
	}

	if ( qMetaTypeId<SwFileDescriptor>() == value.userType() )
	{
		currentFileDescriptor = value.value<SwFileDescriptor>();
		_fdialog->selectFile(currentFileDescriptor.getDoubleDottedPath());
		_fdialog->setDirectory(QFileInfo(currentFileDescriptor.getDoubleDottedPath()).absoluteDir());
		oldFileDescriptor = currentFileDescriptor;
		return;
	}

	if ( qMetaTypeId<SwIconDescriptor>() == value.userType() )
	{
		currentIconDesc = value.value<SwIconDescriptor>();
		QDialog *dialog = getIconDialog();
		static_cast<_QRcViewer *>(dialog)->setIconName(currentIconDesc.ToString());
		oldIconDesc = currentIconDesc;
		return;
	}


	/*    if (qMetaTypeId<SwUUID>()==value.userType()) {
			SwUUID id=value.value<SwUUID>();
			lineEdit->setText(id.toQString());
			return;
			}
			*/
	if ( QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor) )
	{
		lineEdit->setText(displayValue);
		return;
	}

	if ( QLabel *label = qobject_cast<QLabel *>(editor->children().at(1)) )
	{
		label->setText(displayValue);
	}

}

//------------------------------------------------------------------------------
void SwGuiDefaultItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

	QVariant originalValue = index.model()->data(index, Qt::EditRole);
	if ( !originalValue.isValid() )
		originalValue = index.model()->data(index, Qt::DisplayRole);

	if ( originalValue.type() == QVariant::Font )
	{
		model->setData(index, QVariant(currentFont), Qt::EditRole);
		return;
	}

	if ( originalValue.type() == QVariant::Color )
	{
		model->setData(index, QVariant(currentColor), Qt::EditRole);
		return;
	}

	if ( originalValue.userType() == qMetaTypeId<SwIconDescriptor>() )
	{
		QVariant v;
		QLineEdit * lf = currentWidgetIcon->findChild<QLineEdit *>("iconText");
		if ( lf != 0 && lf->text() != oldIconDesc.ToString() )
		{
			currentIconDesc.setPath(lf->text());
		}
		v.setValue<SwIconDescriptor>(currentIconDesc);
		model->setData(index, v, Qt::EditRole);
		return;

	}

	if ( originalValue.userType() == qMetaTypeId<SwFileDescriptor>() )
	{
		QVariant v;
		QLineEdit * lf = currentWidgetFileDescriptor->findChild<QLineEdit *>("fileText");
		if ( lf != 0 && lf->text() != oldFileDescriptor.getFileName() )
		{
			currentFileDescriptor.setFileName(lf->text());
		}
		v.setValue<SwFileDescriptor>(currentFileDescriptor);
		model->setData(index, v, Qt::EditRole);
		return;
	}

	if ( originalValue.type() == QVariant::Bool )
	{
		QComboBox * cbox = qobject_cast<QComboBox *>(editor);
		if ( cbox->currentIndex() == 0 )
			model->setData(index, QVariant(false), Qt::EditRole);
		else
			model->setData(index, QVariant(true), Qt::EditRole);
		return;
	}

	if ( qMetaTypeId<SwIntegerEnum>() == originalValue.userType() )
	{
		SwGuiEnumIntComboBox * cbox = qobject_cast<SwGuiEnumIntComboBox *>(editor);
		QVariant new_val;
		new_val.setValue(cbox->GetEnum());
		model->setData(index, new_val, Qt::EditRole);
		return;
	}
	if ( qMetaTypeId<SwEnum>() == originalValue.userType() )
	{
		SwGuiEnumComboBox * cbox = qobject_cast<SwGuiEnumComboBox *>(editor);
		QVariant new_val;
		new_val.setValue(cbox->GetEnum());
		model->setData(index, new_val, Qt::EditRole);
		return;
	}

	if ( qMetaTypeId<SwInteger>() == originalValue.userType() )
	{
		SwGuiIntegerSpinBox * cbox = qobject_cast<SwGuiIntegerSpinBox *>(editor);
		QVariant new_val;
		new_val.setValue(cbox->GetInteger());
		model->setData(index, new_val, Qt::EditRole);
		return;
	}

	if ( qMetaTypeId<SwString>() == originalValue.userType() )
	{
		SwGuiStringLineEdit * cbox = qobject_cast<SwGuiStringLineEdit *>(editor);
		QVariant new_val;
		new_val.setValue(cbox->GetString());
		model->setData(index, new_val, Qt::EditRole);
		return;
	}

	if ( qMetaTypeId<SwDouble>() == originalValue.userType() )
	{
		SwGuiDoubleSpinBox * cbox = qobject_cast<SwGuiDoubleSpinBox *>(editor);
		QVariant new_val;
		new_val.setValue(cbox->GetDouble());
		model->setData(index, new_val, Qt::EditRole);
		return;
	}

	QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
	if ( !lineEdit->isModified() )
		return;

	QString text = lineEdit->text();
	const QValidator *validator = lineEdit->validator();
	if ( validator )
	{
		int pos;
		if ( validator->validate(text, pos) != QValidator::Acceptable )
			return;
	}

	QVariant value;

	if ( originalValue.userType() == qMetaTypeId<SwIpV4Address>() )
	{
		SwIpV4Address ipv4;
		ipv4.FromString(text);
		QVariant v;
		v.setValue<SwIpV4Address>(ipv4);
		model->setData(index, v, Qt::EditRole);
		return;
	}

	if ( originalValue.userType() == qMetaTypeId<SwUUID>() )
	{
		SwUUID id;
		id.FromQString(text);
		QVariant v;
		v.setValue<SwUUID>(id);
		model->setData(index, v, Qt::EditRole);
		return;
	}


	switch ( originalValue.type() )
	{
		case QVariant::Char:
			value = text.at(0);
			break;
		case QVariant::Date:
		{
			QDate date = QDate::fromString(text, Qt::ISODate);
			if ( !date.isValid() )
				return;
			value = date;
		}
		break;
		case QVariant::DateTime:
		{
			QDateTime dateTime = QDateTime::fromString(text, Qt::ISODate);
			if ( !dateTime.isValid() )
				return;
			value = dateTime;
		}
		break;
		case QVariant::Point:
			pointExp.exactMatch(text);
			value = QPoint(pointExp.cap(1).toInt(), pointExp.cap(2).toInt());
			break;
		case QVariant::Rect:
			rectExp.exactMatch(text);
			value = QRect(rectExp.cap(1).toInt(), rectExp.cap(2).toInt(),
						  rectExp.cap(3).toInt(), rectExp.cap(4).toInt());
			break;
		case QVariant::RectF:
			rectFExp.exactMatch(text);
			value = QRectF(rectFExp.cap(1).toDouble(), rectFExp.cap(2).toDouble(),
						   rectFExp.cap(3).toDouble(), rectFExp.cap(4).toDouble());
			break;
		case QVariant::Size:
			sizeExp.exactMatch(text);
			value = QSize(sizeExp.cap(1).toInt(), sizeExp.cap(2).toInt());
			break;
		case QVariant::StringList:
			value = text.split(",");
			break;
		case QVariant::Time:
		{
			QTime time = QTime::fromString(text, Qt::ISODate);
			if ( !time.isValid() )
				return;
			value = time;
		}
		break;
		default:
			value = text;
			value.convert(originalValue.type());
	}

	model->setData(index, value, Qt::EditRole);
}

//------------------------------------------------------------------------------
void SwGuiDefaultItemDelegate::onFontClick(bool checked)
{
	bool result = false;
	QFont tmpfont = QFontDialog::getFont(&result, currentFont, 0, "Edit Font");
	if ( result )
	{
		currentFont = tmpfont;
		if ( QLabel *label = qobject_cast<QLabel *>(currentWidgetFont->children().at(1)) )
		{
			label->setText(currentFont.toString());
		}
		commitData(currentWidgetFont);
	}
	emit closeEditor(currentWidgetFont);
}

//------------------------------------------------------------------------------
void SwGuiDefaultItemDelegate::onColorClick(bool checked)
{
	QColor tmpColor = QColorDialog::getColor(currentColor, 0, "Select color", QColorDialog::ShowAlphaChannel);
	if ( !tmpColor.isValid() )
	{
		return;
	}
	currentColor = tmpColor;
	if ( QLabel *label = qobject_cast<QLabel *>(currentWidgetColor->children().at(1)) )
	{
		QString colorS = currentColor.name().toUpper();
		QString alphaS = QString::number(currentColor.alpha(), 16).toUpper();
		if ( alphaS.length() == 1 )
		{
			alphaS = QString("0") + alphaS;
		}
		label->setText(colorS + alphaS);
	}
	commitData(currentWidgetColor);
	emit closeEditor(currentWidgetColor);

}

//------------------------------------------------------------------------------
void SwGuiDefaultItemDelegate::onFileLoad(const QString & filename)
{
	//On met le path relative a swapp applicationdirpath

	QDir tmp (SW_APP->GetApplicationDirPath());
	currentFileDescriptor.setFileName(tmp.relativeFilePath(filename));
	if ( QLineEdit *label = qobject_cast<QLineEdit *>(currentWidgetFileDescriptor->children().at(1)) )
		label->setText(currentFileDescriptor.getFileName());

	commitData(currentWidgetFileDescriptor);
	emit closeEditor(currentWidgetFileDescriptor);
}

//------------------------------------------------------------------------------
void SwGuiDefaultItemDelegate::onFileClick(bool checked)
{

	if ( currentFileDescriptor.getFileType() == SwFileDescriptor::DirectorySelect )
	{
		_fdialog->setAcceptMode(QFileDialog::AcceptOpen);
		_fdialog->setWindowTitle("Select Directory");
		_fdialog->setFileMode(QFileDialog::Directory);
		_fdialog->setOption(QFileDialog::ShowDirsOnly);
	}
	if ( currentFileDescriptor.getFileType() == SwFileDescriptor::FileRead )
	{
		_fdialog->setAcceptMode(QFileDialog::AcceptOpen);
		_fdialog->setWindowTitle("Select file to read");
		_fdialog->setNameFilter(currentFileDescriptor.getFilter());
		_fdialog->setFileMode(QFileDialog::ExistingFile);
	}
	if ( currentFileDescriptor.getFileType() == SwFileDescriptor::FileWrite )
	{
		_fdialog->setAcceptMode(QFileDialog::AcceptSave);
		_fdialog->setWindowTitle("Select file to read");
		_fdialog->setNameFilter(currentFileDescriptor.getFilter());
		_fdialog->setFileMode(QFileDialog::AnyFile);
	}
	_fdialog->show();

}

//------------------------------------------------------------------------------
void SwGuiDefaultItemDelegate::onIconClick(bool checked)
{
	getIconDialog()->show();
}

//------------------------------------------------------------------------------
void SwGuiDefaultItemDelegate::onIconLoad(const QString & filename)
{
	currentIconDesc.setPath(filename);
	if ( QLineEdit *label = qobject_cast<QLineEdit *>(currentWidgetIcon->children().at(1)) )
	{
		label->setText(currentIconDesc.ToString());
	}
	commitData(currentWidgetIcon);
}

//------------------------------------------------------------------------------
QDialog * SwGuiDefaultItemDelegate::getIconDialog() const
{
	if ( _iconDialog == 0 )
	{
		_iconDialog = new _QRcViewer;
		_iconDialog->setModal(true);
		QObject::connect(_iconDialog, SIGNAL(iconSelected(const QString &)), this, SLOT(onIconLoad(const QString &)));
	}
	return _iconDialog;
}
