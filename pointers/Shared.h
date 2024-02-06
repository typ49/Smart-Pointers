#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>
#include <map>

namespace sp
{

  /**
   * @brief Smart shared pointer class
   */
  template <typename T>
  class Shared
  {
  public:
    /**
     * @brief Constructor takes a dynamic pointer
     * 
     * @param ptr
     * 
     * @note  The constructor should initialize the reference count to 1 if the pointer is not null, 
     *        and to 0 if the pointer is null.
     */
    Shared(T *ptr = nullptr)
    {
      m_refCount = new std::size_t(1); // m_refCount should be a pointer to std::size_t 
      // if ptr is not null, we create a new reference
      if (ptr)
      {
        m_ptr = ptr;            // m_ptr should be a pointer to T
      }
      else
      {
        m_ptr = nullptr;        // m_ptr should be a pointer to T
      }
    }

    /**
     * @brief Destructor
     */
    ~Shared()
    {
      // decrease the reference count and delete the pointer if m_refCount is 0
      if (--(*m_refCount) == 0)
      {
        delete m_ptr;      // delete the pointer
        delete m_refCount; // delete the reference count
      }
    }

    /**
     * @brief Copy constructor
     * 
     * @param other
     * 
     * @note  The copy constructor should increment the reference count of the shared pointer.
     */
    Shared(const Shared &other) : m_ptr(other.m_ptr), m_refCount(other.m_refCount)
    {
      // Increment the shared reference count.
      (*m_refCount)++;
    }

    /**
     * @brief Copy assignment operator
     * 
     * @param other
     * 
     * @note  The copy assignment operator should increment the reference count of the shared pointer.
     */
    Shared &operator=(const Shared &other)
    {
      if (this == &other)
      {
        return *this;
      }
      // if the current pointer is not the same as the other pointer
      else if (m_ptr != other.m_ptr)
      {
        // decrease the reference count and delete the pointer if m_refCount is 0
        if (--(*m_refCount) == 0)
        {
          delete m_ptr;      // delete the pointer
          delete m_refCount; // delete the reference count
        }

        // copy the other pointer and reference count
        m_ptr = other.m_ptr;
        m_refCount = other.m_refCount;

        // Increment the shared reference count.
        (*m_refCount)++;
      }
      return *this;
    }

    /**
     * @brief Get the raw pointer
     * 
     * @return T* the raw pointer
     */
    T *get()
    {
      return m_ptr;
    }

    /**
     * @brief Get a reference on pointed data
     * 
     * @return T& the reference on pointed data or throw an exception if the pointer is null
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
     * @return T* the raw pointer
     */
    T *operator->()
    {
      return m_ptr;
    }

    /**
     * @brief Get the number of Shared pointed on the current pointer
     * 
     * @return std::size_t the number of Shared pointed on the current pointer
     */
    std::size_t count() const
    {
      return *m_refCount;
    }

    /**
     * @brief  Check if the raw pointer exists
     * 
     * @return true if the raw pointer exists
     */
    bool exists() const
    {
      return m_ptr != nullptr;
    }

  private:
    // implementation defined
    std::size_t *m_refCount;
    T *m_ptr;
  };
}; // namespace sp

#endif // SP_SHARED_H
