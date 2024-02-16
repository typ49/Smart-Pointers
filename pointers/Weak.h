#ifndef SP_WEAK_H
#define SP_WEAK_H

#include "Shared.h" // Ensure this includes the updated Shared.h with BlockControl

namespace sp
{

  /**
   * @brief Smart weak pointer
   */
  template <typename T>
  class Weak
  {
  public:
    // Default constructor
    Weak() : m_ptr(nullptr), m_block(nullptr) {}

    // Constructor takes a Shared pointer
    Weak(const Shared<T> &shared) : m_ptr(shared.m_ptr), m_block(shared.m_block)
    {
      if (m_block)
      {
        ++(m_block->weakCount); // Increment the weak count
      }
    }

    // Destructor
    ~Weak()
    {
      releaseResources();
    }

    // Copy constructor
    Weak(const Weak &other) : m_ptr(other.m_ptr), m_block(other.m_block)
    {
      if (m_block)
      {
        ++(m_block->weakCount);
      }
    }

    // Copy assignment operator
    Weak &operator=(const Weak &other)
    {
      if (this != &other)
      {
        releaseResources();
        m_ptr = other.m_ptr;
        m_block = other.m_block;
        if (m_block)
        {
          ++(m_block->weakCount);
        }
      }
      return *this;
    }

    // Move constructor
    Weak(Weak &&other) noexcept : m_ptr(other.m_ptr), m_block(other.m_block)
    {
      other.m_ptr = nullptr;
      other.m_block = nullptr;
    }

    // Move assignment operator
    Weak &operator=(Weak &&other) noexcept
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

    // Get a Shared pointer from the Weak pointer
    Shared<T> lock()
    {
      if (m_block && m_block->refCount > 0)
      {
        // Using a private constructor of Shared that accepts (T*, BlockControl*)
        return Shared<T>(m_ptr, m_block);
      }
      else
      {
        return Shared<T>();
      }
    }

    // Check if the Weak pointer is expired
    bool expired() const
    {
      return m_block == nullptr || m_block->refCount == 0;
    }


    /**
     * @brief Reset the Weak pointer
    */
    void reset()
    {
      releaseResources();
    }

  private:
    T *m_ptr;
    BlockControl *m_block;


    /**
     * @brief Release resources
     */
    void releaseResources()
    {
      if (m_block && --(m_block->weakCount) == 0 && m_block->refCount == 0)
      {
        delete m_block; // Delete the BlockControl if both refCount and weakCount are 0
      }
      m_ptr = nullptr;
      m_block = nullptr;
    }
  };

} // namespace sp

#endif // SP_WEAK_H