/*!
 \file SwGuiDefaultItemDelegate.cpp
 \brief ItemDelegate par defaut pour StreamWork
 \date 17/01/07 16:04:34
 \author F.Bighelli
*/

#include "SwGuiDefaultItemDelegate.h"
#include <QtGui>
#include <QLocale>
#include <QMetaType>
#include <QFileDialog>
#include <SwEnum.h>
#include <SwIpv4Address.h>
#include <SwUUID.h>
#include "SwGuiEnumComboBox.h"

using namespace StreamWork::SwGui;
using namespace StreamWork::SwCore;

/*! \brief Constructeur */
SwGuiDefaultItemDelegate::SwGuiDefaultItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    boolExp.setPattern("true|false");
    boolExp.setCaseSensitivity(Qt::CaseInsensitive);
    currentWidgetFileDescriptor=0;
    byteArrayExp.setPattern("[\\x00-\\xff]*");
    charExp.setPattern(".");
    ipV4Exp.setPattern("(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})");
    //ipV4Exp.setPatternSyntax(QRegExp::Wildcard);
    doubleExp.setPattern("");
    pointExp.setPattern("\\((-?[0-9]*),(-?[0-9]*)\\)");
    rectExp.setPattern("\\((-?[0-9]*),(-?[0-9]*),(-?[0-9]*),(-?[0-9]*)\\)");
    signedIntegerExp.setPattern("-?[0-9]*");
    sizeExp = pointExp;
    unsignedIntegerExp.setPattern("[0-9]*");

    dateExp.setPattern("([0-9]{,4})-([0-9]{,2})-([0-9]{,2})");
    timeExp.setPattern("([0-9]{,2}):([0-9]{,2}):([0-9]{,2})");
    uuidExp.setPattern("-?[0-9]*,-?[0-9]*");
    dateTimeExp.setPattern(dateExp.pattern() + "T" + timeExp.pattern());
    _fdialog=new QFileDialog(0,"Select");
    _fdialog->setModal(true);
    //_fdialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    QObject::connect(_fdialog,SIGNAL(fileSelected( const QString &)),this,SLOT(onFileLoad(const QString &)));

}
/*! \brief Destructeur */
SwGuiDefaultItemDelegate::~SwGuiDefaultItemDelegate() {
    delete _fdialog;
}
/*! \brief Paint de l'item */
void SwGuiDefaultItemDelegate::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::DisplayRole);
    if (!isSupportedType(value)) {
        QStyleOptionViewItem myOption = option;
        myOption.state &= ~QStyle::State_Enabled;
        QItemDelegate::paint(painter, myOption, index);
        return;
    }
    //QItemDelegate::paint(painter, option, index);
    QStyleOptionViewItemV2 opt = setOptions(index, option);
    const QStyleOptionViewItemV2 *v2 = qstyleoption_cast<const QStyleOptionViewItemV2 *>(&option);
    opt.features = v2 ? v2->features : QStyleOptionViewItemV2::ViewItemFeatures(QStyleOptionViewItemV2::None);

    // prepare
    painter->save();
    //if (d->clipPainting)
    //    painter->setClipRect(opt.rect);

    // get the data and the rectangles

    QIcon icon;
    QIcon::Mode iconMode = QIcon::Normal;//d->iconMode(option.state);
    QIcon::State iconState = QIcon::On;//d->iconState(option.state);

    QPixmap pixmap;
    QRect decorationRect;
    value = index.data(Qt::DecorationRole);
    if (value.isValid()) {
        if (value.type() == QVariant::Icon) {
            icon = qvariant_cast<QIcon>(value);
            decorationRect = QRect(QPoint(0, 0),
                                   icon.actualSize(option.decorationSize, iconMode, iconState));
        } else {
            pixmap = decoration(opt, value);
            decorationRect = QRect(QPoint(0, 0), pixmap.size());
        }
    }

    QString text;
    QRect displayRect;
    value = index.data(Qt::DisplayRole);
    if (value.isValid()) {
        text=displayText(value);
        painter->save();
        painter->setFont(opt.font);
        displayRect = textRectangle(painter, textLayoutBounds(opt), opt.font, text);
        painter->restore();
    }

    QRect checkRect;
    Qt::CheckState checkState = Qt::Unchecked;
    value = index.data(Qt::CheckStateRole);
    if (value.isValid()) {
        checkState = static_cast<Qt::CheckState>(value.toInt());
        checkRect = check(opt, opt.rect, value);
    }

    // do the layout

    doLayout(opt, &checkRect, &decorationRect, &displayRect, false);

    // draw the item

    drawBackground(painter, opt, index);
    drawCheck(painter, opt, checkRect, checkState);
    if (!icon.isNull())
        icon.paint(painter, decorationRect, option.decorationAlignment, iconMode, iconState);
    else
        drawDecoration(painter, opt, decorationRect, pixmap);
    drawDisplay(painter, opt, displayRect, text);
    drawFocus(painter, opt, text.isEmpty() ? QRect() : displayRect);

    // done
    painter->restore();
}
 /*! \brief Permet de recuperer le rectangle englobant */
