# ==============================================================================
# nLEaP CMake File
# ==============================================================================

INCLUDE_DIRECTORIES(lib/nleap SYSTEM)
INCLUDE_DIRECTORIES(lib/nleapcmds SYSTEM)

# ------------------------------------------------

ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(bin)

