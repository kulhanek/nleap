#ifndef NLEaPMainHeaderH
#define NLEaPMainHeaderH
// =============================================================================
// nLEaP - A molecular manipulation program and coding environment
// -----------------------------------------------------------------------------
//     Copyright (C) 2011 Petr Kulhanek, kulhanek@chemi.muni.cz
//
//     This program is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License along
//     with this program; if not, write to the Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// =============================================================================

#define NLEAP_VERSION "1.0.SVNVERSION (DATE) [nLEaP]"

//------------------------------------------------------------------------------

extern const char* LibBuildVersion_NLEAP;

//------------------------------------------------------------------------------

#if defined _WIN32 || defined __CYGWIN__
#ifdef NLEAP_BUILDING_DLL
#ifdef __GNUC__
#define NLEAP_DLL_PUBLIC __attribute__((dllexport))
#else
#define NLEAP_DLL_PUBLIC __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define NLEAP_DLL_PUBLIC __attribute__((dllimport))
#else
#define NLEAP_DLL_PUBLIC __declspec(dllimport)
#endif
#define NLEAP_DLL_LOCAL
#endif
#else
#if __GNUC__ >= 4
#define NLEAP_DLL_PUBLIC __attribute__ ((visibility("default")))
#define NLEAP_DLL_LOCAL  __attribute__ ((visibility("hidden")))
#else
#define NLEAP_DLL_PUBLIC
#define NLEAP_DLL_LOCAL
#endif
#endif

#define NLEAP_PACKAGE NLEAP_DLL_PUBLIC

//------------------------------------------------------------------------------

#endif
