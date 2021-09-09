#ifndef SCREEN_H
#define SCREEN_H

#include "SFML/Graphics.hpp"
#include "level.h"
#include "button.h"

/**
 * @brief Stan gry: poziom, menu map, wygrana, przegrana.
 */
enum class GameState { Level, MapMenu, Win, Lose };

/**
 * @brief Typ zapisu: plik tekstowy, plik binarny.
 */
enum class SaveType { Text, Binary };


class Screen
{
	int screen_width = 1400; ///< Szerokość ekranu
	int screen_height = 800; ///< Wysokość ekranu
	sf::RenderWindow window; ///< Okno w którym reprezentowana jest cała funkcjonalność programu
	sf::Event event; ///< Wydarzenie mające wpływ na przebieg programu, może to być kliknięcie klawisza klawiatury lub myszy
	sf::Vector2f mouse_position; ///< Pozycja kursora myszy
	GameState game_state = GameState::MapMenu; ///< Stan gry
	GameState previous_game_state; ///< Poprzedni stan gry
	Button *map_button, *level_icon[5], *win_restart, *lose_restart, *next; ///< Przycisk odpowiadający za przejście do innego stanu gry
	sf::RectangleShape map_background, map_menu_window, win_lose_window; ///< Dodatkowe, wyskakujące okno
	sf::Text score_text, win_lose_score_text, win_text, lose_text, highscore_text[2]; ///< Tekst wyświetlany na ekranie
	sf::RectangleShape lives_img[3]; ///< Grafika typu obiektu do rzucania do wyboru
	sf::Text lives_text[3]; ///< Liczba oznaczająca klawisz wyboru danego obiektu do rzucania
	sf::CircleShape trajectory_point[18]; ///< Punkty reprezentujące trajektorię lotu
	sf::Font font; ///< Czcionka
	sf::Texture map_background_texture; ///< Tło menu map
	Level* active_level = nullptr; ///< Aktywny poziom
	std::vector<SimulatedObject*>* active_level_objects = nullptr; ///< Wskaźnik do kontenera obiektów aktywnego poziomu
	char level_design[5][22][41]; ///< Wczytany z pliku tekstowego lub binarnego design poziomów - pierwszy wymiar oznacza numer poziomu, drugi wysokość a trzeci szerokość
	int scores[5]; ///< Punkty dla każdego z poziomów

	/**
	 * @brief Tworzy ekran map z potrzebnymi mu oknami, przyciskami i ich rozmieszczeniem.
	 */
	void createMapMenu();

	/**
	 * @brief Tworzy ekran wygranej i przegranej z potrzebnymi mu oknami, przyciskami, tekstami i ich rozmieszczeniem.
	 */
	void createWinLoseWindow();

	/**
	 * @brief Zapisuje wczytany z pliku layout poziomów w pliku binarnym.
	 */
	void saveLevelLayout();

	/**
	 * @brief Odblokowuje poziom jeżeli poziom poprzedni został pokonany.
	 */
	void unlockNextLevels();

	/**
	 * @brief Ustawia aktywny poziom.
	 * 
	 * @param activeLevel Poziom który ma stać się aktywnym
	 */
	void setActiveLevel(Level* activeLevel);

	/**
	 * @brief Wczytuje wybrany poziom i usuwa z pamięci stary. Tworzy wszystkie obiekty nowego poziomu - interpretowane z pliku i stworzone ręcznie w funkcji makeLevel.
	 * Ustawia wczytany poziom jako aktywny. Ustawia grafiki wyboru obiektów do rzucania z tego poziomu.
	 * 
	 * @param level_number Numer wczytywanego poziomu
	 */
	void loadChosenLevel(int level_number);

	/**
	 * @brief Wyświetla wybrany obiekt do rzucania.
	 */
	void renderActiveThrowable();

	/**
	 * @brief Wyświetla trajektorię lotu wybranego obiektu do rzucania.
	 */
	void renderTrajectory();

