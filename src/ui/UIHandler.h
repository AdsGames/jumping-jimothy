/**
 * UIHandler
 * Danny Van Stemp and Allan Legemaate
 * Manages UI
 * 24/09/2017
 **/

#ifndef UIHANDLER_H
#define UIHANDLER_H

#include <memory>
#include <string>
#include <vector>

#include "UIElement.h"

class UIHandler {
 public:
  UIHandler();

  void addElement(std::shared_ptr<UIElement> elem);
  void draw();
  void update();
  bool isHovering();

  void createButton(const int x,
                    const int y,
                    std::string text,
                    std::string id,
                    ALLEGRO_FONT* font);

  void createAnchoredButton(std::string text,
                            ALLEGRO_FONT* font,
                            std::string anchorID,
                            std::string id);

  UIElement* getElementById(std::string id);

  std::vector<std::shared_ptr<UIElement>> getUIElements();

 private:
  // Container for UI Elements
  std::vector<std::shared_ptr<UIElement>> ui_elements{};

  // Cursor image
  ALLEGRO_BITMAP* ui_cursor{nullptr};

  // Element in focus
  int focusedElement{-1};
};

#endif  // UIHANDLER_H
