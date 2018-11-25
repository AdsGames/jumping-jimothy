/**
 * Check Box
 * Danny Van Stemp and Allan Legemaate
 * Check Box
 * 16/10/2017
 **/

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "ui/UIElement.h"

class CheckBox : public UIElement {
  public:
    // Default constructor
    CheckBox();

    // Detailed constructor
    CheckBox(int x, int y, std::string text, std::string id, ALLEGRO_FONT *font);

    // Dtor
    virtual ~CheckBox() {};

    // Update
    void update() override;

    // Draw
    void draw() override;

    // Check
    bool getChecked();

  private:
    // Width of checkbox
    int checkbox_size;

    // Checked
    bool checked;
};

#endif // CHECKBOX_H
