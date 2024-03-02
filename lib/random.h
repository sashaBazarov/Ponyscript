#ifndef RANDOM_H
#define RANDOM_H

class Random {
public:
    static int randint(int min, int max);
    static bool randbool();
    static double randdouble(double min, double max);
};

#endif // RANDOM_H
