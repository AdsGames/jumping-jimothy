#include "Sound.h"

Sound::Sound()
{
}

Sound::~Sound()
{
  //dtor
}


void Sound::load(std::string newPath){

  sample = tools::load_sample_ex(newPath);

}

void Sound::play(){

  if(Options::sfx_enabled)
    al_play_sample( sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
  else
    std::cout<<"ur sound is disabled=-----------------\n";

}

void Sound::play_random_frequency(int newMin, int newMax){

  if(Options::sfx_enabled)
    al_play_sample( sample, 1.0, 0.0, (float)tools::random_int(newMin,newMax)/100, ALLEGRO_PLAYMODE_ONCE, NULL);

}

void Sound::play_at_volume(float newVolume){

  if(Options::sfx_enabled)
    al_play_sample( sample, newVolume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

}
