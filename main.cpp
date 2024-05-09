#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "enemy.hpp"
#include "world.hpp"
#include "turret.hpp"
#include "button.hpp"
using namespace sf;

void create_turret(Vector2f mouse_pos, Texture& texture, std::vector<Turret>& turrets){
    float mouse_tile_x = mouse_pos.x / 48;
    float mouse_tile_y = mouse_pos.y / 48;
    float mouse_tile_num = (mouse_tile_y * 15) + mouse_tile_x;
    Turret new_turret(texture, mouse_tile_x, mouse_tile_y);
    turrets.push_back(new_turret);
}

void create_turret(Vector2f mouse_pos, Texture& texture, std::vector<Turret>& turrets){
    // Calculate tile coordinates
    int tile_size = 48; // Assuming each tile is 48x48 pixels
    int mouse_tile_x = static_cast<int>(mouse_pos.x / tile_size);
    int mouse_tile_y = static_cast<int>(mouse_pos.y / tile_size);

    // Create the turret at the center of the tile
    float turret_x = mouse_tile_x * tile_size + tile_size / 2.0f;
    float turret_y = mouse_tile_y * tile_size + tile_size / 2.0f;

    // Create and add the turret to the vector
    Turret new_turret(texture, turret_x, turret_y);
    new_turret.setPosition(Vector2f(turret_x, turret_y));
    turrets.push_back(new_turret);
}

int main()
{
    //game window
    RenderWindow window(VideoMode(15 * 48, 15 * 48), "TowerDefence");
    window.setFramerateLimit(60);

    //load images
    Texture enemy_image;
    enemy_image.loadFromFile("C:\\Users\\aimra\\Desktop\\TD\\assets\\images\\enemies\\enemy_1.png");
    Texture map_image;
    map_image.loadFromFile("C:\\Users\\aimra\\Desktop\\TD\\levels\\level.png");
    Texture turret_cursor;
    turret_cursor.loadFromFile("C:\\Users\\aimra\\Desktop\\TD\\assets\\images\\turrets\\cursor_turret.png");
    //Enemy Groups
    std::vector<Vector2f> waypoints = {
        Vector2f(624.29, -0.2354),Vector2f(623.678, 191.724),
        Vector2f(479.966, 192.118),Vector2f(479.882, 96.006),Vector2f(95.9785, 95.8455),
        Vector2f(96.1, 479.978),Vector2f(240.224, 480.037),Vector2f(239.834, 287.851),
        Vector2f(384.029, 288.047),Vector2f(384.028, 336.083),Vector2f(672.128, 335.947),
        Vector2f(672.095, 623.988),Vector2f(527.884, 624.03),Vector2f(528.001, 480.038),
        Vector2f(383.978, 480.048),Vector2f(384.031, 623.969),Vector2f(-59.2385, 623.619)};

    std :: vector <std :: unique_ptr<Enemy>> enemies;
    Enemy enemy1(waypoints, enemy_image);
    enemies.push_back(std :: make_unique<Enemy>(enemy1));

    VertexArray lines(LinesStrip, waypoints.size());
    for(int i = 0; i < waypoints.size(); i++){
        lines[i].position = waypoints[i];
        lines[i].color = Color :: White;
    }

    // turret groups
    std :: vector <Turret> turrets;

    // world
    World world(map_image);

    //main loop
    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event :: Closed){
                window.close(); 
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (mousePos.x < 15 * 48 && mousePos.y < 15 * 48) {
                    int mouseTileX = mousePos.x / 48;
                    int mouseTileY = mousePos.y / 48;
                    bool spaceIsFree = true;
                    turrets.emplace_back(turret_cursor, mouseTileX, mouseTileY);
                }
            }
        }

        window.clear(Color :: Black);
        window.draw(world);
        window.draw(lines);
        for (auto it = enemies.begin(); it != enemies.end();) {
            (*it)->update();
            if ((*it)->shouldRemove()) {
                it = enemies.erase(it);
            } 
            else {
                window.draw(**it);
                ++it;
            }
        }
        for (auto& turret : turrets) {
            window.draw(turret);
        }

        sf::Text textFont;
  if (!textFont.loadFromFile("fonts/Consolas.ttf")) {
    // Handle loading error
    return 1;
  }
  sf::Text largeFont;
  largeFont.setFont(textFont);
  largeFont.setCharacterSize(36);

  World world(constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT, mapTexture);
  if (!world.loadLevel("levels/level.tmj")) {
    // Handle loading error
    return 1;
  }

  // Create sprites and buttons
  sf::Sprite cursorTurretSprite(cursorTurretTexture);

  std::vector<Button> buttons;
  buttons.push_back(Button(constants::SCREEN_WIDTH + 30, 120, buyTurretTexture, true));
  buttons.push_back(Button(constants::SCREEN_WIDTH + 50, 180, cancelTexture, true));
  buttons.push_back(Button(constants::SCREEN_WIDTH + 5, 180, upgradeTexture, true));
  buttons.push_back(Button(constants::SCREEN_WIDTH + 60, 300, beginTexture, true));
  buttons.push_back(Button(310, 300, restartTexture, true));
  buttons.push_back(Button(constants::SCREEN_WIDTH + 50, 300, fastForwardTexture, false));

  GameState gameState = GameState::Menu;
  sf::Clock gameClock;
  bool placingTurrets = false;
  std::unique_ptr<Turret> selectedTurret; // Use a smart pointer for ownership management

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (gameState == GameState::Playing && mousePos.x < constants::SCREEN_WIDTH && mousePos.y < constants::SCREEN_HEIGHT) {
          if (placingTurrets) {
            if (world.getMoney() >= constants::BUY_COST) {
              selectedTurret = std::make_unique<Turret>(turretTextures, mousePos / constants::TILE_SIZE, world.getTileMap(), shotBuffer);
              world.addTurret(std::move(selectedTurret));
              world.decreaseMoney(constants::BUY_COST);
            }
            placingTurrets = false;
          } else {
            selectedTurret.reset(); // Deselect previous turret
            for (auto& turret : world.getTurrets()) {
              if (turret->getTilePosition() == mousePos / constants::TILE_SIZE) {
                selectedTurret = std::make_unique<Turret>(*turret); // Copy constructor for selection
                break;
              }
            }
          }
        } else {
          for (auto& button : buttons) {
            if (button.handleMouseClick(mousePos)) {
              if (button.getId() == 0) { // Buy turret button
                placingTurrets = true;
              } else if (button.getId() == 1 && gameState == GameState::Playing) { // Cancel button
                placingTurrets = false;
              } else if (button.getId() == 2 && gameState == GameState::Playing && selectedTurret && selectedTurret->canUpgrade() && world.getMoney() >= constants::UPGRADE_COST) {
                selectedTurret->upgrade();
                world.decreaseMoney(constants::UPGRADE_COST);
              } else if (button.getId() == 3 && gameState == GameState::Menu) {
                gameState = GameState::Playing;
              } else if (button.getId() == 4 && (gameState == GameState::Playing || gameState == GameOver)) {
                // Restart level
                world.reset();
                gameState = GameState::Menu;
              }
            }
          }
        }
      }
    }

    float dt = gameClock.restart().asSeconds();


        window.display();
    }
    std :: vector <std :: unique_ptr<Enemy>> enemies;
    Enemy enemy1(waypoints, enemy_image);
    enemies.push_back(std :: make_unique<Enemy>(enemy1));

    VertexArray lines(LinesStrip, waypoints.size());
    for(int i = 0; i < waypoints.size(); i++){
        lines[i].position = waypoints[i];
        lines[i].color = Color :: White;
    }
