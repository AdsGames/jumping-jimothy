#include "StaticBox.h"

#include "../Globals.h"

namespace {
constexpr float SIZE = 1.5F;

constexpr float TILE_SIZE = 16;
}  // namespace

StaticBox::StaticBox(float x,
                     float y,
                     const asw::SpriteSheet& tile_sheet,
                     const std::array<int, 4>& tiles)
    : Box(x, y, SIZE, SIZE), tile_sheet(tile_sheet), tiles(tiles) {
  // Unknown tiles use the first one
  for (auto& tile : this->tiles) {
    if (tile < 0 || tile >= tile_sheet.get_frame_count()) {
      tile = 0;
    }
  }
}

void StaticBox::draw(const asw::Camera& camera) const {
  const auto corner = camera.world_to_screen(pixelPosition()) -
                      (asw::Vec2<float>(SIZE, SIZE) * (PIXELS_PER_METER / 2));

  for (std::size_t i = 0; i < tiles.size(); i++) {
    const float offset_x = (i % 2 == 1) ? TILE_SIZE : 0;
    const float offset_y = (i >= 2) ? TILE_SIZE : 0;

    tile_sheet.draw_frame(tiles[i],
                          asw::Quad<float>(corner.x + offset_x,
                                           corner.y + offset_y, TILE_SIZE,
                                           TILE_SIZE));
  }
}
