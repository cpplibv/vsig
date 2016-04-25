// File:   tag.hpp Author: Vader Created on 2016. április 23., 6:07

#pragma once

namespace libv {

// tag ---------------------------------------------------------------------------------------------

namespace tag {

struct accumulator {};
struct thread_policy {};
struct history_size {};

} //namespace tag

// -------------------------------------------------------------------------------------------------

template <typename... Tags>
struct tag_type {
};

// -------------------------------------------------------------------------------------------------

} //namespace libv