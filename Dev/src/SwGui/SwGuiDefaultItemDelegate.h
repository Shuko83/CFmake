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
#include <QItemDelegate>
#include <QRegExp>
#include <QFont>
#include <QFileDialog>
#include "SwFileDescriptor.h"
#include "SwUUID.h"

namespace StreamWork
{
	namespace SwGui
	{
        /*!
		 \class SwGuiDefaultItemDelegate
		 \brief ItemDelegate par defaut pour StreamWork
		*/
        class SWGUI_EXPORT SwGuiDefaultItemDelegate : public QItemDelegate {
			Q_OBJECT
        public:
            /*! \brief Constructeur */
            SwGuiDefaultItemDelegate(QObject *parent = 0);
            /*! \brief Destructeur */
            ~SwGuiDefaultItemDelegate();

            /*! \brief Paint de l'item */
            void paint(QPainter *painter, const QStyleOptionViewItem &option,
                    const QModelIndex &index) const;
            /*! \brief Permet de recuperer le rectangle englobant */
            QRect textLayoutBounds(const QStyleOptionViewItemV2 &option) const;
            /*! \brief Cree un editeur pour un item donne */
            QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const;
            /*! \brief Mets a jour l'editeur avec le contenu de l'item */
            void setEditorData(QWidget *editor, const QModelIndex &index) const;
            /*! \brief Mets a jour l'item avec le contenu de l'editeur */
            void setModelData(QWidget *editor, QAbstractItemModel *model,
                            const QModelIndex &index) const;

            /*! \brief Permet de savoir si le type de l'item est support� */
            static bool isSupportedType(QVariant & val/*=QVariant()*/);
            /*! \brief Permet d'afficher l'item au format texte */
            static QString displayText(const QVariant &value);

        private:
            mutable QRegExp boolExp;
            mutable QRegExp byteArrayExp;
            mutable QRegExp charExp;
            mutable QRegExp ipV4Exp;
            mutable QRegExp dateExp;
            mutable QRegExp dateTimeExp;
            mutable QRegExp doubleExp;
            mutable QRegExp pointExp;
            mutable QRegExp rectExp;
            mutable QRegExp signedIntegerExp;
            mutable QRegExp sizeExp;
            mutable QRegExp timeExp;
            mutable QRegExp uuidExp;
            mutable QRegExp unsignedIntegerExp;
            mutable QFont currentFont;
            mutable QColor currentColor;
            mutable SwCore::SwFileDescriptor currentFileDescriptor;
            mutable SwCore::SwFileDescriptor oldFileDescriptor;
            mutable SwCore::SwUUID currentUUID;
            mutable QWidget * currentWidgetFont;
            mutable QWidget * currentWidgetColor;
            mutable QWidget * currentWidgetFileDescriptor;
            mutable QFileDialog * _fdialog;
        public slots:
            void onFontClick(bool checked=false);
            void onColorClick(bool checked=false);
            void onFileClick(bool checked=false);
            void onFileLoad(const QString & filename);
		};
	}
}

#endif