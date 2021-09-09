#ifndef BUTTON_H
#define BUTTON_H

#include "SFML/Graphics.hpp"

/**
 * @brief Typ przycisku: okrągły, prostokątny, z użyciem tekstury.
 */
enum class ButtonType { Circle, Rectangle, Texture };


class Button : public sf::Sprite
{
	ButtonType button_type = ButtonType::Circle; ///< Typ przycisku: okrągły, prostokątny, z użyciem tekstury
	sf::Texture texture_passive; ///< Tekstura przycisku na którym nie znajduje się kursor myszy
	sf::Texture texture_active; ///< Tekstura przycisku na którym znajduje się kursor myszy
	sf::Font font; ///< Czcionka
public:
	sf::CircleShape circle_button; ///< Grafika przycisku okrągłego
	sf::RectangleShape rectangle_button; ///< Grafika przycisku prostokątnego
	sf::Text text; ///< Tekst w przycisku
	sf::Color color_passive_fill; ///< Kolor wnętrza przycisku na którym nie znajduje się kursor myszy
	sf::Color color_passive_outline; ///< Kolor zarysu przycisku na którym nie znajduje się kursor myszy
	sf::Color color_active_fill; ///< Kolor wnętrza przycisku na którym znajduje się kursor myszy
	sf::Color color_active_outline; ///< Kolor zarysu przycisku na którym znajduje się kursor myszy
	float size_x; ///< Szerokość przycisku
	float size_y; ///< Wysokość przycisku
	float radius; ///< Promień przycisku
	bool allow_to_click = true; ///< Zmienna określająca czy przycisk jest do kliknięcia

	/**
	 * @brief Konstruktor przycisku opartego o teksture
	 * 
	 * @param file_location_passive Lokacja pliku z teksturą przycisku na którym nie znajduje się kursor myszy
	 * @param file_location_active Lokacja pliku z teksturą przycisku na którym znajduje się kursor myszy
	 * @param scale_x Skala szerokości tekstury przycisku
	 * @param scale_y Skala wysokości tekstury przycisku
	 */
	Button(std::string file_location_passive,
		std::string file_location_active, float scale_x = 1, float scale_y = 1);

	/**
	 * @brief Konstruktor przycisku opartego o prostokątną grafike
	 * 
	 * @param position_x Pozycja przycisku na płaszczyźnie x
	 * @param position_y Pozycja przycisku na płaszczyźnie y
	 * @param size_x Szerokość przycisku
	 * @param size_y Wysokość przycisku
	 * @param color_passive_fill Kolor wnętrza przycisku na którym nie znajduje się kursor myszy
	 * @param color_passive_outline Kolor zarysu przycisku na którym nie znajduje się kursor myszy
	 * @param color_active_fill Kolor wnętrza przycisku na którym znajduje się kursor myszy
	 * @param color_active_outline Kolor zarysu przycisku na którym znajduje się kursor myszy
	 */
	Button(float position_x, float position_y, float size_x, float size_y,
		sf::Color color_passive_fill = sf::Color(245, 227, 169),
		sf::Color color_passive_outline = sf::Color(241, 195, 131),
		sf::Color color_active_fill = sf::Color(245, 227, 169),
		sf::Color color_active_outline = sf::Color(200, 145, 100));

	/**
	 * @brief Konstruktor przycisku opartego o okrągłą grafike
	 * 
	 * @param position_x Pozycja przycisku na płaszczyźnie x
	 * @param position_y Pozycja przycisku na płaszczyźnie y
	 * @param radius Promień przycisku
	 * @param color_passive_fill Kolor wnętrza przycisku na którym nie znajduje się kursor myszy
	 * @param color_passive_outline Kolor zarysu przycisku na którym nie znajduje się kursor myszy
	 * @param color_active_fill Kolor wnętrza przycisku na którym znajduje się kursor myszy
	 * @param color_active_outline Kolor zarysu przycisku na którym znajduje się kursor myszy
	 */
	Button(float position_x, float position_y, float radius,
		sf::Color color_passive_fill = sf::Color(245, 227, 169),
		sf::Color color_passive_outline = sf::Color(241, 195, 131),
		sf::Color color_active_fill = sf::Color(245, 227, 169),
		sf::Color color_active_outline = sf::Color(200, 145, 100));

	/**
	 * @brief Sprawdza czy kursor myszy znajduje się na przycisku.
	 * Na tej podstawie zmienia wygląd przycisku z pasywnego na aktywny i umożliwia kliknięcie.
	 * 
	 * @param mouse_position Pozycja myszy w pikselach
	 * @return true Kursor myszy znajduje się na przycisku
	 * @return false Kursor myszy nie znajduje się na przycisku
	 */
	bool isHovered(sf::Vector2f mouse_position);

	/**
	 * @brief Sprawdza czy przycisk został kliknięty.
	 * 
	 * @param window Ekran na którym znajduje się przycisk
	 * @param event Wydarzenie kliknięcia myszy na ekranie
	 * @return true Przycisk został kliknięty
	 * @return false Przycisk nie został kliknięty
	 */
	bool isClicked(sf::RenderWindow& window, sf::Event& event);

	/**
	 * @brief Ustawia pasywną teksture przycisku opartego o teksture.
	 * 
	 * @param file_location Lokacja pliku z teksturą pasywną
	 */
	void setPassiveTexture(std::string file_location);

	/**
	 * @brief Ustawia aktywną teksture przycisku opartego o teksture.
	 * 
	 * @param file_location Lokacja pliku z teksturą aktywną
	 */
	void setActiveTexture(std::string file_location);

	/**
	 * @brief Wyświetla przycisk na ekranie.
	 * 
	 * @param window Ekran na którym wyświetlony ma zostać przycisk
	 */
	void drawTo(sf::RenderWindow& window);

	/**
	 * @brief Ustawia pozycje przycisku.
	 * 
	 * @param x Pozycja na płaszczyźnie x
	 * @param y Pozycja na płaszczyźnie y
	 */
	void setButtonPosition(float x, float y);

	/**
	 * @brief Ustawia wygląd przycisku na przeźroczysty jeżeli jest on ustawiony jako nieaktywny na kliknięcia.
	 */
	void setUnclickableColor();
};

#endif
