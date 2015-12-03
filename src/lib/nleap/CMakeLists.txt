# ==============================================================================
# nLEaP CMake File
# ==============================================================================

SET(LEAP_SOURCE
        NLEaPMainHeader.cpp

    # core ---------------------------------------
        core/Key.cpp
        core/PredefinedKeys.cpp
        core/Property.cpp
        core/PropertyMap.cpp
        core/Entity.cpp
        core/ForwardIterator.cpp
        core/RecursiveIterator.cpp

    # engine -------------------------------------
        engine/Command.cpp
        engine/Parser.cpp
        engine/Context.cpp

    # types --------------------------------------
        types/Factory.cpp
        types/Database.cpp
        types/Variable.cpp
        types/String.cpp
        types/Number.cpp
        types/List.cpp
        types/Unit.cpp
        types/Residue.cpp
        types/Atom.cpp
        types/Bond.cpp
        types/AmberFF.cpp
        types/AtomTypes.cpp
        types/PDBAtomMap.cpp
        types/PDBResMap.cpp

    # ASL mask -----------------------------------
        mask/maskparser/MaskParser.cpp
        mask/maskparser/Mask.tab.c
        mask/maskparser/Mask.yy.c
        mask/NLAtom.cpp
        mask/NLResidue.cpp
        mask/NLTopology.cpp
        mask/NLMaskSelection.cpp
        mask/NLMask.cpp

    # format -------------------------------------
        format/CommonIO.cpp
        format/AmberParams.cpp
        format/AmberParm.cpp
        format/AmberPrep.cpp
        format/AmberOFF.cpp
        format/FormatOB.cpp
        format/FormatPDB.cpp
        format/SybylMol2.cpp

    # misc ---------------------------------------
        misc/Geometry.cpp
        )

IF(WIN32)
    SET(LEAP_SOURCE ${LEAP_SOURCE} engine/prefix_win32.cpp)
ENDIF(WIN32)

IF(UNIX)
    SET(LEAP_SOURCE ${LEAP_SOURCE} engine/prefix_unix.c)
ENDIF(UNIX)

ADD_DEFINITIONS(-DNLEAP_BUILDING_DLL)

ADD_LIBRARY(nleap_shared SHARED ${LEAP_SOURCE})

SET_TARGET_PROPERTIES(nleap_shared PROPERTIES
                        OUTPUT_NAME nleap
                        CLEAN_DIRECT_OUTPUT 1
                        VERSION ${NLEAP_VERSION})

TARGET_LINK_LIBRARIES(nleap_shared
                ${ASL_LIB_NAME}
                ${OPEN_BABEL_LIB}
                ${SCIMAFIC_CLIB_NAME}
                ${HIPOLY_LIB_NAME}
                ${SYSTEM_LIBS}
                )

INSTALL(TARGETS
            nleap_shared
        DESTINATION
            lib
        )

