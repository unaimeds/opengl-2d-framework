#pragma once

// this is a precompiled header, already included in all files

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;

using i16 = short;
using i32 = int;

using f32 = float;

template <typename T>
using cref = const T&;

// todo runtime_exception alias and format function
