// from cppast/test/cpp_member_function.cpp

template <typename T>
struct foo
{
    /// void a(int array[]);
    void a(int array[]); // throw in an array argument for good measure
    /// void b()noexcept;
    void b() noexcept;
    /// void c()const;
    void c() const;
    /// void d()const volatile noexcept;
    auto d() const volatile noexcept -> void;
    /// void e()&;
    void e() &;
    /// void f()const volatile&&;
    void f() const volatile &&;
    /// virtual void g();
    virtual void g();
    /// virtual void h()=0;
    virtual void h() = 0;
    /// void i();
    void i() {}
    /// void j()=delete;
    void j() = delete;
};
/// void foo<T>::a(int array[]);
template <typename T>
void foo<T>::a(int array[]) {}
struct bar : foo<int>
{
    /// virtual void g() override;
    void g();
    /// virtual void h() override final;
    virtual auto h() -> void override final;
};