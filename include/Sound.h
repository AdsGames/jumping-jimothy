/*
 * Sound
 * Wrapper for allegro sample
 * Easy interface to interact with samples
 * Danny Van Stemp
 * 23/11/2018
 */

#ifndef SOUND_H
#define SOUND_H

#include <allegro5/allegro_audio.h>
#include <string>

class Sound {
  public:
    Sound();
    virtual ~Sound();
    void load_wav(std::string);
    void load_ogg(std::string);
    void play();
    void play(float volume);
    void stop();
    void play_random_frequency(int,int);
    void play_at_volume(float newVolume);
    ALLEGRO_SAMPLE_ID *getSampleId();
    ALLEGRO_SAMPLE *getSample();

    bool getIsPlaying(){return is_playing;}

  private:

    ALLEGRO_SAMPLE *sample;
    ALLEGRO_SAMPLE_ID *sample_id;

    bool is_wav;
    bool is_playing;
};

#endif // SOUND_H
