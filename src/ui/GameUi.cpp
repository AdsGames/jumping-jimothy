#include "GameUi.h"

#include <algorithm>

#include "../util/Controls.h"

namespace {
constexpr float PADDING = 10;
constexpr float BORDER = 2;
constexpr int HOVER_BRIGHTEN = 40;

const asw::Color BLACK(0, 0, 0);
const asw::Color GREY(200, 200, 200);
const asw::Color LIGHT_GREY(220, 220, 220);
const asw::Color WHITE(255, 255, 255);

asw::Color brighten(asw::Color colour) {
  const auto up = [](uint8_t value) {
    return static_cast<uint8_t>(std::min(255, value + HOVER_BRIGHTEN));
  };
  return {up(colour.r), up(colour.g), up(colour.b), colour.a};
}

asw::Color withAlpha(asw::Color colour, uint8_t alpha) {
  colour.a = alpha;
  return colour;
}
}  // namespace

void GameUi::setup(asw::ui::Root& ui, const asw::Font& font) {
  auto& nav = ui.ctx.navigation;
  nav.up = Controls::UP;
  nav.down = Controls::DOWN;
  nav.left = Controls::LEFT;
  nav.right = Controls::RIGHT;
  nav.activate = Controls::SELECT;
  nav.back = Controls::BACK;

  auto& theme = ui.ctx.theme;
  theme.font = font;
  theme.button = buttonStyle(GREY, BLACK);

  // Focus looks like hover, no ring
  theme.focus_ring.width = 0;

  auto& checkbox = theme.checkbox;
  checkbox.bg = GREY;
  checkbox.bg_hover = LIGHT_GREY;
  checkbox.border = BLACK;
  checkbox.border_width = BORDER;
  checkbox.box = GREY;
  checkbox.box_hover = LIGHT_GREY;
  checkbox.box_pressed = LIGHT_GREY;
  checkbox.box_border = BLACK;
  checkbox.box_border_width = BORDER;
  checkbox.mark = BLACK;
  checkbox.text = BLACK;
  checkbox.box_side = asw::ui::BoxSide::Right;
}

void GameUi::pointerOnly(asw::ui::Root& ui) {
  auto& nav = ui.ctx.navigation;
  nav.up = nav.down = nav.left = nav.right = GameUi::NO_ACTION;
  nav.next = nav.prev = nav.activate = nav.back = GameUi::NO_ACTION;
}

asw::ui::ButtonStyle GameUi::buttonStyle(asw::Color bg, asw::Color text,
                                         uint8_t alpha) {
  asw::ui::ButtonStyle style;
  style.bg = withAlpha(bg, alpha);
  style.bg_hover = withAlpha(brighten(bg), alpha);
  style.bg_pressed = style.bg_hover;
  style.bg_disabled = style.bg;
  style.text = withAlpha(text, alpha);
  style.text_hover = style.text;
  style.text_disabled = style.text;
  style.border = withAlpha(BLACK, alpha);
  style.border_width = BORDER;
  style.text_align = asw::TextJustify::Left;
  return style;
}

asw::ui::Button& GameUi::addButton(asw::ui::Root& ui, float x, float y,
                                   const std::string& text, float width,
                                   float height) {
  auto& button = ui.root.add_child<asw::ui::Button>();
  button.text = text;
  button.padding = PADDING;

  auto size = asw::Vec2<float>(width + (PADDING * 2), height + (PADDING * 2));
  if (width == 0 || height == 0) {
    const auto fit = fitText(ui.ctx.theme.font, text, PADDING);
    size.x = width == 0 ? fit.x : size.x;
    size.y = height == 0 ? fit.y : size.y;
  }

  button.transform = asw::Quad<float>(x, y, size.x, size.y);
  return button;
}

void GameUi::setOutline(asw::ui::Button& button) {
  button.draw_background = false;
  button.style = buttonStyle(GREY, WHITE);
}

asw::ui::Label& GameUi::addLabel(asw::ui::Root& ui, float x, float y,
                                 const std::string& text,
                                 const asw::Font& font, asw::Color color) {
  auto& label = ui.root.add_child<asw::ui::Label>();
  label.transform.position = asw::Vec2<float>(x, y);
  label.text = text;
  label.font = font;
  label.color = color;
  return label;
}

asw::Vec2<float> GameUi::fitText(const asw::Font& font,
                                 const std::string& text, float padding) {
  if (font == nullptr) {
    return {padding * 2, padding * 2};
  }

  return {static_cast<float>(asw::util::get_text_size(font, text).x) +
              (padding * 2),
          static_cast<float>(asw::util::get_font_height(font)) +
              (padding * 2)};
}
