/**
 * Explosive Button
 * Allan Legemaate
 * Editor button showing an explosive direction, the image can be rotated
 * 28/09/2026
 **/

#pragma once

#include <asw/asw.h>

class ExplosiveButton : public asw::ui::Button {
 public:
  ExplosiveButton(const asw::Texture& image, float angle);

  void draw(asw::ui::Context& ctx) override;

 private:
  asw::Texture image;

  // Radians
  float angle{0};
};
