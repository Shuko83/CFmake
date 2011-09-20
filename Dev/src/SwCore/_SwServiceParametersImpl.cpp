/**
@file _SwServiceParametersImpl.cpp
@brief Parameters service implementation
@author Big
 */

#include "_SwServiceParametersImpl.h"

using namespace StreamWork::SwCore;


/** @brief Constructor */
_SwServiceParametersImpl::_SwServiceParametersImpl () 
{
	_parameters.clear ();
}


/** @brief Destructor */
_SwServiceParametersImpl::~_SwServiceParametersImpl () 
{
  //TO DO destruction
}


/*! \brief Permet d'enregistrer un parametre avec sa valeur */
void _SwServiceParametersImpl::registerParameter (QString parameter, QString value)
{
    _parameters[parameter] = value;
}


/*! \brief Permet de savoir si un parametre est enregistre et récupérer sa valeur le cas echeant */
bool _SwServiceParametersImpl::getParameter (QString parameter, QString & value)
{
    bool result = false;
    value = "";

    if (_parameters.contains (parameter))
    {
        value = _parameters[parameter];
        result = true;
    }

    return result;
}