QRect SwGuiDefaultItemDelegate::textLayoutBounds(const QStyleOptionViewItemV2 &option) const
{
    QRect rect = option.rect;
    const bool wrapText = option.features & QStyleOptionViewItemV2::WrapText;
    switch (option.decorationPosition) {
    case QStyleOptionViewItem::Left:
    case QStyleOptionViewItem::Right:
        rect.setWidth(INT_MAX >> 6);
        break;
    case QStyleOptionViewItem::Top:
    case QStyleOptionViewItem::Bottom:
        rect.setWidth(wrapText ? option.decorationSize.width() : (INT_MAX >> 6));
        break;
    }

    return rect;
}
/*! \brief Cree un editeur pour un item donne */
QWidget *SwGuiDefaultItemDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem & /* option */,
        const QModelIndex &index) const
{

    QVariant originalValue = index.model()->data(index, Qt::EditRole);
    if (!originalValue.isValid())
        originalValue= index.model()->data(index, Qt::DisplayRole);

    if (!isSupportedType(originalValue))
        return 0;

    if (qMetaTypeId<SwFileDescriptor>()==originalValue.userType()) {
        QWidget * w=new QWidget(parent);
        QHBoxLayout * hl=new QHBoxLayout(w);
        hl->setSpacing(0);
        hl->setMargin(0);
        QLineEdit * l=new QLineEdit(w);
        //l->setAutoFillBackground(true);
        l->setText(displayText(originalValue));
        l->setObjectName("fileText");
        hl->addWidget(l);
        QPushButton * b=new QPushButton("...",w);
        b->setMaximumWidth(30);
        hl->addWidget(b);
        connect(b,SIGNAL(clicked(bool)),this,SLOT(onFileClick(bool)));
        currentFileDescriptor=originalValue.value<SwFileDescriptor>();
        _fdialog->selectFile(currentFileDescriptor.getFileName());
        currentWidgetFileDescriptor=w;
        oldFileDescriptor=currentFileDescriptor;
        return w;
    }
    if (originalValue.type()==QVariant::Font) {
        QWidget * w=new QWidget(parent);
        QHBoxLayout * hl=new QHBoxLayout(w);
        hl->setSpacing(0);
        hl->setMargin(0);
        QLabel * l=new QLabel(w);
        l->setAutoFillBackground(true);
        l->setText(displayText(originalValue));
        hl->addWidget(l);
        QPushButton * b=new QPushButton("...",w);
        b->setMaximumWidth(30);
        hl->addWidget(b);
        connect(b,SIGNAL(clicked(bool)),this,SLOT(onFontClick(bool)));
        currentFont=originalValue.value<QFont>();
        currentWidgetFont=w;
        return w;
    }

    if (originalValue.type()==QVariant::Color) {
        QWidget * w=new QWidget(parent);
        QHBoxLayout * hl=new QHBoxLayout(w);
        hl->setSpacing(0);
        hl->setMargin(0);
        QLabel * l=new QLabel(w);
        l->setAutoFillBackground(true);
        l->setText(displayText(originalValue));
        hl->addWidget(l);
        QPushButton * b=new QPushButton("...",w);
        b->setMaximumWidth(30);
        hl->addWidget(b);
        connect(b,SIGNAL(clicked(bool)),this,SLOT(onColorClick(bool)));
        currentColor=originalValue.value<QColor>();
        currentWidgetColor=w;
        return w;
    }

    if (originalValue.type()==QVariant::Bool) {
        QComboBox * cbox=new QComboBox(parent);
        cbox->insertItem(0,"false");
        cbox->insertItem(1,"true");
        cbox->setCurrentIndex(originalValue.toBool()?1:0);
        cbox->setFrame(false);
        return cbox;
    }

    if (qMetaTypeId<SwEnum>()==originalValue.userType()) {
        SwEnum venum=originalValue.value<SwEnum>();
        SwGuiEnumComboBox * cbox=new SwGuiEnumComboBox(venum,parent);
        return cbox;
    }
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setFrame(false);

    QRegExp regExp;

    if (qMetaTypeId<SwIpV4Address>()==originalValue.userType()) {
       //QValidator *validator = new QRegExpValidator(regExp, lineEdit);
       //lineEdit->setValidator(validator);
       return lineEdit;
    }
    if (qMetaTypeId<SwUUID>()==originalValue.userType()) {
       regExp=uuidExp;
    }

    switch (originalValue.type()) {
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

    if (!regExp.isEmpty()) {
        QValidator *validator = new QRegExpValidator(regExp, lineEdit);
        lineEdit->setValidator(validator);
    }

    return lineEdit;
}
/*! \brief Mets a jour l'editeur avec le contenu de l'item */
void SwGuiDefaultItemDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::EditRole);
    if (!value.isValid())
        value= index.model()->data(index, Qt::DisplayRole);

    if (value.type()==QVariant::Bool) {
        QComboBox * cbox=qobject_cast<QComboBox *>(editor);
        cbox->setCurrentIndex(value.toBool()?1:0);
        return;
    }

    if (qMetaTypeId<SwEnum>()==value.userType()) {
        SwGuiEnumComboBox * cbox=qobject_cast<SwGuiEnumComboBox *>(editor);
        SwEnum venum=value.value<SwEnum>();
        cbox->SetEnum(venum);
        return;
    }

    if (qMetaTypeId<SwFileDescriptor>()==value.userType()) {
        currentFileDescriptor=value.value<SwFileDescriptor>();
        _fdialog->selectFile(currentFileDescriptor.getFileName());
        oldFileDescriptor=currentFileDescriptor;
        return;
    }


