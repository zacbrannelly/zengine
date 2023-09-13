#include "FutureHelpers.h"

auto thenNoResult(std::future<void>& future, std::function<void(void)> continuation) {
  return std::async(std::launch::async, [future = std::move(future), continuation = std::move(continuation)]() mutable {
    future.get();
    continuation();
  });
}
