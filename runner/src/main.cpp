#include <iostream>

#include "../include/GameRunner.hpp"

int main() {
    BGBot* bot1 = new CLIBot();
    BGBot* bot2 = new CLIBot();
    GameRunner runner = GameRunner(bot1, bot2);

    return 0;
}
