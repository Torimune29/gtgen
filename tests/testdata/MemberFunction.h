template <typename T>
class foo {
 public:
    /// const
    void const_function() const;
    /// volatile
    void volatile_function() volatile;
    /// const volatile noexcept -> void
    auto full_suffix() const volatile noexcept -> void;
    /// noise
    void noise_suffix() const &;
    /// virtual
    virtual void virtual_function();
    /// pure virtual
    virtual void pure_virtual() const = 0;
    /// with definition;
    void with_definition() {}
    /// delete
    void deleted() = delete;
  /// access specifier test
  private:
    void private_function();
    void private_function_2();
  protected:
    void protected_function();
  private:
  public:
    void public_function();
    /// overload check
    int overload(int);
    int overload(double);
    int overload(int) const;
    int overload(int) volatile;
    int overload(int) const volatile;
};
/// definition
template <typename T>
void foo<T>::const_function() const {}
/// specialization
class bar : foo<int> {
 public:
    /// virtual void g() override;
    virtual void virtual_function() override;
    /// virtual void h() override final;
    virtual auto pure_virtual() const -> void override final ;
};


/// struct test
struct baz {
    void struct_function();
};


/// inner class
class fizz {
 public:
  class buzz {
    void inner_class_func();
  };
};

/// namespace namespace class
namespace n3 {
  namespace n3_1 {
    class fizz {
     public:
      class buzz {
        void inner_class_func();
      };
    };
  }
  class fizz {
   public:
    class buzz {
      void inner_class_func();
    };
  };
}
