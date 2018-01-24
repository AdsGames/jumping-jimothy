#include "Sound.h"

Sound::Sound()
{
}

Sound::~Sound()
{
  #warning mess my crap the frick up
  //std::cout<<al_is_audio_installed()<<" is the answer to the question.\n";
  //al_destroy_sample( sample);

}


void Sound::load_wav(std::string newPath){

  is_wav=true;
  sample = tools::load_sample_ex(newPath);

}

void Sound::load_ogg(std::string newPath){

  sample = tools::load_sample_ex(newPath);

}

void Sound::play(){


  if(is_wav && Options::sfx_enabled){
    is_playing=true;
    al_play_sample( sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
  }
  else if(Options::music_enabled){
    al_play_sample( sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, sample_id);
    is_playing=true;

  }
}

void Sound::stop(){

  is_playing=false;
  if(!is_wav){

    // well frick you too future self
    al_stop_samples();

  }

}


void Sound::play_random_frequency(int newMin, int newMax){

  if(Options::sfx_enabled)
    al_play_sample( sample, 1.0, 0.0, (float)tools::random_int(newMin,newMax)/100, ALLEGRO_PLAYMODE_ONCE, NULL);

}

void Sound::play_at_volume(float newVolume){

  if(Options::sfx_enabled)
    al_play_sample( sample, newVolume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

}
