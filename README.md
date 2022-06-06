
# gtgen

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/bfkwl2vu4eom/gtgen/CI)
[![codecov](https://codecov.io/gh/bfkwl2vu4eom/gtgen/branch/main/graph/badge.svg)](https://codecov.io/gh/bfkwl2vu4eom/gtgen)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4ab150dd86c44db9ba17df846aa309a3)](https://www.codacy.com/gh/bfkwl2vu4eom/gtgen/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=bfkwl2vu4eom/dev-cpp-oss-template&amp;utm_campaign=Badge_Grade)
![CodeQL](https://github.com/bfkwl2vu4eom/gtgen/workflows/CodeQL/badge.svg)

## How to Build

- Requirement
  - libclang-header (like libclang-dev on Ubuntu20.04)

## How to Use

- Sample Output

```
~/gtget/build$ ./gtgen -f ../tests/testdata/MemberFunction.h -p .
gtgen version: 1.2.3.4
file: ../tests/testdata/MemberFunction.h,
settings:
  compile_database_path: .
[simple file parser] [info] parsing file '../tests/testdata/MemberFunction.h'
{
    "function": [],
    "memberFunction": [
        {
            "accessSpecifier": 0,
            "className": "foo",
            "constMemberFunction": true,
            "consteval": false,
            "constexpr": false,
            "functionName": "const_function",
            "noexcept": false,
            "polymorphicMemberFunction": false,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "foo",
            "constMemberFunction": true,
            "consteval": false,
            "constexpr": false,
            "functionName": "full_suffix",
            "noexcept": true,
            "polymorphicMemberFunction": false,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "foo",
            "constMemberFunction": true,
            "consteval": false,
            "constexpr": false,
            "functionName": "noise_suffix",
            "noexcept": false,
            "polymorphicMemberFunction": false,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "foo",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "virtual_function",
            "noexcept": false,
            "polymorphicMemberFunction": true,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "foo",
            "constMemberFunction": true,
            "consteval": false,
            "constexpr": false,
            "functionName": "pure_virtual",
            "noexcept": false,
            "polymorphicMemberFunction": true,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "foo",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "with_definition",
            "noexcept": false,
            "polymorphicMemberFunction": false,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "foo",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "deleted",
            "noexcept": false,
            "polymorphicMemberFunction": false,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 1,
            "className": "foo",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "private_function",
            "noexcept": false,
            "polymorphicMemberFunction": false,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 1,
            "className": "foo",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "private_function_2",
            "noexcept": false,
            "polymorphicMemberFunction": false,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 2,
            "className": "foo",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "protected_function",
            "noexcept": false,
            "polymorphicMemberFunction": false,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "foo",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "public_function",
            "noexcept": false,
            "polymorphicMemberFunction": false,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "bar",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "virtual_function",
            "noexcept": false,
            "polymorphicMemberFunction": true,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "bar",
            "constMemberFunction": true,
            "consteval": false,
            "constexpr": false,
            "functionName": "pure_virtual",
            "noexcept": false,
            "polymorphicMemberFunction": true,
            "returnType": "void",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "baz",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "struct_function",
            "noexcept": false,
            "polymorphicMemberFunction": false,
            "returnType": "void",
            "signature": "()"
        }
    ]
}
```

## How to Develop

This uses a OSS Development Template for C++ projects based on [cpp-project](https://github.com/bsamseth/cpp-project), and [dev-cpp-oss-template](https://github.com/bfkwl2vu4eom/dev-cpp-oss-template).
