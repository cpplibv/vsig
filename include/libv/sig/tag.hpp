// File: tag.hpp Author: Vader Created on 2016. április 23., 6:07

#pragma once

namespace libv {

// tag ---------------------------------------------------------------------------------------------

namespace tag {

struct accumulator {};
struct condition {};
struct history_size {};
struct routing_logic {};
struct signal {};
struct thread_policy {};
struct output {};

} //namespace tag

// -------------------------------------------------------------------------------------------------

template <typename... Tags>
struct tag_type {
};

// -------------------------------------------------------------------------------------------------

} //namespace libv