	/**
	 * @brief Iteruje poprzez każdy obiekt w aktywnym poziome, ustalając jego nową pozycję i wyświetlając na ekranie.
	 */
	void updateObjects();

	/**
	 * @brief Wyświetla wszystkie obiekty do rzucania możliwe do wyboru. Użyte stają się bardziej przeźroczyste.
	 */
	void showLives();

	/**
	 * @brief Odpowiada za odpowiedź na wszystkie wydarzenia specyficzne dla stanu gru - Level, takie jak:
	 * R - restart, 1|2|3 - wybór obiektu do rzucania, użycie obiektu do rzucania, stworzenie bomby rzutu bombardującego,
	 * wczytywanie kolejnego poziomu lub restart po przegranej lub wygranej.
	 */
	void handleLevelEvents();

	/**
	 * @brief Odpowiada za odpowiedź na wszystkie wydarzenia specyficzne dla stanu gru - MapMenu.
	 * Czyli za wczytanie poziomu w zależności od klikniętego przycisku.
	 */
	void handleMapMenuEvents();

	/**
	 * @brief Tworzy przeszkode danego typu, o danym położeniu i danej długości, na podstawie interpretacji tablicy level_design.
	 * 
	 * @param obstacle_type Typ przeszkody
	 * @param k Szerokość umiejscowienia przeszkody
	 * @param j Wysokość umiejscowienia przeszkody
	 * @param number_of_repetition Ilość powtórzeń przeszkody w level_design skutkujące jej wydłużeniem
	 */
	void createLayoutObject(ObstacleType obstacle_type, int k, int j, int number_of_repetition);

	/**
	 * @brief Tworzy przeciwnika lub podłoże, o danym położeniu i danej długości, na podstawie interpretacji tablicy level_design.
	 * 
	 * @param object_type Typ obiektu - przeciwnik lub podłoże
	 * @param k Szerokość umiejscowienia obiektu
	 * @param j Wysokość umiejscowienia obiektu
	 */
	void createLayoutObject(ObjectType object_type, int k, int j);

	/**
	 * @brief Dla wybranego poziomu iteruje przez 22 wiersze i 41 kolumn tablicy level_design.
	 * Na podstawie znaku 'w', 's', 'm', 'e', 'g' tworzy kolejno przeszkode drewnianą, kamienną, metalową,
	 * przeciwnika lub podłoże.
	 * 
	 * @param level_number Wybrany poziom
	 */
	void interpretLevelLayout(int level_number);

public:
	/**
	 * @brief Konstruktor okna. Tworzy wszystkie potrzebne okna, przyciski i napisy potrzebne do wyświetlenia gry.
	 * 
	 * @param screen_width Szerokość ekranu
	 * @param screen_height Wysokość ekranu
	 */
	Screen(int screen_width = 1400, int screen_height = 800);

	/**
	 * @brief Destruktor okna. Niszczy i zwalnia pamięć aktywnego poziomu i wszystkich przycisków.
	 */
	~Screen();

	/**
	 * @brief Wczytuje punkty do tablicy scores z pliku binarnego lub tekstowego.
	 * 
	 * @param save_type Wybór pliku binarnego lub tekstowego do wczytania
	 */
	void loadScore(SaveType save_type);

	/**
	 * @brief Wczytuje design poziomów do tablicy level_design z pliku binarnego lub tekstowego.
	 * 
	 * @param save_type Wybór pliku binarnego lub tekstowego do wczytania
	 */
	void loadLevelsLayout(SaveType save_type);

	/**
	 * @brief Zapisuje tablice scores do pliku binarnego lub tekstowego.
	 * 
	 * @param save_type Wybór pliku binarnego lub tekstowego do zapisu
	 */
	void saveScore(SaveType save_type);

	/**
	 * @brief Tworzy okno w którym toczy się cała gra. Wywołuje funkcje i operacje w zależności od stanu gry.
	 * Odpowiada za wyświetlanie grafik i odbieranie inputu użytkownika w postaci zmiennej event.
	 */
	void createWindow();
};

#endif
