# ==============================================================================
# nLEap CMake File
# ==============================================================================

# program objects --------------------------------------------------------------
SET(NLEAP_SRC
        main.cpp
        nleap.cpp
        options.cpp
        commands.cpp
        )

IF(UNIX)
    ADD_DEFINITIONS(-DHAVE_READLINE)
    SET(READLINE_LIB readline)
ENDIF(UNIX)

# final build ------------------------------------------------------------------
ADD_EXECUTABLE(nleap-cli ${NLEAP_SRC})

TARGET_LINK_LIBRARIES(nleap-cli
                nleapcmds_shared
                nleap_shared
                obcore
                asl
                cscimafic
                hipoly
                ${READLINE_LIB}
                )

INSTALL(TARGETS
            nleap-cli
        DESTINATION
            bin
        )
