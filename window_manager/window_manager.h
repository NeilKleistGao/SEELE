/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file window_manager.h

#ifndef SEELE_WINDOW_MANAGER_H
#define SEELE_WINDOW_MANAGER_H

#ifdef PLATFORM_LINUX
#include "linux.h"
#elif PLATFORM_WINDOWS
#include "windows.h"
#elif PLATFORM_MACOS
#include "mac_os.h"
#else
#error "THIS PLATFORM IS NOT SUPPORTED!"
#endif

namespace window_manager {
window_manager::Window* window_manager::Window::_instance = nullptr;
} // namespace window_manager

#endif //SEELE_WINDOW_MANAGER_H
