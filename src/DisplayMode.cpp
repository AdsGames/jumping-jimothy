#include "DisplayMode.h"
#include "allegro5/allegro.h"
#include "Options.h"

int DisplayMode::window_render_width=1024;
int DisplayMode::window_render_height=768;

int DisplayMode::game_render_width=1024;
int DisplayMode::game_render_height=768;

float DisplayMode::scale_h = 0;
float DisplayMode::scale_w = 0;
float DisplayMode::scale_x = 0;
float DisplayMode::scale_y = 0;


ALLEGRO_DISPLAY *DisplayMode::gameDisplay=nullptr;
int DisplayMode::mode = 0;

DisplayMode::DisplayMode()
{
  //ctor
}

DisplayMode::~DisplayMode()
{
  //dtor
}

ALLEGRO_DISPLAY* DisplayMode::init(int newType){

    ALLEGRO_DISPLAY *newDisplay;

    if(newType==0){

      al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

      newDisplay = al_create_display(window_render_width, window_render_height);

      setMode(newDisplay,newType);
  }

    if(newType==2){


      al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);


      newDisplay = al_create_display(window_render_width, window_render_height);

      setMode(newDisplay,newType);
  }
    if(newType==4){

      al_set_new_display_flags(ALLEGRO_WINDOWED);

      newDisplay = al_create_display(game_render_width, game_render_height);

      setMode(newDisplay,newType);


  }

  return newDisplay;

}



void DisplayMode::setMode(ALLEGRO_DISPLAY *newDisplay,int newMode){

  if(newMode==4){

    std::cout<<"Setting mode to windowed.\n";

    window_render_height=768;
    window_render_width=1024;

    al_toggle_display_flag(newDisplay,ALLEGRO_FULLSCREEN_WINDOW,false);

    al_show_mouse_cursor(newDisplay);
    Options::draw_cursor=false;

    mode=4;

  }else if(newMode==0){

      std::cout<<"Setting mode to fullscreen windowed.\n";


      al_toggle_display_flag(newDisplay,ALLEGRO_FULLSCREEN_WINDOW,true);

      window_render_width = al_get_display_width(newDisplay);
      window_render_height = al_get_display_height(newDisplay);


      float sx = (float)window_render_width  / (float)game_render_width;
      float sy = (float)window_render_height  / (float)game_render_height;
      float scale = std::min(sx, sy);


      scale_w = (float)game_render_width * sx;
      scale_h = (float)game_render_height * sy;
      scale_x = ((float)window_render_width - scale_w) / 2;
      scale_y = ((float)window_render_height - scale_h) / 2;

      al_hide_mouse_cursor(newDisplay);
      Options::draw_cursor=true;

      mode=0;

  }else{

    if(newMode!=2)
      std::cout<<"WARNING: Invalid display mode passed, setting to mode 0.\n";

    std::cout<<"Setting mode to fullscreen centered.\n";

    al_toggle_display_flag(newDisplay,ALLEGRO_FULLSCREEN_WINDOW,true);

    window_render_width = al_get_display_width(newDisplay);
    window_render_height = al_get_display_height(newDisplay);

    float sx = window_render_width  / game_render_width;
    float sy = window_render_height  / game_render_height;
    float scale = std::min(sx, sy);

    scale_w = game_render_width * sx;
    scale_h = game_render_height * sy;
    scale_x = (window_render_width - scale_w) / 2;
    scale_y = (window_render_height - scale_h) / 2;

    al_hide_mouse_cursor(newDisplay);
    Options::draw_cursor=true;

    mode=2;
  }



}




