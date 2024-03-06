/*!
	\file _SwPerfChrono.h
	\class _SwPerfChrono
	\author AAY
	\brief Cette classe permet de mesurer le temps necessaire pour effectuer une portion de code
*/


/*Include System*/
#ifdef Q_OS_WIN
#include <windows.h>
#endif

#define Ms 1000
#define Us 1000000
#define Ns 1000000000

class _SwPerfChrono
{
	/*Frequence de l'horloge*/
	__int64 freq;
	/*Temps initial*/
	__int64 t0;

public:

	/*! \brief constructeur qui va récupérer la fréquence de l'horloge */
	/*! On passe en paramètre à QueryPerformanceFrequency une structure LARGE_INTEGER qui est composée d'un __int64 */
	/*  Cette structure(içi freq) va contenir la frequence de l'horloge */
	_SwPerfChrono() 
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	}

	/*! \brief Démare le timer */
	/*! On fait pareil qu'au dessus mais cette fois t0 va contenir le temps au debut du chronometrage */
	void start()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&t0);
	}

	/*! \brief Retourne le temps écoulé depuis le start en Ms */
	/*! Pour avoir le temps écoulé, on retranche le temps au moment de la fonction du temps au debut du chrono,*/
	/*!	Puis on le multiplie par la précision (1000) et on le divise par la frequence.*/
	/*! \return le temps écoulé en Ms*/
	DWORD getDiffMs()
	{
		/*le temps au moment de l'execution de la fonction*/
		__int64 t1;
		/*On assigne à t1 le temp au moment de l'execution de la fonction*/
		QueryPerformanceCounter((LARGE_INTEGER*)&t1);
		/*La forumle qui permet de calculer le temps écoulé*/
		return (DWORD)(((t1 - t0) * Ms) / freq); 

	}

	/*! \brief Retourne le temps écoulé depuis le start en µs */
	/*! Pour avoir le temps écoulé, on retranche le temps au moment de la fonction du temps au debut du chrono,*/
	/*!	Puis on le multiplie par la précision (1000000) et on le divise par la frequence.*/
	/*! \return le temps écoulé en µs*/
	DWORD getDiffUs() //Us pour µs
	{
		/*le temps au moment de l'execution de la fonction*/
		__int64 t1;
		/*On assigne à t1 le temp au moment de l'execution de la fonction*/
		QueryPerformanceCounter((LARGE_INTEGER*)&t1);
		/*La forumle qui permet de calculer le temps écoulé*/
		return (DWORD)(((t1 - t0) * Us) / freq); 
	}

	/*! \brief Retourne le temps écoulé depuis le start en Ns */
	/*! Pour avoir le temps écoulé, on retranche le temps au moment de la fonction du temps au debut du chrono,*/
	/*!	Puis on le multiplie par la précision (1000000000) et on le divise par la frequence.*/
	/*! \return le temps écoulé en Ns*/
	DWORD getDiffNs()
	{
		/*le temps au moment de l'execution de la fonction*/
		__int64 t1;
		/*On assigne à t1 le temp au moment de l'execution de la fonction*/
		QueryPerformanceCounter((LARGE_INTEGER*)&t1);
		/*La forumle qui permet de calculer le temps écoulé*/
		return (DWORD)(((t1 - t0) * Ns) / freq);
	}


	/*! \brief Retourne le temps écoulé depuis le start en l'unité voulue */
	/*! Pour avoir le temps écoulé, on retranche le temps au moment de la fonction du temps au debut du chrono,*/
	/*!	Puis on le multiplie par la précision (Unit) et on le divise par la frequence.*/
	/*! \param unit : int definissant la précision (1000 -> Ms , 1000000 -> µs, 1000000000 -> Ns)
	/*! \return le temps écoulé en unit*/
	DWORD getDiff(UINT unit)
	{
		/*le temps au moment de l'execution de la fonction*/
		__int64 t1;
		/*On assigne à t1 le temp au moment de l'execution de la fonction*/
		QueryPerformanceCounter((LARGE_INTEGER*)&t1);
		/*La forumle qui permet de calculer le temps écoulé*/
		return (DWORD)(((t1 - t0) * unit) / freq);
	}

	/*! \brief Retourne la frequence de l'horloge */
	/*! \return la frequence de l'horloge en Hertz ?*/
	DWORD getFreq()
	{
		return (DWORD)freq;
	}
}; 