/*    if (qMetaTypeId<SwUUID>()==value.userType()) {
        SwUUID id=value.value<SwUUID>();
        lineEdit->setText(id.toQString());
        return;
    }
*/
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor)) {
        lineEdit->setText(displayText(value));
        return;
    }

    if (QLabel *label = qobject_cast<QLabel *>(editor->children().at(1))) {
        label->setText(displayText(value));
    }
    
}
/*! \brief Mets a jour l'item avec le contenu de l'editeur */
void SwGuiDefaultItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{

    QVariant originalValue = index.model()->data(index, Qt::EditRole);
    if (!originalValue.isValid())
        originalValue= index.model()->data(index, Qt::DisplayRole);

    if (originalValue.type()==QVariant::Font) {
        model->setData(index, QVariant(currentFont), Qt::EditRole);
        return;
    }

    if (originalValue.type()==QVariant::Color) {
        model->setData(index, QVariant(currentColor), Qt::EditRole);
        return;
    }

    if (originalValue.userType()==qMetaTypeId<SwFileDescriptor>()) {
        QVariant v;
        QLineEdit * lf=currentWidgetFileDescriptor->findChild<QLineEdit *>("fileText");
        if (lf!=0 && lf->text()!=oldFileDescriptor.getFileName()) {
            currentFileDescriptor.setFileName(lf->text());
        }
        v.setValue<SwFileDescriptor>(currentFileDescriptor);
        model->setData(index, v, Qt::EditRole);
        return;
    }

    if (originalValue.type()==QVariant::Bool) {
        QComboBox * cbox=qobject_cast<QComboBox *>(editor);
        if (cbox->currentIndex()==0)
            model->setData(index, QVariant(false), Qt::EditRole);
        else
            model->setData(index, QVariant(true), Qt::EditRole);
        return;
    }

    if (qMetaTypeId<SwEnum>()==originalValue.userType()) {
        SwGuiEnumComboBox * cbox=qobject_cast<SwGuiEnumComboBox *>(editor);
        QVariant new_val;
        new_val.setValue(cbox->GetEnum());
        model->setData(index,new_val, Qt::EditRole);
        return;
    }


    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (!lineEdit->isModified())
        return;

    QString text = lineEdit->text();
    const QValidator *validator = lineEdit->validator();
    if (validator) {
        int pos;
        if (validator->validate(text, pos) != QValidator::Acceptable)
            return;
    }

    QVariant value;

    if (originalValue.userType()==qMetaTypeId<SwIpV4Address>()) {
        SwIpV4Address ipv4;
        ipv4.FromString(text);
        QVariant v;
        v.setValue<SwIpV4Address>(ipv4);
        model->setData(index, v, Qt::EditRole);
        return;
    }

    if (originalValue.userType()==qMetaTypeId<SwUUID>()) {
        SwUUID id;
        id.FromQString(text);
        QVariant v;
        v.setValue<SwUUID>(id);
        model->setData(index, v, Qt::EditRole);
        return;
    }


    switch (originalValue.type()) {
    case QVariant::Char:
        value = text.at(0);
        break;
    case QVariant::Date:
        {
            QDate date = QDate::fromString(text, Qt::ISODate);
            if (!date.isValid())
                return;
            value = date;
        }
        break;
    case QVariant::DateTime:
        {
            QDateTime dateTime = QDateTime::fromString(text, Qt::ISODate);
            if (!dateTime.isValid())
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
            if (!time.isValid())
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
/*! \brief Permet de savoir si le type de l'item est support� */
bool SwGuiDefaultItemDelegate::isSupportedType(QVariant & val)
{
    switch (val.type()) {
    case QVariant::Bool:
    case QVariant::ByteArray:
    case QVariant::Char:
    case QVariant::Color:
    case QVariant::Date:
    case QVariant::DateTime:
    case QVariant::Double:
    case QVariant::Font:
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::Point:
    case QVariant::Rect:
    case QVariant::Size:
    case QVariant::String:
    case QVariant::StringList:
    case QVariant::Time:
    case QVariant::UInt:
    case QVariant::ULongLong:
        return true;
    case QVariant::UserType:
        if (qMetaTypeId<SwEnum>()==val.userType()) {
            return true;
        }
        if (qMetaTypeId<SwFileDescriptor>()==val.userType()) {
            return true;
        }
        if (qMetaTypeId<SwIpV4Address>()==val.userType()) {
            return true;
        }
        if (qMetaTypeId<SwUUID>()==val.userType()) {
            return true;
        }
    default:
        break;
    }
    return false;
}
/*! \brief Permet d'afficher l'item au format texte */
QString SwGuiDefaultItemDelegate::displayText(const QVariant &value)
{
    switch (value.type()) {
    case QVariant::Bool:
    case QVariant::ByteArray:
    case QVariant::Char:
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::String:
    case QVariant::UInt:
    case QVariant::ULongLong:
        return value.toString();
    case QVariant::Double:
        return QLocale().toString(value.toDouble());
    case QVariant::Color:
        {
            QColor color = qvariant_cast<QColor>(value);
            QString text = color.name ().toUpper ();
            QString alphaS = QString::number (color.alpha (), 16).toUpper ();
            if (alphaS.length () == 1)
            {
                alphaS = QString ("0") + alphaS;
            }
            text += alphaS;
            return text;
        }
    case QVariant::Font:
        {
            return value.value<QFont>().toString();
        }
    case QVariant::Date:
        return value.toDate().toString(Qt::ISODate);
    case QVariant::DateTime:
        return value.toDateTime().toString(Qt::ISODate);
    case QVariant::Invalid:
        return "<Invalid>";
    case QVariant::Point:
        {
            QPoint point = value.toPoint();
            return QString("(%1,%2)").arg(point.x()).arg(point.y());
        }
    case QVariant::Rect:
        {
            QRect rect = value.toRect();
            return QString("(%1,%2,%3,%4)")
                   .arg(rect.x()).arg(rect.y())
                   .arg(rect.width()).arg(rect.height());
        }
    case QVariant::Size:
        {
            QSize size = value.toSize();
            return QString("(%1,%2)").arg(size.width()).arg(size.height());
        }
    case QVariant::StringList:
        return value.toStringList().join(",");
    case QVariant::Time:
        return value.toTime().toString(Qt::ISODate);
    case QVariant::UserType:
        if (qMetaTypeId<SwEnum>()==value.userType()) {
            SwEnum venum=value.value<SwEnum>();
            return venum.ToString();
        }
        if (qMetaTypeId<SwFileDescriptor>()==value.userType()) {
            return value.value<SwFileDescriptor>().getFileName();
        }
        if (qMetaTypeId<SwIpV4Address>()==value.userType()) {
            return value.value<SwIpV4Address>().ToString();
        }
        if (qMetaTypeId<SwUUID>()==value.userType()) {
            return value.value<SwUUID>().toQString();
        }
    }
    return QString("<%1>").arg(value.typeName());
}
void SwGuiDefaultItemDelegate::onFontClick(bool checked) {
    bool result=false;
    QFont tmpfont=QFontDialog::getFont (&result,currentFont,0,"Edit Font");
    if(result) {
        currentFont=tmpfont;
        if (QLabel *label = qobject_cast<QLabel *>(currentWidgetFont->children().at(1))) {
            label->setText(currentFont.toString());
        }
        commitData(currentWidgetFont);
    }
    emit closeEditor(currentWidgetFont);
}
void SwGuiDefaultItemDelegate::onColorClick(bool checked) {
    QColor tmpColor=QColorDialog::getColor (currentColor,0,"Select color",QColorDialog::ShowAlphaChannel);
    currentColor=tmpColor;
    if (QLabel *label = qobject_cast<QLabel *>(currentWidgetColor->children().at(1))) {
        QString colorS = currentColor.name ().toUpper();
        QString alphaS = QString::number (currentColor.alpha (), 16).toUpper ();
        if (alphaS.length () == 1)
        {
            alphaS = QString ("0") + alphaS;
        }
        label->setText(colorS + alphaS);
    }
    commitData(currentWidgetColor);
    emit closeEditor(currentWidgetColor);

}
void SwGuiDefaultItemDelegate::onFileLoad(const QString & filename) {
    currentFileDescriptor.setFileName(filename);
    if (QLineEdit *label = qobject_cast<QLineEdit *>(currentWidgetFileDescriptor->children().at(1))) {
        label->setText(currentFileDescriptor.getFileName());
    }
    commitData(currentWidgetFileDescriptor);
    emit closeEditor(currentWidgetFileDescriptor);
}
void SwGuiDefaultItemDelegate::onFileClick(bool checked) {

    if (currentFileDescriptor.getFileType()==SwFileDescriptor::DirectorySelect) {
        _fdialog->setAcceptMode(QFileDialog::AcceptOpen);
        _fdialog->setWindowTitle("Select Directory");
        _fdialog->setFileMode(QFileDialog::Directory);
        _fdialog->setOption(QFileDialog::ShowDirsOnly);
    }
    if (currentFileDescriptor.getFileType()==SwFileDescriptor::FileRead) {
        _fdialog->setAcceptMode(QFileDialog::AcceptOpen);
        _fdialog->setWindowTitle("Select file to read");
        _fdialog->setNameFilter(currentFileDescriptor.getFilter());
        _fdialog->setFileMode(QFileDialog::ExistingFile);
    }
    if (currentFileDescriptor.getFileType()==SwFileDescriptor::FileWrite) {
        _fdialog->setAcceptMode(QFileDialog::AcceptSave);
        _fdialog->setWindowTitle("Select file to read");
        _fdialog->setNameFilter(currentFileDescriptor.getFilter());
        _fdialog->setFileMode(QFileDialog::AnyFile);
    }
    _fdialog->show();

}
