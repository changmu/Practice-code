/*
 * 任务：有n辆车，每辆有m的概率在一秒钟的时间里向前走一步，画出在k秒时间里面车的运动图.
 **/
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using namespace std;

struct Env {

struct Game {
    Game(double m, int k, const vector<int>& cars)
      : m(m)
      , k(k)
      , cars(cars)
    {
        srand(time(NULL));
    }
    double m; // 向前走的概率
    int k; // 时间，单位秒
    vector<int> cars;

    static void PrintCar(int p) {
        for (int i = 0; i < p; ++i)
            putchar('*');
        putchar('\n');
    }

    void Draw() {
        printf("cars' position:\n");
        for_each(cars.begin(), cars.end(), PrintCar);
        putchar('\n');
    }
};

static void MoveCar(double m, int& p) {
    if (rand() % 100 < m*100)
        p++;
}

static Game& RaceOnce(Game& game) {
    game.k--;
    for_each(game.cars.begin(), game.cars.end(), std::bind(MoveCar, game.m, placeholders::_1));
    sleep(1);
    return game;
}

static void Race(Game& game) {
    game.Draw();
    if (game.k > 0)
        Race(RaceOnce(game));
}

static int main() {
    Game game(0.5, 6, vector<int>(10, 1));
    Race(game);
    return 0;
}

};

int main(int argc, char const *argv[]) {
    return Env().main();
}
