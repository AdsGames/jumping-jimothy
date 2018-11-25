#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <string>

class UIElement {
  public:
    // Constructor
    UIElement();
    UIElement(int, int, std::string, std::string, ALLEGRO_FONT*);

    // Dtor
    virtual ~UIElement();

    // Get X
    int getX();

    // Get y
    int getY();

    // Get text of element
    std::string getText();

    // Get element ID
    std::string getId();

    // Hide element
    void hide();

    // Show element
    void show();

    // Toggle visiblity
    void toggleVisibility();

    // Disable element
    void disable();

    // Enable element
    void enable();

    // Togle disabled
    void toggleDisabled();

    // Set transparency level
    void setTransparency(float alpha);

    // Set image rotation
    void setBitmapRotationAngle(float rotation);

    // Set x
    void setX(int x);

    // Set y
    void setY(int y);

    // Set text colour
    void setTextColour(ALLEGRO_COLOR colour);

    // Set background colour
    void setBackgroundColour(ALLEGRO_COLOR colour);

    // Set cell fill
    void setCellFillTransparent(bool n);

    // Set text justification
    void setTextJustification(int justification);

    // Element width
    int getWidth();

    // Element height
    int getHeight();

    // Set padding
    void setPadding(int x, int y);

    // Set position
    void setPosition(int x, int y);

    // Set element size
    void setSize(int width, int height);

    // Set text
    void setText(std::string text);

    // Set id
    void setId(std::string id);

    // Set image
    void setImage(ALLEGRO_BITMAP *image);

    // Set font
    void setFont(ALLEGRO_FONT *font);

    // Set background visibility
    void setVisibleBackground(bool b);

    // Set width
    void setWidth(int width);

    // Set height
    void setHeight(int height);

    // Set border thickness
    void setBorderThickness(int thickness);

    // Disable hover effect
    void disableHoverEffect();

    // Disable hover effect
    void enableHoverEffect();

    // Mouse over
    bool hover();

    // Clicked
    bool clicked();

    // Update element
    virtual void update() = 0;

    // Draw element
    virtual void draw() = 0;

  protected:
    // Variables
    int x;
    int y;
    int width;
    int height;

    int padding_x;
    int padding_y;

    float bitmap_rotation_angle;
    bool visible_background;

    int border_thickness;

    float alpha;

    int justification;

    // Frick you
    ALLEGRO_COLOR text_colour;
    ALLEGRO_COLOR background_colour;

    // Inactive cannot be clicked/hovered
    // Invisible cannot be seen
    // Disabled is both
    bool visible;
    bool disabled;

    bool transparent_cell_fill;
    bool hover_effect;

    ALLEGRO_BITMAP *image;

    // Font
    ALLEGRO_FONT *UIElement_font;

    std::string text;
    std::string id;


  private:

};

#endif // UIELEMENT_H
