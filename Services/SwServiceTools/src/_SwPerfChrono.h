/*!
	\file _SwPerfChrono.h
	\class _SwPerfChrono
	\author AAY
	\brief Cette classe permet de mesurer le temps necessaire pour effectuer une portion de code
*/


/*Include System*/
#include <chrono>

class _SwPerfChrono
{

public:

	/*! \brief constructeur qui va récupérer la fréquence de l'horloge */
	/*! On passe en paramètre à QueryPerformanceFrequency une structure LARGE_INTEGER qui est composée d'un __int64 */
	/*  Cette structure(içi freq) va contenir la frequence de l'horloge */
	_SwPerfChrono() 
	{
	}

private:

	/*Temps initial*/
	std::chrono::high_resolution_clock::time_point t0;

	/*! \brief Retourne le temps écoulé depuis le start en l'unité voulue */
	/*! Pour avoir le temps écoulé, on retranche le temps au moment de la fonction du temps au debut du chrono,*/
	/*!	Puis on le multiplie par la précision (Unit) et on le divise par la frequence.*/
	/*! \param unit : int definissant la précision (1000 -> Ms , 1000000 -> µs, 1000000000 -> Ns)
	/*! \return le temps écoulé en unit*/
	std::chrono::duration<double> getDiff()
	{
		/*le temps au moment de l'execution de la fonction*/
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

		return (t1 - t0);
	}


public:

	/*! \brief Démare le timer */
	/*! On fait pareil qu'au dessus mais cette fois t0 va contenir le temps au debut du chronometrage */
	void start()
	{
		t0 = std::chrono::high_resolution_clock::now();
	}

	std::chrono::seconds getDiffSec()
	{
		return std::chrono::duration_cast<std::chrono::seconds>(getDiff());
	}


	/*! \brief Retourne le temps écoulé depuis le start en Ms */
	/*! Pour avoir le temps écoulé, on retranche le temps au moment de la fonction du temps au debut du chrono,*/
	/*!	Puis on le multiplie par la précision (1000) et on le divise par la frequence.*/
	/*! \return le temps écoulé en Ms*/
	std::chrono::milliseconds getDiffMs()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(getDiff());
	}

	/*! \brief Retourne le temps écoulé depuis le start en µs */
	/*! Pour avoir le temps écoulé, on retranche le temps au moment de la fonction du temps au debut du chrono,*/
	/*!	Puis on le multiplie par la précision (1000000) et on le divise par la frequence.*/
	/*! \return le temps écoulé en µs*/
	std::chrono::microseconds getDiffUs() //Us pour µs
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(getDiff());
	}

	/*! \brief Retourne le temps écoulé depuis le start en Ns */
	/*! Pour avoir le temps écoulé, on retranche le temps au moment de la fonction du temps au debut du chrono,*/
	/*!	Puis on le multiplie par la précision (1000000000) et on le divise par la frequence.*/
	/*! \return le temps écoulé en Ns*/
	std::chrono::nanoseconds getDiffNs()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(getDiff());
	}
}; 
