/**
@file _SwServiceParametersImpl.cpp
@brief Parameters service implementation
@author Big
 */

#include "_SwServiceParametersImpl.h"
#include <QCoreApplication>

using namespace StreamWork::SwCore;


/** @brief Constructor */
_SwServiceParametersImpl::_SwServiceParametersImpl () 
{
	_parameters.clear ();

	QStringList args=QCoreApplication::instance()->arguments();      
	int nbArgs = args.count();		

	for(int i=1; i<nbArgs; i++) {
		// Parameters
		if (args[i]=="-P" && i+1<nbArgs ) 
		{
			QStringList tmpList = args[i+1].split("=");
			if(tmpList.count() <= 2)
			{
				if( tmpList.count() == 2)
				{
					registerParameter (tmpList.at(0), tmpList.at(1));
				}
				else
				{
					registerParameter (tmpList.at(0), QString());
				}
			}
		}
	}
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
