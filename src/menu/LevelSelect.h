/**
 * Level Select
 * Danny Van Stemp
 * Level select state
 * 04/01/2018
 **/

#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include <allegro5/allegro_font.h>

#include "../State.h"
#include "../ui/UIHandler.h"

class LevelSelect : public State {
 public:
  LevelSelect();
  ~LevelSelect() override;

  void draw() override;
  void update(StateEngine* engine) override;

 private:
  void createLevelButton(int, int, int);

  ALLEGRO_FONT* levelselect_font{nullptr};
  ALLEGRO_FONT* levelselect_font_large{nullptr};

  UIHandler levelSelectUI{};

  bool reset_game_menu{false};
};

#endif  // LEVELSELECT_H
