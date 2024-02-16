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
     * @brief Destructor
     */
    ~Unique()
    {
      delete m_ptr;
    }

    // Non-copyable
    Unique(const Unique &) = delete;
    Unique &operator=(const Unique &) = delete;

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

    /**
     * @brief Check if the raw pointer exists
     */
    operator bool() const
    {
      return exists();
    }

    /**
   * @brief make a unique pointer
   *
   * @note usage : sp::Unique<T> uniquePtr = sp::Unqiue<T>::makeUnique<T>(*T);
   */
    template <typename... Args>
    static Unique makeUnique(Args &&...args)
    {
      return Unique(new T(std::forward<Args>(args)...));
    }

    void reset()
    {
      delete m_ptr;
      m_ptr = nullptr;
    }

  private:
    // implementation defined
    T *m_ptr;
  };
}

#endif // SP_UNIQUE_H
