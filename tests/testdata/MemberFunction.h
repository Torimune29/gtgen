template <typename T>
class foo {
 public:
    /// const
    void const_function() const;
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
    /// delete (unsupported)
    void deleted() = delete;
  /// scope test
  private:
    void private_function();
    void private_function_2();
  protected:
    void protected_function_2();
  private:
  public:
    void public_function();
};
/// definition
template <typename T>
void foo<T>::const_function() const {}
/// specialization
class bar : foo<int> {
 public:
    /// virtual void g() override;
    void virtual_function() override;
    /// virtual void h() override final;
    virtual auto pure_virtual() const -> void override final ;
};


/// struct test
struct baz {
    void struct_function();
};