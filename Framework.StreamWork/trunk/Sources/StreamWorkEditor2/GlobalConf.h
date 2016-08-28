#pragma once

#include <QObject>

class GlobalConf : public QObject
{
	Q_OBJECT;
public:
	virtual ~GlobalConf();

	static GlobalConf * instance();

	bool isSplineLink() const;
	void setSplineLink(bool val);
protected:
	GlobalConf();

	void load();
	void save();
private:
	bool _isSplineLink;
};

