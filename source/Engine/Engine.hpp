#ifndef Engine_hpp
#define Engine_hpp

#include <pcg_random.hpp>

namespace aleatoric {
class Engine {
  public:
    Engine();
    pcg32 &getEngine();

  private:
    pcg32 m_engine;
};
} // namespace aleatoric
#endif /* Engine_hpp */
