#ifndef _SwGuiFlagComboBox_H
#define _SwGuiFlagComboBox_H

#include "SwGuiConstantes.h"

#include <QComboBox>
#include <QMetaEnum>

namespace StreamWork
{
	namespace SwGui
	{

		class SwGuiFlagComboBoxModel;

		class SWGUI_EXPORT SwGuiFlagComboBox : public QComboBox
		{
		    Q_OBJECT
        private:
			SwGuiFlagComboBoxModel* _model;
			QMetaEnum _metaEnum;
		public:
			SwGuiFlagComboBox(const QMetaEnum & metaEnum, QWidget *parent = 0);
		    virtual ~SwGuiFlagComboBox();

            void setValue(int value);
			int getValue() const;


			void paintEvent(QPaintEvent *) override;

		private:
			void onActivated(int index);
		};
	}
}

#endif 
