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
     * @brief Destructor
     */
    ~Weak() {
      // No need to delete the pointer as it is managed by Shared
      // Just set to nullptr to avoid dangling pointer
      m_ptr = nullptr;
      m_refCount = nullptr;
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
        return Shared<T>(m_ptr, m_refCount); // Use the existing refCount
      } else {
        return Shared<T>();
      }
    }

    /**
     * @brief Check if the Weak pointer is expired
     *
     * @return true if the Weak pointer is expired, false otherwise
     */
    bool expired() const {
      return m_refCount == nullptr || *m_refCount == 0;
    }

  private:
    T *m_ptr;
    std::size_t *m_refCount;
  };
};

#endif // SP_WEAK_H
