// from cppast/test/cpp_function.cpp

/// void a();
void a();
/// int b(int a,float* b=nullptr);
int b(int a, float* b = nullptr);
/// auto c(decltype(42) a,...)->int(&)[10];
int (&c(decltype(42) a, ...))[10];
// noexcept conditions
/// void d()noexcept;
void d() noexcept;
/// void e()noexcept(false);
void e() noexcept(false);
/// void f()noexcept(noexcept(d()));
void f() noexcept(noexcept(d()));
// storage class + constexpr
/// extern void g();
extern void g();
/// static void h();
static void h();
/// constexpr void i();
constexpr void i();
/// static constexpr void j();
static constexpr void j();
// body
namespace ns
{
    /// void k()=delete;
    void k() = delete;
    /// void l();
    void l();
    using m = int;
}
/// void ns::l();
void ns::l()
{
    // might confuse parser
    auto b = noexcept(g());
}
/// ns::m m();
ns::m m();
/// void n(int i=int());
void n(int i = int());
