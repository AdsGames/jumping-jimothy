#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "UIElement.h"

class CheckBox : public UIElement {
  public:
    // Ctor
    CheckBox();
    CheckBox(int, int, std::string, std::string, ALLEGRO_FONT*);

    // Dtor
    virtual ~CheckBox() {};

    // Update
    void update() override;

    // Draw
    virtual void draw();

    // Check
    bool getChecked();

  private:
    int checkbox_width = 20;
    bool checked;
};

#endif // CHECKBOX_H
