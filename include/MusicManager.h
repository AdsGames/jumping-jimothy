#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include "Sound.h"

class MusicManager
{
  public:
    MusicManager();
    virtual ~MusicManager();

    static Sound menu_music;
    static Sound game_music;

    static void load();

    static void destroy();

  protected:

  private:
};

#endif // MUSICMANAGER_H
