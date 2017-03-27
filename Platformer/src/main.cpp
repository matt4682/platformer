#include <Box2D\Box2D.h>

#include <exception>
#include <iostream>

#include "./game.h"
#include "./log.h"

using namespace std;

// TODO(Matt): Handle command line arguments
int main() {
  Game game;

  try {
    game.run();
  }
  catch (const exception &e) {
    gamelog(ERROR) << "Exception: " << e.what() << endl;
  }
  catch (...) {
    gamelog(ERROR) << "Unknown error" << endl;
  }
}