
# gtgen

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/Torimune29/gtgen/CI)
[![codecov](https://codecov.io/gh/Torimune29/gtgen/branch/main/graph/badge.svg)](https://codecov.io/gh/Torimune29/gtgen)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4ab150dd86c44db9ba17df846aa309a3)](https://www.codacy.com/gh/Torimune29/gtgen/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Torimune29/dev-cpp-oss-template&amp;utm_campaign=Badge_Grade)
![CodeQL](https://github.com/Torimune29/gtgen/workflows/CodeQL/badge.svg)

## How to Build

- Requirement
  - libclang-header (like libclang-dev on Ubuntu20.04)

## How to Use

- Sample Output

```
~gtgen/build$ ./gtgen -f ../include/FunctionParser.h -p .
gtgen version: 0.0.1
[simple file parser] [info] parsing file '../include/FunctionParser.h'
MOCK_METHOD0(Ready, bool());
MOCK_METHOD0(GetFunction, std::vector<FunctionInfo>());
MOCK_METHOD0(GetMemberFunction, std::vector<MemberFunctionInfo>());

~gtgen/build$ ./gtgen -f ../include/FunctionParser.h -p . 2>/dev/null
MOCK_METHOD0(Ready, bool());
MOCK_METHOD0(GetFunction, std::vector<FunctionInfo>());
MOCK_METHOD0(GetMemberFunction, std::vector<MemberFunctionInfo>());

~gtgen/build$ ./gtgen -f ../include/FunctionParser.h -p . --view-only
gtgen version: 0.0.1
[simple file parser] [info] parsing file '../include/FunctionParser.h'
{
    "function": [],
    "memberFunction": [
        {
            "accessSpecifier": 0,
            "className": "FunctionParser",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "Ready",
            "noexcept": true,
            "parameterNum": 0,
            "polymorphicMemberFunction": true,
            "returnType": "bool",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "FunctionParser",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "GetFunction",
            "noexcept": true,
            "parameterNum": 0,
            "polymorphicMemberFunction": false,
            "returnType": "std::vector<FunctionInfo>",
            "signature": "()"
        },
        {
            "accessSpecifier": 0,
            "className": "FunctionParser",
            "constMemberFunction": false,
            "consteval": false,
            "constexpr": false,
            "functionName": "GetMemberFunction",
            "noexcept": true,
            "parameterNum": 0,
            "polymorphicMemberFunction": false,
            "returnType": "std::vector<MemberFunctionInfo>",
            "signature": "()"
        }
    ]
}

```

## How to Develop

This uses a OSS Development Template for C++ projects based on [cpp-project](https://github.com/bsamseth/cpp-project), and [dev-cpp-oss-template](https://github.com/Torimune29/dev-cpp-oss-template).
