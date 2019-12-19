#include "Engine.hpp"
#include "catch.hpp"

// TODO: This needs to be instantiated ONCE for ALL tests to use,
// ideally in the main.cpp of the tests
// It also needs to be handled by a smart pointer or Engine::Instance() needs to
// return a smart pointer
actlib::RNG::Engine *engine = actlib::RNG::Engine::Instance();
