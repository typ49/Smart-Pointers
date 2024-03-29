#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>
#include <map>
#include <stdexcept> // Include for std::runtime_error

namespace sp
{

  // a struct to keep track of the reference count and weak count
  struct BlockControl
  {
    std::size_t refCount = 0;  // Count of Shared pointers
    std::size_t weakCount = 0; // Count of Weak pointers

    BlockControl() : refCount(1), weakCount(0) {} // Initialize refCount to 1 for the first Shared pointer
  };

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Smart shared pointer
   */
  template <typename T>
  class Shared
  {
  public:
    /**
     * @brief Constructor takes a dynamic pointer
     */
    Shared(T *ptr = nullptr)
    {
      if (ptr)
      {
        m_ptr = ptr;
        m_block = new BlockControl(); // Create a new BlockControl instance
      }
      else
      {
        m_ptr = nullptr;
        m_block = nullptr;
      }
    }

    // Destructor
    ~Shared()
    {
      if (m_block && --(m_block->refCount) == 0)
      {
        delete m_ptr;
        if (m_block->weakCount == 0)
        {
          delete m_block;
        }
      }
    }

    // Move constructor
    Shared(Shared &&other) noexcept : m_block(other.m_block), m_ptr(other.m_ptr)
    {
      other.m_ptr = nullptr;
      other.m_block = nullptr;
    }

    // Move assignment operator
    Shared &operator=(Shared &&other) noexcept
    {
      if (this != &other)
      {
        releaseResources();
        m_ptr = other.m_ptr;
        m_block = other.m_block;
        other.m_ptr = nullptr;
        other.m_block = nullptr;
      }
      return *this;
    }

    // Copy constructor
    Shared(const Shared &other) : m_block(other.m_block), m_ptr(other.m_ptr)
    {
      if (m_block)
      {
        ++(m_block->refCount);
      }
    }

    // Copy assignment operator
    Shared &operator=(const Shared &other)
    {
      if (this != &other)
      {
        releaseResources();
        m_ptr = other.m_ptr;
        m_block = other.m_block;
        if (m_block)
        {
          ++(m_block->refCount);
        }
      }
      return *this;
    }

    /**
     * @brief Get the raw pointer
     *
     * @return T*
     */
    T *get()
    {
      return m_ptr;
    }

    /**
     * @brief Get a reference on pointed data
     *
     * @return T&
     */
    T &operator*()
    {
      if (m_ptr)
      {
        return *m_ptr;
      }
      throw std::runtime_error("Null pointer exception");
    }

    /**
     * @brief Get the raw pointer
     *
     * @return T*
     */
    T *operator->()
    {
      return m_ptr;
    }

    /**
     * @brief Get the reference count
     *
     * @return std::size_t
     */
    std::size_t count() const
    {
      return m_block ? m_block->refCount : 0;
    }

    /**
     * @brief Check if the raw pointer exists
     *
     * @return bool
     */
    bool exists() const
    {
      return m_ptr != nullptr;
    }

    /**
     * @brief Check if the raw pointer exists
     *
     * @return bool
     */
    operator bool() const
    {
      return exists();
    }

    /**
     * @brief make a shared pointer
     *
     * @note usage example: sp::Shared<T> uniquePtr = sp::Shared<T>::makeShared<T>(*T);
     */
    template <typename... Args>
    static Shared makeShared(Args &&...args)
    {
      return Shared(new T(std::forward<Args>(args)...));
    }

    /**
     * @brief Get a weak pointer from the shared pointer
     *
     * @return Weak<T>
     */
    void reset()
    {
      releaseResources();
      m_ptr = nullptr;
      m_block = nullptr;
    }

  private:
    template <typename U>
    friend class Weak; // Allow Weak to access private members
    BlockControl *m_block = nullptr;
    T *m_ptr = nullptr;

    /**
     * @brief Release the resources
    */
    void releaseResources()
    {
      if (m_block && --(m_block->refCount) == 0)
      {
        delete m_ptr;
        if (m_block->weakCount == 0)
        {
          delete m_block;
        }
      }
    }

    /**
     * @brief Private constructor
     */
    Shared(T *ptr, BlockControl *block) : m_block(block), m_ptr(ptr)
    {
      if (m_block)
      {
        ++m_block->refCount;
      }
    }
  };

} // namespace sp

#endif // SP_SHARED_H
