#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include "box2d/box2d.h"
#include "simulated_object.h"
#include "throwable.h"

class ContactListener : public b2ContactListener
{
	SimulatedObject* object_a = nullptr; ///< Pierwszy obiekt kolizji
	SimulatedObject* object_b = nullptr; ///< Drugi obiekt kolizji
	float damage = 0; ///< Ilość zadanych obrażeń dla obiektu

	/**
	 * @brief Metoda wywoływana w momencie wykrycia kolizji.
	 * Oblicza i przyznaje obrażenia do obu obiektów uległych kolizji.
	 * 
	 * @param contact Klasa odpowiadająca za zarządzanie kolizją pomiędzy dwoma obiektami
	 */
	void BeginContact(b2Contact* contact);

	/**
	 * @brief Oblicza obrażanie zadane obiektowi będące sumą prędkości liniowej obu ciał na płaszczyźnie x i y.
	 * Jeżeli obiektem kolizji jest Throwable to do obliczeń brana jest pod uwagę również masa, czyli pęd ciała.
	 * 
	 * @param object_a Pierwszy obiekt kolizji
	 * @param object_b Drugi obiekt kolizji
	 */
	void calculateContactDamage(SimulatedObject* object_a, SimulatedObject* object_b);

	/**
	 * @brief Odejmuje HP przeszkodzie lub przeciwnikowi na podstawie wcześniej obliczonych obrażeń.
	 * 
	 * @param object Obiekt otrzymujący obrażenia
	 */
	void applyDamage(SimulatedObject* object);

	/**
	 * @brief Sprawdza czy obiekt który uległ kolizji jest typu Cluster
	 * 
	 * @param object Obiekt ulegający kolizji typu Throwable
	 * @return true Obiekt ulegający kolizji jest typu Cluster
	 * @return false Obiekt ulegający kolizji nie jest typu Cluster
	 */
	bool checkIfClusterContact(Throwable* object);

public:
	bool cluster_contact = false; ///< Czy obiekt do rzucania uległ już kolizji
};

#endif