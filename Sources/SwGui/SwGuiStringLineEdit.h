/*!
\file SwGuiStringLineEdit.h
\author QBN
*/

#ifndef _STREAMWORK_SWGUI_SWGUISTRINGLINEEDIT_H
#define _STREAMWORK_SWGUI_SWGUISTRINGLINEEDIT_H

/*
* INCLUDES LOCAUX
*/

#include "SwGuiConstantes.h"
/*
* INCLUDES GLOBAUX
*/
#include <QWidget>
#include <QLineEdit>
#include <QValidator>
#include <SwString.h>

namespace StreamWork
{
	namespace SwGui
	{

		/*!
		*	@brief Class Encapsulant une SwString pour la transformer en QValidator
		*/
		class SwStringQValidatorDecorator : public QValidator
		{
			Q_OBJECT
		public:
			SwStringQValidatorDecorator(SwCore::SwString & source,QWidget *parent = 0);
			virtual ~SwStringQValidatorDecorator();

			void setValidator(SwCore::SwString & source);

			virtual State 	validate ( QString & input, int & pos ) const;
		private:
			SwCore::SwString _content;
		};

		/*!
		*	@brief Class permetant d'editer une SwString sour la forme d'une lineEdit
		*/
		class SWGUI_EXPORT SwGuiStringLineEdit: public QLineEdit
		{
			Q_OBJECT
		private:
			//\brief contenu */
			SwCore::SwString _content;
			SwStringQValidatorDecorator _validator;
		public:
			//! \brief Constructeur */
			SwGuiStringLineEdit(SwCore::SwString & source,QWidget *parent = 0);
			//\brief Destructeur */
			virtual ~SwGuiStringLineEdit();

			void SetString(SwCore::SwString & source);
			SwCore::SwString GetString();

		public slots:
			void onEditFinished();
		};

	}
}


#endif 
