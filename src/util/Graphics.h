/**
 * Graphics
 * Allan Legemaate
 * Drawing helpers asw does not provide
 * 27/09/2026
 **/

#pragma once

#include <asw/asw.h>

namespace gfx {

// Draw part of a texture, rotated clockwise by angle (radians) around the
// centre of dest and optionally mirrored
void region(const asw::Texture& texture,
            const asw::Quad<float>& source,
            const asw::Quad<float>& dest,
            float angle = 0.0F,
            bool flip_x = false);

// Draw a filled rectangle rotated clockwise by angle (radians) around its
// centre
void rotatedRectFill(const asw::Quad<float>& quad,
                     float angle,
                     asw::Color color);

// Draw a rectangle outline thickness pixels wide, drawn inside the quad
void thickRect(const asw::Quad<float>& quad,
               float thickness,
               asw::Color color);

// Height of a line of text
float lineHeight(const asw::Font& font);

}  // namespace gfx
