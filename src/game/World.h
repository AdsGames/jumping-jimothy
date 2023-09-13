#ifndef WORLD_H
#define WORLD_H

#include <box2d/box2d.h>
#include <memory>

class World {
 public:
  // Constructor
  World(b2Vec2 gravity,
        float timeStep,
        int32 velocityIterations,
        int32 positionIterations,
        bool doSleep);

  // Get b2 world
  std::shared_ptr<b2World> getB2World() const;

  // Step
  void step() const;

 private:
  // Box 2d game world
  std::shared_ptr<b2World> bWorld;

  // Vars
  b2Vec2 gravity;
  float timeStep;
  int32 velocityIterations;
  int32 positionIterations;
  bool doSleep;
};

#endif  // WORLD_H
