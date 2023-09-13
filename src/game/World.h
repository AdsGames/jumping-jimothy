#ifndef WORLD_H
#define WORLD_H

#include <box2d/box2d.h>

class World {
 public:
  // Constructor
  World(b2Vec2 gravity,
        float timeStep,
        int32 velocityIterations,
        int32 positionIterations,
        bool doSleep);

  // Get b2 world
  b2World* getB2World();

  // Step
  void step();

 private:
  // Box 2d game world
  b2World* bWorld;

  // Vars
  b2Vec2 gravity;
  float timeStep;
  int32 velocityIterations;
  int32 positionIterations;
  bool doSleep;
};

#endif  // WORLD_H
