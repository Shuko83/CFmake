/**
	 * @file TColor.h
	 * @brief class TColor mappé sur la table TColor
	 * @version 1.0
	 * @date 2011/10/18 - 18:10:2011 - 10:48
	 * @author AAY
	 */


#ifndef TColor_H
#define TColor_H

#include <QDjangoModel.h>

class TColor : public QDjangoModel
{
	Q_OBJECT

	Q_PROPERTY(QString color READ getColor WRITE setColor);
	Q_PROPERTY(QString interfaceName READ getInterfaceName WRITE setInterfaceName);

public:

	QString getColor() const { return _color; }
	void setColor(QString val) { _color = val; }

	QString getInterfaceName() const { return _interfaceName; }
	void setInterfaceName(QString val) { _interfaceName = val; }

private:
	QString _color;
	QString _interfaceName;
	
};

#endif // USER_H
