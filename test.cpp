#include "SMP.h"
#include <mutex>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#ifdef USE_STD_MUTEX
std::mutex g_mutex;
#else
SMP::Mutex g_mutex;
#endif

void addLoop(int* sum) {
  for (int i = 0; i < 1e6; ++i) {
#ifdef USE_STD_MUTEX
    std::unique_lock<std::mutex> lock(g_mutex);
#else
    LOCK(g_mutex, lock);
#endif
    ++sum;
  }
}

int main() {
  std::vector<std::thread> threads;
  int total;

  auto begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < THREADS; ++i) {
    threads.emplace_back(&addLoop, &total);
  }

  for (auto& thread : threads) {
    thread.join();
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end-begin;

  std::cout << diff.count() << std::endl;
}
