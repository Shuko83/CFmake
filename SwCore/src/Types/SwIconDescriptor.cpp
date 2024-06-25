/**
@file SwIconDescriptor.cpp
@brief IconDescriptor
@author Big
 */

#include "Types/SwIconDescriptor.h"
 
using namespace StreamWork::SwCore;

/** @brief Constructor */
SwIconDescriptor::SwIconDescriptor() {
	//TO DO creation
}
/** @brief Destructor */
SwIconDescriptor::~SwIconDescriptor() {
  //TO DO destruction
}
/*! \brief Operateur d'affectation */
SwIconDescriptor & SwIconDescriptor::operator=(const SwIconDescriptor& source) {
    if (this!=&source) {
        _path=source._path;
        _icon=QIcon(_path);
    }
    return *this;

}
/*! \brief Operateur de comparaison*/
bool SwIconDescriptor::operator==(const SwIconDescriptor& source) const{
    return _path==source._path;
}
/*! \brief de recuperation de la valeur par une string*/
QString SwIconDescriptor::ToString() const{
    return _path;
}
/*! \brief de recuperation de la valeur par un icon*/
QIcon SwIconDescriptor::ToIcon() const {
    return _icon;
}
/*! \brief set du path de l'icon*/
void SwIconDescriptor::setPath(QString path) {
    _path=path;
    _icon=QIcon(_path);
}

QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwIconDescriptor &myObj) {
    out<<myObj.ToString();
    return out;
}
QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwIconDescriptor &myObj) {
    QString tmp;
    in>>tmp;
    myObj.setPath(tmp);
    return in;
}

