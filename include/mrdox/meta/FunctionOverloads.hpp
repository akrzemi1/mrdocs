//
// This is a derivative work. originally part of the LLVM Project.
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (c) 2023 Vinnie Falco (vinnie.falco@gmail.com)
//
// Official repository: https://github.com/cppalliance/mrdox
//

#ifndef MRDOX_META_FUNCTIONOVERLOADS_HPP
#define MRDOX_META_FUNCTIONOVERLOADS_HPP

#include <mrdox/meta/Function.hpp>
#include <clang/Basic/Specifiers.h>
#include <llvm/ADT/StringRef.h>

namespace clang {
namespace mrdox {

class Corpus;
struct Scope;

/** A list of overloads for one function name.
*/
struct FunctionOverloads
{
    llvm::StringRef name;
    std::vector<FunctionInfo const*> list;
};

/** A set of unique function names in a scope
*/
struct FunctionOverloadsSet
{
    /** The access control of this scope.
    */
    AccessSpecifier access;

    /** The list of function overloads in the scope.
    */
    std::vector<FunctionOverloads> list;
};

FunctionOverloadsSet
makeFunctionOverloadsSet(
    Corpus const& corpus,
    Scope const& scope,
    AccessSpecifier access);

} // mrdox
} // clang

#endif