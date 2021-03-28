/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file linux.h

#ifndef SEELE_REGISTRATION_H
#define SEELE_REGISTRATION_H

#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "script/script_manager.h"

#define __SEELE_CAT(__X__, __Y__) __X__##__Y__
#define SEELE_CAT(__X__, __Y__) __SEELE_CAT(__X__, __Y__)

#define SEELE_REGISTRATION(__NAME__) \
static void SEELE_CAT(__REGISTER_, __NAME__) (); \
namespace {                          \
struct SEELE_CAT(__NAME__, __REGISTTRATION_) {     \
    SEELE_CAT(__NAME__, __REGISTTRATION_) () { \
          SEELE_CAT(__REGISTER_, __NAME__) ();                       \
    }                               \
};                               \
}                                    \
static const SEELE_CAT(__NAME__, __REGISTTRATION_) SEELE_CAT(__NAME__, __registration); \
static void SEELE_CAT(__REGISTER_, __NAME__) ()

#endif //SEELE_REGISTRATION_H
