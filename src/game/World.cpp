#include "World.h"

World::World(b2Vec2 gravity,
             float timeStep,
             int32 velocityIterations,
             int32 positionIterations,
             bool doSleep)
    : gravity(gravity),
      timeStep(timeStep),
      velocityIterations(velocityIterations),
      positionIterations(positionIterations),
      doSleep(doSleep) {
  // Create world
  bWorld = std::make_shared<b2World>(this->gravity);
}

// Get b2 world
std::shared_ptr<b2World> World::getB2World() const {
  return this->bWorld;
}

// Step
void World::step() const {
  this->bWorld->Step(timeStep, velocityIterations, positionIterations);
}
