#ifndef SP_WEAK_H
#define SP_WEAK_H

#include "Shared.h"

namespace sp {

  /**
   * @brief Smart weak pointer
   */
  template<typename T>
  class Weak {
  public:
    /**
     * @brief Default constructor
     */
    Weak() : m_ptr(nullptr), m_refCount(nullptr) {
    }

    /**
     * @brief Constructor takes a Shared pointer
     */
    Weak(const Shared<T>& shared) : m_ptr(shared.m_ptr), m_refCount(shared.m_refCount) {
    }

    /**
     * @brief Copy constructor
     */
    Weak(const Weak& other) : m_ptr(other.m_ptr), m_refCount(other.m_refCount) {
    }

    /**
     * @brief Copy assignment operator
     */
    Weak& operator=(const Weak& other) {
      if (this != &other) {
        m_ptr = other.m_ptr;
        m_refCount = other.m_refCount;
      }
      return *this;
    }

    /**
     * @brief Move constructor
     */
    Weak(Weak&& other) noexcept : m_ptr(other.m_ptr), m_refCount(other.m_refCount) {
      other.m_ptr = nullptr;
      other.m_refCount = nullptr;
    }

    /**
     * @brief Move assignment operator
     */
    Weak& operator=(Weak&& other) noexcept {
      if (this != &other) {
        m_ptr = other.m_ptr;
        m_refCount = other.m_refCount;
        other.m_ptr = nullptr;
        other.m_refCount = nullptr;
      }
      return *this;
    }

    /**
     * @brief Get a Shared pointer from the Weak pointer
     *
     * If the raw pointer still exists, the method
     * initialize a Shared object. Otherwise, the method
     * return a non existing Shared pointer.
     */
    Shared<T> lock() {
      if (m_refCount && *m_refCount > 0) {
        return Shared<T>(m_ptr);
      } else {
        return Shared<T>();
      }
    }

  private:
    T *m_ptr;
    std::size_t *m_refCount;
  };
};

#endif // SP_WEAK_H
