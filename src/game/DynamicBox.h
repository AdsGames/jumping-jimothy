/**
 * Dynamic Box
 * Danny Van Stemp
 * Physics based box that is influenced by
 *   gravity. Freezes while time is paused.
 * 30/07/2017
 **/

#pragma once

#include "Box.h"

class DynamicBox : public Box {
 public:
  DynamicBox(float x, float y, const asw::Texture& image, b2World& world);

  void draw(const asw::Camera& camera) const override;
  BoxType getType() const override { return BoxType::Dynamic; }
  bool isPausable() const override { return true; }

 private:
  asw::Texture image;
};
