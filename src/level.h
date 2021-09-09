#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include "simulated_object.h"
#include "throwable.h"
#include "contact_listener.h"

class Level
{
	int screen_width; ///< Szerokość ekranu
	int screen_height; ///< Wysokość ekranu
	const float ground_size_x = 35.f; ///< Standardowa szerokość bloku
	const float ground_size_y = 35.f; ///< Standardowa wysokość bloku
	int score = 0; ///< Punkty zdobyte na poziomie
	int throwable_number = 1; ///< Stopień ulepszonych rzutów, 1 - wszystkie Default, 2 - jeden Cluster, 3 - jeden Cluster i Bombarding
	int level_number = 0; ///< Liczba całkowita jednoznacznie identyfikująca Level
	GroundType level_type = GroundType::Normal; ///< Typ Levelu określający: podłożę, tło i grawitację
	sf::RectangleShape background; ///< Grafika tła
	sf::Texture background_texture; ///< Tekstura tła
	b2Vec2 gravity; ///< Siła grawitacji opisana przez wektor
	b2World  *world; ///< Świat w którym znajdują się obiekty na których przeprowadzane są obliczenia ich: położenia, przyspieszenia, prędkości, rotacji i kolizji
	ContactListener contact_listener_instance; ///< Obiekt który przypisany do świata wykrywa wszystkie kolizje obiektów w tym świecie
	std::vector<SimulatedObject*> objects; ///< Kontener wskaźników do wszyskich stworzonych obiektów: przeszkód, przeciwników, podłoża, rzutów
	Throwable* active_throwable = nullptr; ///< Wskaźnik do aktywnego obiektu do rzucania
	Throwable* throwables[3]; ///< Tablica wszystkich obiektów do rzucania dostępnych dla Levelu
	Cluster* thr_clus = nullptr; ///< Wskaźnik obiektu do rzucania typu Cluster
	Bombarding* thr_bom = nullptr; ///< Wskaźnik obiektu do rzucania typu Bombarding
	
	/**
	 * @brief Tworzy podłoże na samym dole Levelu i na jego całej szerokości.
	 * 
	 * @param ground_type Typ podłoża do stworzenia
	 */
	void createWholeGround(GroundType ground_type);

	/**
	 * @brief Tworzy 3 przeźroczyste, cienkie platformy na granicach ekranu które nie pozwalają obiektom wydastać się poza ekran.
	 * 
	 */
	void createBoundaries();

	/**
	 * @brief Tworzy 3 obiekty gotowe do rzutu gdy zostaną aktywowane.
	 * 
	 * @param throwable_amount Określa stopień ulepszonych rzutów, 1 - wszystkie Default, 2 - jeden Cluster, 3 - jeden Cluster i Bombarding
	 * @param radius Rozmiar obiektu do rzucania w pikselach - rozmiar standardowy to 40.0
	 */
	void createThrowables(int throwable_amount, float radius = 40.f);
	
	/**
	 * @brief Niszczy wszystkie stworzone obiekty i czyści wektor objects.
	 * 
	 */
	void destroyAllObjects();
	
public:
	/**
	 * @brief Konstruktor obiektu typu Level, określa grawitację i tło.
	 * 
	 * @param level_number Liczba całkowita jednoznacznie identyfikująca Level
	 * @param throwable_amount Określa stopień ulepszonych rzutów, 1 - wszystkie Default, 2 - jeden Cluster, 3 - jeden Cluster i Bombarding
	 * @param level_type Typ Levelu określający: podłożę, tło i grawitację
	 * @param screen_width Szerokość ekranu
	 * @param screen_height Wysokość ekranu
	 */
	Level(int level_number, int throwable_number, GroundType level_type,
		int screen_width = 1400, int screen_height = 800);

	/**
	 * @brief Destruktor obiektu typu Level który jednocześnie niszczy i zwalnia pamięć wszystkich stworzonych obiektów.
	 * 
	 */
	~Level();

	/**
	 * @brief Tworzy pojedynczy blok podłoża i umieszcza pointer stworzonego obiektu w wektorze objects. Rozmiar bloku podłoża jest stały.
	 * 
	 * @param ground_type Typ podłoża: Normal, Moon
	 * @param x Lokacja stworzenia obiektu na koordynacie x
	 * @param y Lokacja stworzenia obiektu na koordynacie y
	 */
	void createGround(GroundType ground_type, float x, float y);
	
	/**
	 * @brief Tworzy pojedynczy blok przeszkody i umieszcza pointer stworzonego obiektu w wektorze objects.
	 * 
	 * @param obstacle_type Typ przeszkody: Wood, Stone, Metal
	 * @param x Lokacja stworzenia obiektu na koordynacie x
	 * @param y Lokacja stworzenia obiektu na koordynacie y
	 * @param box_size_x Szerokość przeszkody w pikselach
	 * @param box_size_y Wysokość przeszkody w pikselach
	 */
	void createObstacle(ObstacleType obstacle_type, float x, float y,
		float box_size_x = 35.f, float box_size_y = 35.f);

	/**
	 * @brief Tworzy pojedynczy blok przeciwnika i umieszcza pointer stworzonego obiektu w wektorze objects.
	 * 
	 * @param x Lokacja stworzenia obiektu na koordynacie x
	 * @param y Lokacja stworzenia obiektu na koordynacie y
	 * @param box_size_x Szerokość przeciwnika w pikselach
	 * @param box_size_y Wysokość przeciwnika w pikselach
	 */
	void createEnemy(float x, float y, float box_size_x = 35.f, float box_size_y = 35.f);

