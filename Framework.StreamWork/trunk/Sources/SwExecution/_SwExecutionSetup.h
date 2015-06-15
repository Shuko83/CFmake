/*!
\file _SwExecutionSetup.h
\date 12/04/2006
\brief Ihm de configuration de l'execution
\author  Big
\version 1.0
 */

#ifndef __SwExecutionSetup_H
#define __SwExecutionSetup_H
/*
  * INCLUDES GLOBAUX
  */

#include <QDialog>
#include <QList>
/*
  * INCLUDES LOCAUX
  */

#include <SwComponent_Class.h>
#include "ISwExecutable_Service.h"
#include "_SwExecution_Service.h"
#include "ui__SwExecutionSetup.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/*!
	\class _SwExecutionSetup 
	\brief Ihm de configuration de l'execution
*/
class _SwExecutionSetup : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief	: Constructeur
     */
    _SwExecutionSetup(QWidget *parent,SwComponent_Class * root_component,QList<QString> * current_list,QList<_SwExecutionMode> * current_mode_list);

	/**
	* @brief	: Destructeur
	*/
    ~_SwExecutionSetup();

    /**
     * @brief	: Permet d'acceder a la nouvelle liste
     * 
     * @return	: const QList<QString> * - Pointeur sur la liste
     */
    const QList<QString> * GetEditedList();

    /**
     * @brief	: Permet d'acceder a la nouvelle liste mode
     * 
     * @return	: const QList<_SwExecutionMode> *
     */
    const QList<_SwExecutionMode> * GetEditedListMode();

protected:
	/*! \brief composant racine */
	SwComponent_Class * _root_component;

	/* List des interfaces d'execution */
	QList<QString> _exe_paths;

	/* List des modes d'executions*/
	QList<_SwExecutionMode> _exe_modes;


	Ui::_SwExecutionSetupWidget* ui;
};

#endif
