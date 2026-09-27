#include "StaticBox.h"

#include "../Globals.h"
#include "../util/Graphics.h"

namespace {
constexpr float SIZE = 1.5F;

// Tile sheet layout
constexpr int SHEET_COLUMNS = 3;
constexpr int SHEET_TILES = 15;
constexpr float TILE_SIZE = 16;
}  // namespace

StaticBox::StaticBox(float x,
                     float y,
                     const asw::Texture& tile_sheet,
                     const std::array<int, 4>& tiles)
    : Box(x, y, SIZE, SIZE), tile_sheet(tile_sheet), tiles(tiles) {
  // Unknown tiles use the first one
  for (auto& tile : this->tiles) {
    if (tile < 0 || tile >= SHEET_TILES) {
      tile = 0;
    }
  }
}

void StaticBox::draw() const {
  const auto corner = screenPosition() - asw::Vec2<float>(SIZE, SIZE) *
                                             (PIXELS_PER_METER / 2);

  for (std::size_t i = 0; i < tiles.size(); i++) {
    const auto column = static_cast<float>(tiles[i] % SHEET_COLUMNS);
    const auto row = static_cast<float>(tiles[i] / SHEET_COLUMNS);
    const float offset_x = (i % 2 == 1) ? TILE_SIZE : 0;
    const float offset_y = (i >= 2) ? TILE_SIZE : 0;

    gfx::region(tile_sheet,
                asw::Quad<float>(column * TILE_SIZE, row * TILE_SIZE,
                                 TILE_SIZE, TILE_SIZE),
                asw::Quad<float>(corner.x + offset_x, corner.y + offset_y,
                                 TILE_SIZE, TILE_SIZE));
  }
}
