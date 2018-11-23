#include "Options.h"
#include "DisplayMode.h"
#include "Config.h"


Options::Options()
{
    Config::read_data("data/options_data.xml");

    options_font = al_load_ttf_font( "fonts/munro.ttf", 18, 0);
    title_font = al_load_ttf_font( "fonts/munro.ttf", 36, 0);

    cursor = tools::load_bitmap_ex("images/cursor.png");
    highlight = tools::load_bitmap_ex("images/highlight.png");

    OptionsUI.addElement(new UIElement(25, 25, "Options",title_font));
    OptionsUI.getElementByText("Options") -> setVisibleBackground(false);
    OptionsUI.getElementByText("Options") -> setTextColour(al_map_rgb(255,255,255));

    if(Config::joystick_data == "")
      Config::joystick_data = "None detected.";

    OptionsUI.addElement(new UIElement(400,25,"Gamepad: " + Config::joystick_data,"joydata",options_font));
    OptionsUI.getElementById("joydata") -> setVisibleBackground(false);
    OptionsUI.getElementById("joydata") -> setTextColour(al_map_rgb(255,255,255));

    OptionsUI.addElement(new Button(100, 101, "Toggle SFX",options_font));
    OptionsUI.getElementByText("Toggle SFX") -> setSize(180,18);
    OptionsUI.getElementByText("Toggle SFX") -> setTextColour(al_map_rgb(255,255,255));
    OptionsUI.getElementByText("Toggle SFX") ->setVisibleBackground(false);


    OptionsUI.addElement(new Button(100, 151, "Toggle Music",options_font));
    OptionsUI.getElementByText("Toggle Music") -> setSize(180,18);
    OptionsUI.getElementByText("Toggle Music") -> setTextColour(al_map_rgb(255,255,255));
    OptionsUI.getElementByText("Toggle Music") ->setVisibleBackground(false);

    OptionsUI.addElement(new Button(260,101,"Off","sfx_toggle",options_font));
    OptionsUI.getElementById("sfx_toggle") -> setBackgroundColour(al_map_rgb(150,0,0));
    OptionsUI.getElementById("sfx_toggle") ->setSize(20,18);

    OptionsUI.addElement(new Button(260,151,"Off","music_toggle",options_font));
    OptionsUI.getElementById("music_toggle") -> setBackgroundColour(al_map_rgb(150,0,0));
    OptionsUI.getElementById("music_toggle") ->setSize(20,18);

    OptionsUI.addElement(new Button(100, 201, "Graphics Mode",options_font));
    OptionsUI.getElementByText("Graphics Mode") -> setSize(180,18);
    OptionsUI.getElementByText("Graphics Mode") -> setTextColour(al_map_rgb(255,255,255));
    OptionsUI.getElementByText("Graphics Mode") -> setVisibleBackground(false);


    OptionsUI.addElement(new UIElement(300,200,DisplayMode::getDisplayModeString(),"graphicsdata",options_font));
    OptionsUI.getElementById("graphicsdata") -> setVisibleBackground(false);
    OptionsUI.getElementById("graphicsdata") -> setTextColour(al_map_rgb(255,255,255));


    OptionsUI.addElement(new Button(100, 251, "Apply Graphics",options_font));
    OptionsUI.getElementByText("Apply Graphics") -> setSize(180,18);
    OptionsUI.getElementByText("Apply Graphics") -> setTextColour(al_map_rgb(255,255,255));
    OptionsUI.getElementByText("Apply Graphics") ->setVisibleBackground(false);

    OptionsUI.addElement(new Button(100, 301, "Back",options_font));
    OptionsUI.getElementByText("Back") -> setSize(180,18);
    OptionsUI.getElementByText("Back") -> setTextColour(al_map_rgb(255,255,255));
    OptionsUI.getElementByText("Back") ->setVisibleBackground(false);


    Config::read_data("data/options_data.xml");
    updateUI();

    // Graphics mode
    temp_graphics_mode = DisplayMode::getDisplayMode();



}

Options::~Options()
{
  //dtor
}

void Options::draw(){

    al_clear_to_color( al_map_rgb(50,50,50));

    OptionsUI.draw();

    if(Config::draw_cursor)
      al_draw_bitmap(cursor,MouseListener::mouse_x,MouseListener::mouse_y,0);

    al_draw_bitmap(highlight,100,highlight_y,0);


}

