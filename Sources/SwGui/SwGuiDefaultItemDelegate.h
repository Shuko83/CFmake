/*!
 \file SwGuiDefaultItemDelegate.h
 \brief ItemDelegate par defaut pour StreamWork
 \date 17/01/07 16:04:34
 \author F.Bighelli
*/

#ifndef _SwGuiDefaultItemDelegate_H
#define _SwGuiDefaultItemDelegate_H
/*
  * INCLUDES LOCAUX
  */
#include "SwGuiConstantes.h"
/*
  * INCLUDES GLOBAUX
  */
#include <QDynamicStyledItemDelegate.h>
#include <QRegularExpression>
#include <QFont>
#include <QFileDialog>
#include "Types/SwFileDescriptor.h"
#include "Types/SwIconDescriptor.h"
#include "Types/SwUUID.h"

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \class SwGuiDefaultItemDelegate
		 \brief ItemDelegate par defaut pour StreamWork
		*/
        class SWGUI_EXPORT SwGuiDefaultItemDelegate : public QDynamicStyledItemDelegate {
			Q_OBJECT
        public:
            /*! \brief Constructeur */
            SwGuiDefaultItemDelegate(QObject *parent = 0);
            /*! \brief Destructeur */
            ~SwGuiDefaultItemDelegate();

            /*! \brief Paint de l'item */
            void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

            /*! \brief Cree un editeur pour un item donne */
            QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const;
            /*! \brief Mets a jour l'editeur avec le contenu de l'item */
            void setEditorData(QWidget *editor, const QModelIndex &index) const;
            /*! \brief Mets a jour l'item avec le contenu de l'editeur */
            void setModelData(QWidget *editor, QAbstractItemModel *model,
                            const QModelIndex &index) const;
        private:
            mutable QRegularExpression boolExp;
            mutable QRegularExpression byteArrayExp;
            mutable QRegularExpression charExp;
            mutable QRegularExpression ipV4Exp;
            mutable QRegularExpression dateExp;
            mutable QRegularExpression dateTimeExp;
            mutable QRegularExpression doubleExp;
            mutable QRegularExpression pointExp;
			mutable QRegularExpression rectExp;
			mutable QRegularExpression rectFExp;
			mutable QRegularExpression signedIntegerExp;
            mutable QRegularExpression sizeExp;
            mutable QRegularExpression timeExp;
            mutable QRegularExpression uuidExp;
            mutable QRegularExpression unsignedIntegerExp;
            mutable QFont currentFont;
            mutable QColor currentColor;
            mutable SwCore::SwIconDescriptor currentIconDesc;
            mutable SwCore::SwIconDescriptor oldIconDesc;
            mutable SwCore::SwFileDescriptor currentFileDescriptor;
            mutable SwCore::SwFileDescriptor oldFileDescriptor;
            mutable SwCore::SwUUID currentUUID;
            mutable QWidget * currentWidgetFont;
            mutable QWidget * currentWidgetColor;
            mutable QWidget * currentWidgetIcon;
            mutable QWidget * currentWidgetFileDescriptor;
            mutable QFileDialog * _fdialog;
            mutable QDialog * _iconDialog;

        private:
            QDialog * getIconDialog() const;
        public slots:
            void onFontClick(bool checked=false);
            void onColorClick(bool checked=false);
            void onFileClick(bool checked=false);
            void onFileLoad(const QString & filename);
            void onIconClick(bool checked=false);
            void onIconLoad(const QString & filename);
		};
	}
}

#endif
