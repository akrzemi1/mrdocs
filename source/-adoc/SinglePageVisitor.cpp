//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (c) 2023 Vinnie Falco (vinnie.falco@gmail.com)
//
// Official repository: https://github.com/cppalliance/mrdox
//

#include "SinglePageVisitor.hpp"
#include <mrdox/Support/unlock_guard.hpp>

namespace clang {
namespace mrdox {
namespace adoc {

void
SinglePageVisitor::
operator()(auto const& I)
{
    renderPage(I, numPages_++);
    if constexpr(
            I.isNamespace() ||
            I.isRecord())
        corpus_.traverse(I, *this);
}

// Launch a task to render the page
// pageNumber is zero-based
//
void
SinglePageVisitor::
renderPage(
    auto const& I,
    std::size_t pageNumber)
{
    ex_.async(
        [this, &I, pageNumber](Builder& builder)
        {
            auto pageText = builder(I);
            if(! pageText)
                throw pageText.getError();
            endPage(std::move(*pageText), pageNumber);
        });
}

// pageNumber is zero-based
void
SinglePageVisitor::
endPage(
    std::string pageText,
    std::size_t pageNumber)
{
    std::unique_lock<std::mutex> lock(mutex_);

    if(pageNumber > topPage_)
    {
        // defer this page
        if( pages_.size() <= pageNumber)
            pages_.resize(pageNumber + 1);
        pages_[pageNumber] = std::move(pageText);
        return;
    }

    // write contiguous pages
    for(;;)
    {
        {
            unlock_guard unlock(mutex_);
            os_.write(pageText.data(), pageText.size());
            ++pageNumber;
        }
        topPage_ = pageNumber;
        if(pageNumber >= pages_.size())
            return;
        if(! pages_[pageNumber])
            return;
        pageText = std::move(*pages_[pageNumber]);
        // VFALCO this is in theory not needed but
        // I am paranoid about the std::move of the
        // string not resulting in a deallocation.
        pages_[pageNumber].reset();
    }
}

#define DEFINE(T) template void \
    SinglePageVisitor::operator()<T>(T const&)

DEFINE(NamespaceInfo);
DEFINE(RecordInfo);
DEFINE(FunctionInfo);
DEFINE(EnumInfo);
DEFINE(TypedefInfo);
DEFINE(VariableInfo);
DEFINE(FieldInfo);
DEFINE(SpecializationInfo);

} // adoc
} // mrdox
} // clang
