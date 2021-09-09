#ifndef SIMULATED_OBJECT_H
#define SIMULATED_OBJECT_H

#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"

/**
 * @brief Typ obiektu: przeszkoda, podłoże, przeciwnik, obiekt do rzucania.
 */
enum class ObjectType { Obstacle, Ground, Enemy, Throwable };

/**
 * @brief Typ przeszkody: drewno, kamień, metal.
 * Typ przeszkody wpływa na: maksymalne HP, gęstość i teksture.
 */
enum class ObstacleType { Wood, Stone, Metal };

/**
 * @brief Typ podłoża: normalne, księżycowe, puste (brak grafiki, tylko kolizje).
 */
enum class GroundType { Normal, Moon, Blank };


class SimulatedObject
{
public:
	float x; ///< Położenie obiektu na płaszczyźnie x
	float y; ///< Położenie obiektu na płaszczyźnie y
	float hp = 1; ///< HP obiektu
	float max_hp = 1; ///< Maksymalne HP obiektu
	float block_size_x = 0; ///< Szerokość bloku
	float block_size_y = 0; ///< Wysokość bloku
	float radius = 0; ///< Promień obiektu
	sf::RectangleShape graphics; ///< Grafika obiektu
	sf::Texture texture; ///< Tekstura obiektu
	sf::Texture texture_damaged; ///< Tekstura poniszczonego obiektu
	b2World* world = nullptr; ///< Świat do którego należy obiekt
	b2Body* physics; ///< Fizyczne właściwości obiektu
	ObjectType object_type; ///< Typ obiektu: przeszkoda, podłoże, przeciwnik, obiekt do rzucania

	/**
	 * @brief Metoda czysto wirtualna.
	 */
	void virtual createObject() = 0;

	/**
	 * @brief Ustawia świat w którym znajduje się obiekt i jego położenie.
	 * 
	 * @param world Świat w którym znajduje się obiekt
	 * @param x Położenie obiektu na płaszczyźnie x
	 * @param y Położenie obiektu na płaszczyźnie y
	 */
	void initializePosition(b2World* world, float x, float y);

	/**
	 * @brief Ustawia odpowiednie położenie i rotację grafiki obiektu, w oparciu o jego interakcje z innymi obiektami w świecie.
	 */
	void updatePosition();

	/**
	 * @brief Usuwa obiekt ze świata.
	 */
	void deleteObject();

	/**
	 * @brief Ustawia teksture obiektu na wersje poniszczoną.
	 */
	void setAsDamaged();
};


class Obstacle : public SimulatedObject
{
	ObstacleType obstacle_type; ///< Typ przeszkody: drewno, kamień, metal

	/**
	 * @brief Tworzy przeszkode która jest gotowa do symulacji fizyki.
	 * Określa jej parametry po stworzeniu: pozycje, rozmiar, gęstość i tarcie.
	 */
	void createObject();

	/**
	 * @brief Ustawia typ przeszkody.
	 * Wpływa na: maksymalne HP, gęstość i teksture.
	 * 
	 * @param obstacle_type Typ przeszkody: drewno, kamień, metal
	 */
	void setType(ObstacleType obstacle_type);

public:
	/**
	 * @brief Konstruktor przeszkody.
	 * Ustalony zostaje typ przeszkody, jej lokacja, tekstura i rozmiar.
	 * 
	 * @param world Świat w którym ma zostać stworzona przeszkoda
	 * @param block_size_x Szerokość przeszkody
	 * @param block_size_y Wysokość przeszkody
	 * @param obstacle_type Typ przeszkody: drewno, kamień, metal
	 * @param x Położenie przeszkody na płaszczyźnie x
	 * @param y Położenie przeszkody na płaszczyźnie y
	 */
	Obstacle(b2World* world, float block_size_x, float block_size_y,
		ObstacleType obstacle_type, float x, float y);
};


class Ground : public SimulatedObject
{
	/**
	 * @brief Tworzy podłoże które jest gotowe do symulacji fizyki.
	 * Określa jego parametry po stworzeniu: pozycje, rozmiar i gęstość.
	 */
	void createObject();

public:
	/**
	 * @brief Konstruktor podłoża w oparciu o teksture.
	 * Ustalony zostaje typ podłoża, jej lokacja, tekstura i rozmiar.
	 * 
	 * @param world Świat w którym ma zostać stworzony blok podłoża
	 * @param block_size_x Szerokość podłoża
	 * @param block_size_y Wysokość podłoża
	 * @param ground_type Typ podłoża
	 * @param x Położenie podłoża na płaszczyźnie x
	 * @param y Położenie podłoża na płaszczyźnie y
	 */
	Ground(b2World* world, float block_size_x, float block_size_y,
		GroundType ground_type, float x, float y);

	/**
	 * @brief Konstruktor podłoża w oparciu o kolor.
	 * Ustalona zostaje lokacja podłoża, kolor i rozmiar.
	 * 
	 * @param world Świat w którym ma zostać stworzony blok podłoża
	 * @param block_size_x Szerokość podłoża
	 * @param block_size_y Wysokość podłoża
	 * @param color Kolor podłoża
	 * @param x Położenie podłoża na płaszczyźnie x
	 * @param y Położenie podłoża na płaszczyźnie y
	 */
	Ground(b2World* world, float block_size_x, float block_size_y,
		sf::Color color, float x, float y);
};


class Enemy : public SimulatedObject
{
	/**
	 * @brief Tworzy przeciwnika który jest gotowy do symulacji fizyki.
	 * Określa jego parametry po stworzeniu: pozycje, rozmiar, gęstość i tarcie.
	 */
	void createObject();

public:
	/**
	 * @brief Konstruktor przeciwnika.
	 * Ustalona zostaje lokacja przeciwnika, tekstura, rozmiar i maksymalne HP.
	 * 
	 * @param world Świat w którym ma zostać stworzony przeciwnik
	 * @param block_size_x Szerokość przeciwnika
	 * @param block_size_y Wysokość przeciwnika
	 * @param x Położenie przeciwnika na płaszczyźnie x
	 * @param y Położenie przeciwnika na płaszczyźnie y
	 */
	Enemy(b2World* world, float block_size_x, float block_size_y, float x, float y);
};
#endif