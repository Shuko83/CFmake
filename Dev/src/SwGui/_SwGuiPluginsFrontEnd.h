/*!
\file _SwGuiPluginsFrontEnd.h
\date 12/04/2006
\brief Ihm du wizard de generation de plugin
\author  Big
\version 1.0
 */

#ifndef __SwGuiPluginsFrontEnd_H
#define __SwGuiPluginsFrontEnd_H
/*
  * INCLUDES GLOBAUX
  */

#include <QDialog>

/*
  * INCLUDES LOCAUX
  */


/*!
	\class _SwGuiPluginsFrontEnd 
	\brief Ihm du wizard de generation de plugin
*/
class _SwGuiPluginsFrontEnd : public QDialog
{
    Q_OBJECT
public:
    /*! \brief Constructeur */
    _SwGuiPluginsFrontEnd(QWidget *parent);
    /*! \brief Destructeur */
    ~_SwGuiPluginsFrontEnd();
private slots:
    /*! \brief export */
    void Export();

};

#endif