void World::update(float dt, const std::map<std::string, sf::Texture>& enemyTextures) {
  if (gameSpeed > 1.0f) {
    dt *= gameSpeed;
  }

  // Update enemies
  for (auto& enemy : enemies) {
    enemy->update(dt, waypoints);
  }

  // Check for collisions and remove dead enemies
  for (auto it = enemies.begin(); it != enemies.end();) {
    if (it->get()->isDead()) {
      increaseMoney(constants::ENEMY_REWARD);
      it = enemies.erase(it);
    } else {
      // Check for collision with player base
      if (it->get()->getPosition().x <= 0) {
        health -= it->get()->getDamage();
        enemies.erase(it);
      } else {
        ++it;
      }
    }
  }

  // Update turrets
  for (auto& turret : turrets) {
    turret->update(dt, enemies);
  }
}

void draw(sf::RenderTarget& target) const {
  // Draw map
  sf::Sprite mapSprite(mapTexture);
  target.draw(mapSprite);

  // Draw enemies
  for (const auto& enemy : enemies) {
    enemy->draw(target, enemyTextures.at(enemy->getType()));
  }

  // Draw turrets
  for (const auto& turret : turrets) {
    turret->draw(target);
  }

  // Draw UI elements
  drawHealthBar(target);
  drawMoneyBar(target);
  drawLogo(target);

  if (gameState == GameState::Playing) {
    if (placingTurrets) {
      cursorTurretSprite.setPosition(sf::Mouse::getPosition(window) / constants::TILE_SIZE * constants::TILE_SIZE);
      target.draw(cursorTurretSprite);
    }


    return 0;
}