/**
@file SwIpV4Address.cpp
@brief Adresse IPV4
@author F.Bighelli
 */

#include <QStringList>
#include "SwIpV4Address.h"
 
using namespace StreamWork::SwCore;

/** @brief Constructor */
SwIpV4Address::SwIpV4Address() {
	_b0=127;
    _b1=0;
    _b2=0;
    _b3=1;
}
/** @brief Constructor */
SwIpV4Address::SwIpV4Address(unsigned char b0,unsigned char b1,unsigned char b2,unsigned char b3){
	_b0=b0;
    _b1=b1;
    _b2=b2;
    _b3=b3;
}
/*! \brief Constructeur de copie */
SwIpV4Address::SwIpV4Address(const SwIpV4Address & source){
	_b0=source._b0;
    _b1=source._b1;
    _b2=source._b2;
    _b3=source._b3;
}
/*! \brief Destructeur */
SwIpV4Address::~SwIpV4Address(){
	//A Completer
}
/** @brief Acces au nom */
void SwIpV4Address::getAddress(unsigned char &b0,unsigned char &b1,unsigned char &b2,unsigned char &b3) const{
	b0=_b0;
    b1=_b1;
    b2=_b2;
    b3=_b3;
}
void SwIpV4Address::setAddress(unsigned char b0,unsigned char b1,unsigned char b2,unsigned char b3){
	_b0=b0;
    _b1=b1;
    _b2=b2;
    _b3=b3;
}

/*! \brief Operateur d'affectation */
SwIpV4Address & SwIpV4Address::operator=(const SwIpV4Address& source){
    if (this!=&source) {
	    _b0=source._b0;
        _b1=source._b1;
        _b2=source._b2;
        _b3=source._b3;
    }
    return *this;
}
/*! \brief Operateur de comparaison*/
bool SwIpV4Address::operator==(const SwIpV4Address& source) const{
    if (this!=&source) {
	    return _b0==source._b0 &&
        _b1==source._b1 &&
        _b2==source._b2 &&
        _b3==source._b3;
    } else
        return true;
}
/*! \brief ecriture  de la valeur par une string*/
void SwIpV4Address::FromString(const QString & value) {
    QStringList v=value.split(".");
    if (v.size()!=4)
        return;
    _b0=(unsigned char)v.at(0).toInt();
    _b1=(unsigned char)v.at(1).toInt();
    _b2=(unsigned char)v.at(2).toInt();
    _b3=(unsigned char)v.at(3).toInt();
}
/*! \brief de recuperation de la valeur par une string*/
QString SwIpV4Address::ToString() const{
	return QString("%1.%2.%3.%4").arg(_b0).arg(_b1).arg(_b2).arg(_b3);
}

QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwIpV4Address &myObj) {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    myObj.getAddress(b0,b1,b2,b3);
    out<<b0;
    out<<b1;
    out<<b2;
    out<<b3;
    return out;
}
QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwIpV4Address &myObj) {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    in>>b0;
    in>>b1;
    in>>b2;
    in>>b3;
    myObj.setAddress(b0,b1,b2,b3);
    return in;
}
