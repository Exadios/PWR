/*
 * \section COPYRIGHT
 *
 * Passive Weather Radar (PWR).
 * Copyright (C) 2010-2020 Peter F Bradshaw
 * A detailed list of copyright holders can be found in the file "AUTHORS". 
 *
 * \section LICENSE
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define LTE_IMPORT __declspec(dllimport)
  #define LTE_EXPORT __declspec(dllexport)
  #define LTE_LOCAL
#else
  #if __GNUC__ >= 4
    #define LTE_IMPORT __attribute__ ((visibility ("default")))
    #define LTE_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define LTE_IMPORT
    #define LTE_EXPORT
    #define LTE_LOCAL
  #endif
#endif

// Define LTE_API
// LTE_API is used for the public API symbols.
#ifdef LTE_DLL_EXPORTS // defined if we are building the LTE DLL (instead of using it)
  #define LTE_API LTE_EXPORT
#else
  #define LTE_API LTE_IMPORT
#endif


// Useful macros for templates
#define CONCAT(a, b) a##b
#define CONCAT2(a, b) CONCAT(a, b)

#define STRING2(x) #x
#define STRING(x) STRING2(x)


// Common error codes
#define LTE_SUCCESS                 0
#define LTE_ERROR                  -1
#define LTE_ERROR_INVALID_INPUTS   -2

// Cf_t definition
typedef _Complex float Cf_t;

#ifdef ENABLE_C16
typedef _Complex short int C16_t;
#endif /* ENABLE_C16 */

#endif // __CONFIG_H__
