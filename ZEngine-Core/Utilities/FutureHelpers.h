#pragma once

#include <future>

auto thenNoResult(std::future<void>& future, std::function<void(void)> continuation);

template<typename T, typename F>
auto then(std::future<T>& future, F&& continuation) {
  return std::async(std::launch::async, [future = std::move(future), continuation = std::forward<F>(continuation)]() mutable {
    return continuation(future.get());
  });
}
