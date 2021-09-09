#ifndef THROWABLE_H
#define THROWABLE_H

#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include "simulated_object.h"

/**
 * @brief Stan obiektu do rzucania:
 * Oczekujący (Idle) - wyświetlana jest grafika obiektu i oczekuje na kliknięcie aby wyświetlić trajektorię.
 * Celujący (Aiming) - wyświetlana jest trajektoria lotu.
 * Użyty (Used) - obiekt został użyty i jego fizyka jest symulowana.
 */
enum class ThrowableState { Idle, Aiming, Used };

/**
 * @brief Typ obiektu do rzucania:
 * Standardowy (Default) - brak dodatkowych efektów.
 * Odłamkowy (Cluster) - odłamki przy kolizji.
 * Bombardujący (Bombarding) - tworzy dodatkowy obiekt przy wciśnięciu lewego przycisku myszy.
 * Odłamek (ClusterAddon) - tworzony przy kolizji obiektu odłamkowego.
 */
enum class ThrowableType { Default, Cluster, Bombarding, ClusterAddon };


class Throwable : public SimulatedObject
{
	ThrowableState state; ///< Stan obiektu do rzucania
	b2Vec2 linear_velocity; ///< Prędkość liniowa
	float max_velocity = 30.f; ///< Maksymalna prędkość liniowa przy wystrzeleniu

	/**
	 * @brief Ustala jaka jest maksymalna prędkość liniowa przy wyrzuceniu obiektu do rzucania.
	 * 
	 * @param velocity Prędkość która ma zostać ograniczona
	 * @param max_velocity Maksymalna prędkość liniowa przy wystrzeleniu
	 */
	void maximumVelocityLimitation(float& velocity, float max_velocity);

public:
	float thr_start_x = 150; ///< Pozycja startowa (na płaszczyźnie x) obiektu do rzucania
	float thr_start_y = 600; ///< Pozycja startowa (na płaszczyźnie y) obiektu do rzucania
	bool is_moving = true; ///< Określa czy obiekt jest wciąż w ruchu
	ThrowableType throwable_type; ///< Typ obiektu do rzucania

	/**
	 * @brief Tworzy obiekt który jest gotowy do symulacji fizyki.
	 * Określa jego parametry po stworzeniu: pozycje, rozmiar, gęstość, tarcie, sprężystość, tłumienie kątowe.
	 */
	void createObject();

	/**
	 * @brief Zwraca przyszłą lokacje obiektu na płaszczyźnie x.
	 * Bierze pod uwagę startową prędkość po wystrzeleniu i grawitację.
	 * 
	 * @param thr_shifted_x Pozycja kursora myszy na płaszczyźnie x po wystrzeleniu
	 * @param n Ilość skoków w czasie
	 * @return float Przyszłą lokacje obiektu na płaszczyźnie x
	 */
	float getTrajectoryX(float thr_shifted_x, float n);

	/**
	 * @brief Zwraca przyszłą lokacje obiektu na płaszczyźnie y.
	 * Bierze pod uwagę startową prędkość po wystrzeleniu i grawitację.
	 * 
	 * @param thr_shifted_y Pozycja kursora myszy na płaszczyźnie y po wystrzeleniu
	 * @param n Ilość skoków w czasie
	 * @return float Przyszłą lokacje obiektu na płaszczyźnie y
	 */
	float getTrajectoryY(float thr_shifted_y, float n);

	/**
	 * @brief Tworzy obiekt który został rzucony, z prędkością liniową określoną pozycją myszy.
	 * 
	 * @param thr_shifted_x Pozycja myszy na płaszczyźnie x
	 * @param thr_shifted_y Pozycja myszy na płaszczyźnie y
	 */
	void launch(float thr_shifted_x, float thr_shifted_y);

	/**
	 * @brief Ustawia obiekt jako nieruszający się jeżeli suma jego prędkości na płaszczyźnie x i y nie przekracza 0.2f.
	 */
	void checkIfMoving();

	/**
	 * @brief Zwraca stan obiektu do rzucania.
	 * 
	 * @return ThrowableState Stan obiektu (oczekujący, celujący, użyty)
	 */
	ThrowableState getThrowableState();

	/**
	 * @brief Ustawia stan obiektu do rzucania.
	 * 
	 * @param state Stan obiektu (oczekujący, celujący, użyty)
	 */
	void setThrowableState(ThrowableState state);
};


class DefaultThrowable : public Throwable
{
public:
	/**
	 * @brief Konstruktor obiektu do rzucania, standardowego typu (Default).
	 * Ustalony zostaje typ obiektu, jego lokacja, tekstura i rozmiar.
	 * 
	 * @param world Świat w którym ma zostać stworzony obiekt
	 * @param radius Promień obiektu do rzucania
	 */
	DefaultThrowable(b2World* world, float radius);
};


class Cluster : public Throwable
{
public:
	bool collided = false; ///< Określa czy obiekt brał udział w kolizji

	/**
	 * @brief Konstruktor obiektu do rzucania, odłamkowego typu (Cluster).
	 * Ustalony zostaje typ obiektu, jego lokacja, tekstura i rozmiar.
	 * 
	 * @param world Świat w którym ma zostać stworzony obiekt
	 * @param radius Promień obiektu do rzucania
	 * @param is_add Określa czy zostaje stworzony obiekt Cluster czy jego odłamek
	 * @param x Lokacja odłamka na płaszczyźnie x
	 * @param y Lokacja odłamka na płaszczyźnie y
	 */
	Cluster(b2World* world, float radius, bool is_add = false, float x = 150, float y = 600);
};


class Bombarding : public Throwable
{
public:
	int adds_used = 0; ///< Ilość stworzonych dodatkowych obiektów bombardujących

	/**
	 * @brief Konstruktor obiektu do rzucania, bombardującego typu (Bombarding).
	 * Ustalony zostaje typ obiektu, jego lokacja, tekstura i rozmiar.
	 * 
	 * @param world Świat w którym ma zostać stworzony obiekt
	 * @param radius Promień obiektu do rzucania
	 * @param is_add Określa czy zostaje stworzony obiekt Bombarding czy jego odłamek
	 * @param x Lokacja odłamka na płaszczyźnie x
	 * @param y Lokacja odłamka na płaszczyźnie y
	 */
	Bombarding(b2World* world, float radius, bool is_add = false, float x = 150, float y = 600);
};

#endif
