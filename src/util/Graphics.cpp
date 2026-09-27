#include "Graphics.h"

#include <numbers>

namespace {
// Single white pixel, tinted to draw rotated rectangles
const asw::Texture& whitePixel() {
  static asw::Texture pixel = [] {
    auto* texture =
        SDL_CreateTexture(asw::display::get_renderer(), SDL_PIXELFORMAT_RGBA32,
                          SDL_TEXTUREACCESS_STATIC, 1, 1);
    const uint32_t white = 0xFFFFFFFF;
    SDL_UpdateTexture(texture, nullptr, &white, sizeof(white));
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // The renderer may already be gone when this is released at exit
    return asw::Texture(texture, [](SDL_Texture* t) {
      if (asw::display::get_renderer() != nullptr) {
        SDL_DestroyTexture(t);
      }
    });
  }();

  return pixel;
}
}  // namespace

void gfx::region(const asw::Texture& texture,
                 const asw::Quad<float>& source,
                 const asw::Quad<float>& dest,
                 float angle,
                 bool flip_x) {
  auto* renderer = asw::display::get_renderer();
  if (renderer == nullptr || texture == nullptr) {
    return;
  }

  const SDL_FRect src{source.position.x, source.position.y, source.size.x,
                      source.size.y};
  const SDL_FRect dst{dest.position.x, dest.position.y, dest.size.x,
                      dest.size.y};
  const double degrees = angle * (180.0 / std::numbers::pi);

  SDL_RenderTextureRotated(renderer, texture.get(), &src, &dst, degrees,
                           nullptr,
                           flip_x ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void gfx::rotatedRectFill(const asw::Quad<float>& quad,
                          float angle,
                          asw::Color color) {
  const auto& pixel = whitePixel();
  SDL_SetTextureColorMod(pixel.get(), color.r, color.g, color.b);
  SDL_SetTextureAlphaMod(pixel.get(), color.a);
  region(pixel, asw::Quad<float>(0, 0, 1, 1), quad, angle);
}

void gfx::thickRect(const asw::Quad<float>& quad,
                    float thickness,
                    asw::Color color) {
  const auto& [x, y] = quad.position;
  const auto& [w, h] = quad.size;

  asw::draw::rect_fill(asw::Quad<float>(x, y, w, thickness), color);
  asw::draw::rect_fill(asw::Quad<float>(x, y + h - thickness, w, thickness),
                       color);
  asw::draw::rect_fill(
      asw::Quad<float>(x, y + thickness, thickness, h - (thickness * 2)),
      color);
  asw::draw::rect_fill(asw::Quad<float>(x + w - thickness, y + thickness,
                                        thickness, h - (thickness * 2)),
                       color);
}

float gfx::lineHeight(const asw::Font& font) {
  if (font == nullptr) {
    return 0.0F;
  }

  return static_cast<float>(TTF_GetFontHeight(font.get()));
}
