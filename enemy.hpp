#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <vector>
using namespace sf;

class Enemy : public Drawable, public Transformable{
    private:
        Sprite m_sprite;
        Sprite originalImage;
        bool removeFlag;
        Sprite image;
        Vector2f pos;
        size_t targetWaypoint;
        float speed;
        float angle;
        std :: vector<Vector2f> waypoints;

        void draw(RenderTarget& target, RenderStates states) const override {
            states.transform *= getTransform();
            target.draw(image, states);
        }   

        void move() {
            if (targetWaypoint < waypoints.size()) {
            Vector2f target = waypoints[targetWaypoint];
            Vector2f movement = target - pos;

            float dist = std::sqrt(movement.x * movement.x + movement.y * movement.y);

            if (dist >= speed) {
                pos += movement / dist * speed;
            } else {
                if (dist != 0) {
                    pos += movement / dist * dist;
                }
                targetWaypoint++;
            }
            } else {
                // Enemy has reached the end of the path
                removeFlag = true;
            }
            setPosition(pos);
        }

         void rotate() {
            if (targetWaypoint < waypoints.size()) {
                sf::Vector2f dist = waypoints[targetWaypoint] - pos;
                angle = std::atan2(dist.y, dist.x) * (180 / static_cast<float>(3.14159265358979323846));
                image.setRotation(angle);
            }
    }

    public:
        Enemy(const std::vector<Vector2f>& waypoints, const Texture& texture) : 
            waypoints(waypoints),
            targetWaypoint(1),
            speed(8),
            angle(0),
            removeFlag(false),
            originalImage(texture),
            image(texture),
            pos(waypoints[0]){
                image.setOrigin(image.getLocalBounds().width / 2, image.getLocalBounds().height / 2);
            }

        bool shouldRemove() const {
            return removeFlag;
        }

        void update(){
            if(!removeFlag){
                move();
                rotate();
            }
        }
};