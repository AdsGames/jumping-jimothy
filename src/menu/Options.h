/**
 * Options
 * Danny Van Stemp and Allan Legemaate
 * The options menu state. Works with config
 * 22/11/2018
 **/

#ifndef OPTIONS_H
#define OPTIONS_H

#include <allegro5/allegro_font.h>

#include "../State.h"
#include "../ui/UIHandler.h"

// Options state
class Options : public State {
 public:
  Options();
  ~Options() override;

  void draw() override;
  void update(StateEngine* engine) override;
  void updateUI();

 private:
  ALLEGRO_FONT* options_font{nullptr};
  ALLEGRO_FONT* title_font{nullptr};

  UIHandler OptionsUI;

  int temp_graphics_mode{0};
};

#endif  // OPTIONS_H
