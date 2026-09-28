/**
 * Static Box
 * Danny Van Stemp
 * Decoration only, collision comes from collision boxes
 * 05/05/2017
 **/

#pragma once

#include <array>

#include "Box.h"

class StaticBox : public Box {
 public:
  // Tiles are frames of the static tile sheet for the top left, top right,
  // bottom left and bottom right corners
  StaticBox(float x,
            float y,
            const asw::SpriteSheet& tile_sheet,
            const std::array<int, 4>& tiles);

  void draw(const asw::Camera& camera) const override;
  BoxType getType() const override { return BoxType::Static; }

 private:
  asw::SpriteSheet tile_sheet;
  std::array<int, 4> tiles;
};
