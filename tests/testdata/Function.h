// from cppast/test/cpp_function.cpp

// normal
void normal();
// suffix test
// noexcept conditions
void noexcept_only() noexcept;
void noexcept_false() noexcept(false);
void noexcept_complex() noexcept(noexcept(noexcept_only()));
// prefix test
/// extern
extern void extern_function();
/// static
static void static_function();
/// constexpr
constexpr void constexpr_function();
/// static constexpr
static constexpr void static_constexpr();
// namespace
namespace ns {
    void namespace_deleted() = delete;
    void namespace_normal();
    using m = int;
}
/// namespace definition
void ns::namespace_normal() {
}
/// ns::m namespace_type_return();
ns::m namespace_type_return();
