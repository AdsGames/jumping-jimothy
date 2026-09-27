#include "DynamicBox.h"

#include "../Globals.h"
#include "../util/Graphics.h"

namespace {
constexpr float SIZE = 1.55F;
constexpr float IMAGE_SIZE = 32;
}  // namespace

DynamicBox::DynamicBox(float x,
                       float y,
                       const asw::Texture& image,
                       b2World& world)
    : Box(x, y, SIZE, SIZE), image(image) {
  createBody(world, b2_dynamicBody);
}

void DynamicBox::draw() const {
  // Backing shows through the see through parts of the image
  const float fill = (SIZE * PIXELS_PER_METER) - 2;
  gfx::rotatedRectFill(screenQuad(fill, fill), screenAngle(),
                       asw::Color(0, 255, 0));

  gfx::region(image, asw::Quad<float>(0, 0, IMAGE_SIZE, IMAGE_SIZE),
              screenQuad(IMAGE_SIZE, IMAGE_SIZE), screenAngle());
}
