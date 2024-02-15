#ifndef SP_UNIQUE_H
#define SP_UNIQUE_H

namespace sp {

  /**
   * @brief Smart unique pointer
   */
  template<typename T>
  class Unique {
  public:
    /**
     * @brief Constructor takes a dynamic pointer
     */
    Unique(T* ptr = nullptr) {
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
     * @brief Check if the raw pointer exists
     */
    bool exists() const {
      return false;
    }

  private:
    // implementation defined
  };
}

#endif // SP_UNIQUE_H
