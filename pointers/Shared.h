#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>
#include <map>

namespace sp {

  /**
   * @brief Smart shared pointer class
   */
  template<typename T>
  class Shared {
  public:
    /**
     * @brief Constructor takes a dynamic pointer
     */
    Shared(T* ptr = nullptr) {
    }

    /**
     * @brief Get the raw pointer
     */
    T* get() {
      return nullptr;
    }

    /**
     * @brief Get a reference on pointed data
     */
    T& operator*() {
      return T();
    }

    /**
     * @brief Get the raw pointer
     */
    T* operator->() {
      return nullptr;
    }

    /**
     * @brief Get the number of Shared pointed on the current pointer
     */
    std::size_t count() const {
      return 0;
    }

    /**
     * @brief  Check if the raw pointer exists
     */
    bool exists() const {
      return false;
    }

  private:
    // implementation defined
  };
}

#endif // SP_SHARED_H
