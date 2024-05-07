#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

class Button {
public:
  Button(float x, float y, const sf::Texture& texture, bool isActive = true);
  virtual ~Button() = default;

  // Update button state based on mouse events (optional)
  virtual void update(const sf::RenderWindow& window) = 0;

  // Check if mouse clicks on the button
  bool handleMouseClick(const sf::Vector2i& mousePos) const;

  // Get button's unique identifier (optional)
  virtual int getId() const = 0;

  void draw(sf::RenderTarget& target) const;

private:
  sf::Sprite sprite;
  sf::Vector2f position;
  bool isActive;
  int id; // For future implementation of button types
};

#endif // BUTTON_HPP
