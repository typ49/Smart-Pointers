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
    Weak() {
    }

    /**
     * @brief Constructor takes a Shared pointer
     */
    Weak(const Shared<T>& shared) {
    }

    /**
     * @brief Get a Shared pointer from the Weak pointer
     *
     * If the raw pointer still exists, the method
     * initialize a Shared object. Otherwise, the method
     * retrun a non existing Shared pointeur.
     */
    Shared<T> lock() {
      return Shared<T>();
    }

  private:
    // implementation defined
  };
}

#endif // SP_WEAK_H
