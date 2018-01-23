#ifndef SOUND_H
#define SOUND_H

#include "tools.h"
#include "allegro5/allegro_audio.h"
#include "Options.h"

class Sound
{
  public:
    Sound();
    virtual ~Sound();
    void load(std::string);
    void play();
    void play_random_frequency(int,int);
    void play_at_volume(float newVolume);

  protected:

  private:

    ALLEGRO_SAMPLE *sample;
};

#endif // SOUND_H
