#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <UIElement.h>


class CheckBox : public UIElement
{
  public:
    CheckBox();
    CheckBox(int, int, std::string, ALLEGRO_FONT*);
    virtual ~CheckBox();
    void update() override;
    virtual void draw();

  protected:

  private:
    int checkbox_width = 20;
    bool status;
};

#endif // CHECKBOX_H
