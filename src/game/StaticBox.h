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
  // Tiles are indices into the 3 by 5 static tile sheet for the top left, top
  // right, bottom left and bottom right corners
  StaticBox(float x,
            float y,
            const asw::Texture& tile_sheet,
            const std::array<int, 4>& tiles);

  void draw() const override;
  BoxType getType() const override { return BoxType::Static; }

 private:
  asw::Texture tile_sheet;
  std::array<int, 4> tiles;
};
