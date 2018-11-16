#ifndef STATICBOX_H
#define STATICBOX_H

#include <Box.h>

class StaticBox : public Box
{
  public:
    StaticBox();
    virtual ~StaticBox();
    void init(float,float,BITMAP*, BITMAP*, BITMAP*, BITMAP*);
    void draw();
    void update();

    int getType(){ return STATIC; };


  protected:

  private:

};

#endif // STATICBOX_H
