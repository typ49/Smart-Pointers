#ifndef SP_UNIQUE_H
#define SP_UNIQUE_H

namespace sp
{

  /**
   * @brief Smart unique pointer
   */
  template <typename T>
  class Unique
  {
  public:
    /**
     * @brief Constructor takes a dynamic pointer
     */
    Unique(T *ptr = nullptr)
        : m_ptr(ptr)
    {
    }

    /**
     * @brief Destructor
     */
    ~Unique()
    {
      delete m_ptr;
    }

    Unique(const Unique&) = delete;
    Unique& operator=(const Unique&) = delete;

    /**
     * @brief Move constructor
     */
    Unique(Unique &&other) noexcept
    : m_ptr(other.m_ptr)
    {
      other.m_ptr = nullptr;
    }

    /**
     * @brief Move assignment
     */
    Unique &operator=(Unique &&other) noexcept
    {
      if (this != &other)
      {
        delete m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
      }
      return *this;
    }

    /**
     * @brief Get the raw pointer
     */
    T *get()
    {
      return m_ptr;
    }

    /**
     * @brief Get a reference on pointed data
     */
    T &operator*()
    {
      return *m_ptr;
    }

    /**
     * @brief Get the raw pointer
     */
    T *operator->()
    {
      return m_ptr;
    }

    /**
     * @brief Check if the raw pointer exists
     */
    bool exists() const
    {
      return m_ptr != nullptr;
    }

  private:
    // implementation defined
    T *m_ptr;
  };
}

#endif // SP_UNIQUE_H