void Options::update(){


  if(KeyListener::keyPressed[ALLEGRO_KEY_ESCAPE] || OptionsUI.getElementByText("Back") -> mouseReleased() ||
        (JoystickListener::buttonReleased[JOY_XBOX_A] && highlight_y_destination==350) || JoystickListener::buttonReleased[JOY_XBOX_B]){
    set_next_state(STATE_MENU);
  }
  OptionsUI.update();


  if(OptionsUI.getElementById("sfx_toggle") -> mouseReleased() || OptionsUI.getElementByText("Toggle SFX") -> mouseReleased() ||
        (JoystickListener::buttonReleased[JOY_XBOX_A] && highlight_y_destination==100)){
    Config::sfx_enabled=!Config::sfx_enabled;
    Config::write_data("data/options_data.xml");
    updateUI();
  }

  if(OptionsUI.getElementById("music_toggle") -> mouseReleased() || OptionsUI.getElementByText("Toggle Music") -> mouseReleased() ||
        (JoystickListener::buttonReleased[JOY_XBOX_A] && highlight_y_destination==150)){
    Config::music_enabled=!Config::music_enabled;
    Config::write_data("data/options_data.xml");
    updateUI();
  }

  if(OptionsUI.getElementByText("Graphics Mode") -> mouseReleased()){
    temp_graphics_mode = (temp_graphics_mode + 1) % NUM_GRAPHICS_MODES;
    OptionsUI.getElementById("graphicsdata") -> setText(DisplayMode::getDisplayModeString(temp_graphics_mode));
    updateUI();
  }

  if(OptionsUI.getElementByText("Apply Graphics") -> mouseReleased()){
    DisplayMode::setMode(temp_graphics_mode);
    Config::write_data("data/options_data.xml");
    updateUI();
  }


  if (OptionsUI.getElementByText("Toggle SFX") -> hover() && !Config::joystick_mode) {
    highlight_y_destination=100;
  }
  if (OptionsUI.getElementByText("Toggle Music") ->hover() && !Config::joystick_mode) {
    highlight_y_destination=150;
  }
  if (OptionsUI.getElementByText("Graphics Mode") -> hover() && !Config::joystick_mode) {
    highlight_y_destination=200;
  }
  if (OptionsUI.getElementByText("Apply Graphics") -> hover() && !Config::joystick_mode) {
    highlight_y_destination=250;
  }
  if (OptionsUI.getElementByText("Back") -> hover() && !Config::joystick_mode) {
    highlight_y_destination=300;
  }

  if(highlight_y>highlight_y_destination)highlight_y-=10;
  if(highlight_y<highlight_y_destination)highlight_y+=10;


  if((JoystickListener::stickDirections[LEFT_STICK_UP] || JoystickListener::stickDirections[DPAD_UP2]) && !joystick_direction_hit){
    if(highlight_y_destination<350)
      highlight_y_destination+=50;
  }

  if((JoystickListener::stickDirections[LEFT_STICK_DOWN] || JoystickListener::stickDirections[DPAD_DOWN])  && !joystick_direction_hit){
    if(highlight_y_destination>100)
      highlight_y_destination-=50;
  }

  if(JoystickListener::stickDirections[LEFT_STICK_DOWN] || JoystickListener::stickDirections[LEFT_STICK_UP] || JoystickListener::stickDirections[DPAD_DOWN] || JoystickListener::stickDirections[DPAD_UP2]){
    joystick_direction_hit=true;
    Config::joystick_mode=true;
  }else{
    joystick_direction_hit=false;
  }
  if(MouseListener::mouse_moved){
    Config::joystick_mode=false;
  }

  OptionsUI.getElementById("joydata") -> setText("Gamepad: "+ Config::joystick_data);


}

void Options::updateUI(){

  if(Config::sfx_enabled){
    OptionsUI.getElementById("sfx_toggle") -> setBackgroundColour(al_map_rgb(0,150,0));
    OptionsUI.getElementById("sfx_toggle") -> setText("On");
  }else{
    OptionsUI.getElementById("sfx_toggle") -> setBackgroundColour(al_map_rgb(150,0,0));
    OptionsUI.getElementById("sfx_toggle") -> setText("Off");
  }

  if(Config::music_enabled){
    OptionsUI.getElementById("music_toggle") -> setBackgroundColour(al_map_rgb(0,150,0));
    OptionsUI.getElementById("music_toggle") -> setText("On");
  }else{
    OptionsUI.getElementById("music_toggle") -> setBackgroundColour(al_map_rgb(150,0,0));
    OptionsUI.getElementById("music_toggle") -> setText("Off");
  }

}

