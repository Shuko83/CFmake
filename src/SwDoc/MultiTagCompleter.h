#ifndef MULTITAGCOMPLETER_H
#define MULTITAGCOMPLETER_H

#include <QCompleter>
#include <QStringList>
#include "SwDocConstantes.h"


class BUILD_SWDOC MultiTagCompleter : public QCompleter
{
	Q_OBJECT

public:
	MultiTagCompleter(const QStringList& items = QStringList(), QObject *parent=NULL);
	~MultiTagCompleter();

public:
	QString pathFromIndex( const QModelIndex& index ) const;
	QStringList splitPath( const QString& path ) const;
	
};

#endif // MULTITAGCOMPLETER_H
