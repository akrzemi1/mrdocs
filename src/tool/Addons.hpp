//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (c) 2023 Vinnie Falco (vinnie.falco@gmail.com)
//
// Official repository: https://github.com/cppalliance/mrdocs
//

#ifndef MRDOCS_TOOL_ADDONS_HPP
#define MRDOCS_TOOL_ADDONS_HPP

#include <mrdocs/Support/Error.hpp>
#include <llvm/Support/CommandLine.h>
#include <string>

namespace clang {
namespace mrdocs {

/** Set the addons directory using the argument as a hint.

    @return The error if any occurred.
*/
Error
setupAddonsDir(
    llvm::cl::opt<std::string>& addonsDirArg,
    char const* argv0,
    void* addressOfMain);

} // mrdocs
} // clang

#endif
