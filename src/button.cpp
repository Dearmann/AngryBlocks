#include "button.h"

Button::Button(std::string file_location_passive,
    std::string file_location_active, float scale_x, float scale_y)
{
    this->button_type = ButtonType::Texture;
    this->setScale(scale_x, scale_y);
    setPassiveTexture(file_location_passive);
    setActiveTexture(file_location_active);
    this->setTexture(texture_passive);
}

Button::Button(float position_x, float position_y, float size_x, float size_y,
    sf::Color color_passive_fill,
    sf::Color color_passive_outline,
    sf::Color color_active_fill,
    sf::Color color_active_outline)
{
    this->button_type = ButtonType::Rectangle;
    this->color_passive_fill = color_passive_fill;
    this->color_passive_outline = color_passive_outline;
    this->color_active_fill = color_active_fill;
    this->color_active_outline = color_active_outline;
    this->size_x = size_x;
    this->size_y = size_y;
    font.loadFromFile("external/OriginalSurfer-Regular.ttf");
    rectangle_button.setSize(sf::Vector2f(size_x, size_y));
    rectangle_button.setFillColor(color_passive_fill);
    rectangle_button.setOutlineColor(color_passive_outline);
    rectangle_button.setOutlineThickness(2);
    text.setFillColor(sf::Color(255, 255, 255));
    //text.setPosition(
    //    position_x + rectangle_button.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2,
    //    position_y + rectangle_button.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2
    //);
    this->setButtonPosition(position_x, position_y);
    text.setCharacterSize(size_y);
    text.setFont(font);
}

Button::Button(float position_x, float position_y, float radius,
    sf::Color color_passive_fill,
    sf::Color color_passive_outline,
    sf::Color color_active_fill,
    sf::Color color_active_outline)
{
    this->button_type = ButtonType::Circle;
    this->color_passive_fill = color_passive_fill;
    this->color_passive_outline = color_passive_outline;
    this->color_active_fill = color_active_fill;
    this->color_active_outline = color_active_outline;
    this->radius = radius;
    font.loadFromFile("external/OriginalSurfer-Regular.ttf");
    circle_button.setRadius(radius);
    circle_button.setFillColor(color_passive_fill);
    circle_button.setOutlineColor(color_passive_outline);
    circle_button.setOutlineThickness(2);
    //circle_button.setOrigin(circle_button.getLocalBounds().width / 2,
    //    circle_button.getLocalBounds().height / 2);
    text.setFillColor(sf::Color(255, 255, 255));
    //text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    //text.setPosition(
    //    position_x + circle_button.getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2,
    //    position_y + circle_button.getGlobalBounds().height / 2 - text.getGlobalBounds().height / 2
    //);
    this->setButtonPosition(position_x, position_y);
    text.setCharacterSize(radius * 2);
    text.setFont(font);
}

bool Button::isHovered(sf::Vector2f mouse_position)
{
    if (button_type == ButtonType::Texture)
    {
        if (this->getGlobalBounds().contains(mouse_position))
        {
            this->setTexture(texture_active);
            return true;
        }
        else
        {
            this->setTexture(texture_passive);
            return false;
        }
    }
    else if (button_type == ButtonType::Rectangle)
    {
        if (rectangle_button.getGlobalBounds().contains(mouse_position))
        {
            rectangle_button.setFillColor(color_active_fill);
            rectangle_button.setOutlineColor(color_active_outline);
            text.setFillColor(sf::Color(255, 255, 255));
            return true;
        }
        else
        {
            rectangle_button.setFillColor(color_passive_fill);
            rectangle_button.setOutlineColor(color_passive_outline);
            text.setFillColor(sf::Color(255, 255, 255));
            return false;
        }
    }
    else if (button_type == ButtonType::Circle)
    {
        if (circle_button.getGlobalBounds().contains(mouse_position))
        {
            circle_button.setFillColor(color_active_fill);
            circle_button.setOutlineColor(color_active_outline);
            text.setFillColor(sf::Color(255, 255, 255));
            return true;
        }
        else
        {
            circle_button.setFillColor(color_passive_fill);
            circle_button.setOutlineColor(color_passive_outline);
            text.setFillColor(sf::Color(255, 255, 255));
            return false;
        }
    }
}

bool Button::isClicked(sf::RenderWindow& window, sf::Event& event)
{
    sf::Vector2f mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    if (allow_to_click
        && isHovered(mouse_position)
        && event.type == sf::Event::MouseButtonReleased
        && event.mouseButton.button == sf::Mouse::Left)
        return true;
    else
        return false;
}

void Button::setPassiveTexture(std::string file_location)
{
    if (button_type == ButtonType::Texture)
    {
        texture_passive.loadFromFile(file_location);
        texture_passive.setSmooth(true);
    }
}

void Button::setActiveTexture(std::string file_location)
{
    if (button_type == ButtonType::Texture)
    {
        texture_active.loadFromFile(file_location);
        texture_active.setSmooth(true);
    }
}

void Button::drawTo(sf::RenderWindow& window)
{
    setUnclickableColor();
    window.draw(circle_button);
    window.draw(rectangle_button);
    window.draw(text);
}

void Button::setButtonPosition(float x, float y)
{
    if (button_type == ButtonType::Texture)
    {
        this->setPosition(x, y);
    }
    else if (button_type == ButtonType::Rectangle)
    {
        rectangle_button.setPosition(x, y);
        text.setPosition(
            rectangle_button.getPosition().x + size_x / 14,
            rectangle_button.getPosition().y - size_y / 6
        );
    }
    else if (button_type == ButtonType::Circle)
    {
        circle_button.setPosition(x, y);
        if (text.getString() == "1") // Fixing position of text "1"
        {
            text.setPosition(
                circle_button.getPosition().x + radius / 1.7,
                circle_button.getPosition().y - radius / 4
            );
        }
        else
            text.setPosition(x + radius / 3, y - radius / 4);
    }
}

void Button::setUnclickableColor()
{
    if (!allow_to_click)
    {
        circle_button.setFillColor(sf::Color(245, 227, 169, 120));
        circle_button.setOutlineColor(sf::Color(241, 195, 131, 120));
        rectangle_button.setFillColor(sf::Color(245, 227, 169, 120));
        rectangle_button.setOutlineColor(sf::Color(241, 195, 131, 120));
        text.setFillColor(sf::Color(255, 255, 255, 120));
    }
}