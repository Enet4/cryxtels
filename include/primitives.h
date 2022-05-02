/** \file primitives.h
 *  This file is part of Crystal Pixels.
 *
 *  Crystal Pixels is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Crystal Pixels is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the program. If not, see <http://www.gnu.org/licenses/>.
 */
// Concise and useful primitive types to use throughout the code base.
#ifndef PRIMITIVES_H_INCLUDED
#define PRIMITIVES_H_INCLUDED

#include <cstdint>

/// 8-bit unsigned integer
using u8 = uint8_t;
/// 8-bit signed integer
using i8 = int8_t;
/// 16-bit unsigned integer
using u16 = uint16_t;
/// 16-bit signed integer
using i16 = int16_t;
/// 32-bit unsigned integer
using u32 = uint32_t;
/// 32-bit signed integer
using i32 = int32_t;

/// 32-bit floating point
using f32 = float;
/// 64-bit floating point
using f64 = double;

#endif
