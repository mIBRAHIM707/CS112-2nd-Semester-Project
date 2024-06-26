#include <SFML/Graphics.hpp>

using namespace sf;

class Turret : public Drawable, public Transformable{
    private:
        Texture image;
        float tile_x;
        float tile_y;

        void draw(RenderTarget& target, RenderStates states) const override {
            Sprite turretSprite(image);
            turretSprite.setPosition(tile_x * 48, tile_y * 48);
            target.draw(Sprite(image), states);
        }

    public:
        Turret(Texture &texture, float x, float y) : image(texture), tile_x(x), tile_y(y){}



};
