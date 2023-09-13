/**
 * Menu
 * Menu state
 * Danny Van Stemp
 * 06/05/2017
 **/

#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro_image.h>

#include "../State.h"
#include "../ui/Button.h"
#include "../ui/UIHandler.h"
#include "../util/Sound.h"

enum {
  menu_button_play,
  menu_button_exit,
  menu_button_edit,
  menu_button_help,
  menu_button_options,
};

// Menu
class Menu : public State {
 public:
  Menu();
  ~Menu() override;

  void update(StateEngine* engine) override;
  void draw() override;

 private:
  // Images
  ALLEGRO_BITMAP* title{nullptr};
  ALLEGRO_BITMAP* title_overlay{nullptr};
  ALLEGRO_BITMAP* title_shine{nullptr};

  ALLEGRO_BITMAP* logo{nullptr};

  ALLEGRO_FONT* menu_font{nullptr};
  ALLEGRO_FONT* credits_font{nullptr};
  ALLEGRO_FONT* button_font{nullptr};

  UIHandler menu_ui;

  bool credits_menu{false};
  int counter_title{0};
};

#endif  // MENU_H