	/**
	 * @brief Tworzy całe podłoże, granice i obiekty do rzutu poziomu.
	 * Pozwala stworzyć obiekty - w dowolnych rozmiarach i dowolnych pozycjach po edycji definicji funkcji.
	 */
	void makeLevel();

	/**
	 * @brief Wprowadza aktywny obiekt do rzutu do kontenera wszystkich obiektów.
	 * Czyni go to obiektem do fizycznej symulacji który może wchodzić w interakcje z innymi obiektami.
	 * Oznacza rzucony obiekt jako użyty.
	 */
	void throwableUsed();

	/**
	 * @brief Tworzy maksymalnie 3 odłamki od obiektu typu Bombarding.
	 * Odłamki są wystrzeliwane z prędkością liniową skierowaną w dół.
	 */
	void createBombardingAdds();

	/**
	 * @brief Tworzy 8 odłamków po kolizji obiektu typu Cluster.
	 * Stworzone obiekty są wstrzeliwane z prędkością liniową w 4 różnych kierunkach.
	 */
	void createClusterAdds();

	/**
	 * @brief Sprawdza czy obiekty do rzucania skończyły się poruszać.
	 * Obiekt który przestał się poruszać zostaje oflagowany jako nieporuszający się.
	 */
	void checkThrowableMovement();

	/**
	 * @brief Niszczy obiekt którego HP jest mniejsze lub równe 0 przyznając punkty równe jego maksymalnego HP * 10.
	 * Jeżeli HP spadnie poniżej połowy maksymalnego HP to obiekt zostaje oznaczony jako poniszczony i zmienia swoją teksture.
	 */
	void destroyFlaggedObjects();

	/**
	 * @brief Iteruje poprzez wszystkie obiekty ze złożonością obliczeniową O(n) i sprawdza czy istnieje obiekt typu Enemy.
	 * 
	 * @return true Nie znaleziono obiektu typu Enemy
	 * @return false Znaleziono obiekt typu Enemy
	 */
	bool isLevelWon();

	/**
	 * @brief Sprawdza czy obiekty do rzucania skończyły się poruszać.
	 * 
	 * @return true Każdy z obiektów do rzucania skończył się poruszać
	 * @return false Istnieje przynajmniej 1 obiekt do rzucania który jeszcze się porusza
	 */
	bool isLevelLost();

	/**
	 * @brief Przyznaje punkty za każdy z niezużytych obiektów do rzucania.
	 * 3000 punktów za Default, 5000 punktów za Cluster i Bombarding
	 */
	void addPointsForUnusedThrows();

	/**
	 * @brief Zwraca wskaźnik do świata.
	 * 
	 * @return b2World* Wskaźnik do świata w którym znajdują się obiekty na których przeprowadzane są obliczenia ich: położenia, przyspieszenia, prędkości, rotacji i kolizji
	 */
	b2World* getWorldPointer();

	/**
	 * @brief Zwraca wskaźnik do obiektów.
	 * 
	 * @return std::vector<SimulatedObject*>* Wskaźnik na kontener wskaźników do wszyskich stworzonych obiektów: przeszkód, przeciwników, podłoża, rzutów
	 */
	std::vector<SimulatedObject*>* getObjects();

	/**
	 * @brief Zwraca aktywny obiekt do rzucania.
	 * 
	 * @return Throwable* Wskaźnik do aktywnego obiektu do rzucania
	 */
	Throwable* getActiveThrowable();

	/**
	 * @brief Zwraca grafike tła.
	 * 
	 * @return sf::RectangleShape Grafika tła
	 */
	sf::RectangleShape getBackgroud();

	/**
	 * @brief Zwraca ilość punktów.
	 * 
	 * @return int Punkty zdobyte na poziomie
	 */
	int getScore();

	/**
	 * @brief Zwraca numer poziomu.
	 * 
	 * @return int Liczba całkowita jednoznacznie identyfikująca Level
	 */
	int getLevelNumber();

	/**
	 * @brief Zwraca typ poziomu.
	 * 
	 * @return GroundType Typ Levelu określający: podłożę, tło i grawitację
	 */
	GroundType getLevelType();

	/**
	 * @brief Zwraca wskaźnik do obiektu do rzucania na podstawie jego numeru w tablicy.
	 * Dla stopnia ulepszenia rzutów 1 - wszystkie są Default.
	 * Dla stopnia ulepszenia rzutów 2 - rzut numer 2 to Cluster, reszta Default.
	 * Dla stopnia ulepszenia rzutów 3 - rzut numer 2 to Bombarding, rzut numer 1 to Cluster, rzut numer 0 to Default.
	 * 
	 * @param throwable_number Numer obiektu do rzucania od 0 do 2
	 * @return Throwable* Wskaźnik obiektu do rzucania
	 */
	Throwable* getThrowableByNumber(int throwable_number);

	/**
	 * @brief Ustawia obiektu do rzucania jako aktywny na podstawie jego numeru w tablicy.
	 * Dla stopnia ulepszenia rzutów 1 - wszystkie są Default.
	 * Dla stopnia ulepszenia rzutów 2 - rzut numer 2 to Cluster, reszta Default.
	 * Dla stopnia ulepszenia rzutów 3 - rzut numer 2 to Bombarding, rzut numer 1 to Cluster, rzut numer 0 to Default.
	 * 
	 * @param throwable_number Numer obiektu do rzucania od 0 do 2
	 */
	void setActiveThrowable(int throwable_number);
};

#endif
