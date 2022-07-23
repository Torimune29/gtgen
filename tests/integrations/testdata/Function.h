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
void namespace_deleted() = delete;  // unsupported
void namespace_normal();
void namespace_normal_2();
using m = int;
}  // namespace ns
/// namespace definition (unsupported)
// void ns::namespace_normal() {
// }
/// ns::m namespace_type_return();
ns::m namespace_type_return();

/// function template
template <typename T, typename U, int I>
void t1(T val1, U *val2, int i = I) {
}
