#ifndef LABEL_H
#define LABEL_H

#include "ui/UIElement.h"

class Label : public UIElement {
  public:
    // Constructor
    Label();

    // Detailed constructor
    Label(int x, int y, std::string text, std::string id, ALLEGRO_FONT *font);

    // Dtor
    virtual ~Label() {};

    // Draw
    void draw() override;

    // Update
    void update() override;
};

#endif // LABEL_H
