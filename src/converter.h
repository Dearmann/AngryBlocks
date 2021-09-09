#ifndef CONVERTER_H
#define CONVERTER_H

namespace Converter
{
	constexpr double PIXELS_PER_METER = 32.0; ///< Ilość pikseli (w SFML) na 1 metr (w Box2D)
	constexpr double PI = 3.14159265358979323846; ///< Liczba pi

	/**
	 * @brief Przekształca wartość w pikselach na metry.
	 * 
	 * @tparam T Wartość w pikselach
	 * @param x Wartość w pikselach
	 * @return constexpr T Wartość w metrach
	 */
	template<typename T>
	constexpr T pixelsToMeters(const T& x)
	{
		return x / PIXELS_PER_METER;
	}

	/**
	 * @brief Przekształca wartość w metrach na piksele.
	 * 
	 * @tparam T Wartość w metrach
	 * @param x Wartość w metrach
	 * @return constexpr T Wartość w pikselach
	 */
	template<typename T>
	constexpr T metersToPixels(const T& x)
	{
		return x * PIXELS_PER_METER;
	}

	/**
	 * @brief Przekształca wartość kąta w stopniach na radiany.
	 * 
	 * @tparam T Wartość w stopniach
	 * @param x Wartość w stopniach
	 * @return constexpr T Wartość w radianach
	 */
	template<typename T>
	constexpr T degToRad(const T& x)
	{
		return PI * x / 180.0;
	}

	/**
	 * @brief Przekształca wartość kąta w radianach na stopnie.
	 * 
	 * @tparam T Wartość w radianach
	 * @param x Wartość w radianach
	 * @return constexpr T Wartość w stopniach
	 */
	template<typename T>
	constexpr T radToDeg(const T& x)
	{
		return 180.0 * x / PI;
	}
}

#endif
