#include "random.h"
#include <cstdlib>
#include <ctime>

int Random::randint(int min, int max) {
    srand(time(nullptr));
    return min + rand() % (max - min + 1);
}

bool Random::randbool() {
    srand(time(nullptr));
    return rand() % 2 == 0;
}

double Random::randdouble(double min, double max) {
    srand(time(nullptr));
    return min + (rand() / (RAND_MAX / (max - min)));
}
