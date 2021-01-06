#include "utils/task_pool.h"

#include <iostream>

int main() {
    Util::TaskPool pool(5, 10);
    pool.start();

    pool.add([]() {
       std::cout << "Fuck you" << std::endl;
    });

    pool.join();

    return 0;
}