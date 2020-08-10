#ifndef _SwGuiFlagComboBoxModel_H
#define _SwGuiFlagComboBoxModel_H

#include <QAbstractListModel >

#include <QMetaEnum>

namespace StreamWork
{
	namespace SwGui
	{
		class SwGuiFlagComboBoxModel : public QAbstractListModel
		{
			Q_OBJECT
		private:
			QMetaEnum _metaEnum;
			int _value;
		public:
			SwGuiFlagComboBoxModel(const QMetaEnum metaEnum, QObject * parent = 0);
			virtual ~SwGuiFlagComboBoxModel();

			int getValue() const;
			void setValue(int val);

			//-----------------------------------------------------------------------------------
			// QAbstractItemModel
			//-----------------------------------------------------------------------------------
			int rowCount(const QModelIndex &parent) const override;

			QVariant data(const QModelIndex &index, int role) const override;
			bool setData(const QModelIndex &index, const QVariant &value, int role) override;
			Qt::ItemFlags flags(const QModelIndex &index) const override;
		};
	}
}

#endif
