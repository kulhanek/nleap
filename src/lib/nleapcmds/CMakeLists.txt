# ==============================================================================
# nLEaP CMake File
# ==============================================================================

INCLUDE_DIRECTORIES(context SYSTEM)
INCLUDE_DIRECTORIES(control SYSTEM)
INCLUDE_DIRECTORIES(input SYSTEM)
INCLUDE_DIRECTORIES(openbabel SYSTEM)
INCLUDE_DIRECTORIES(output SYSTEM)
INCLUDE_DIRECTORIES(properties SYSTEM)

SET(NLEAP_CMDS_SOURCE

    # context commands -----------------
        context/Alias.cpp
        context/Help.cpp
        context/History.cpp
        context/LoadState.cpp
        context/LogFile.cpp
        context/NLeap.cpp
        context/Redo.cpp
        context/SaveState.cpp
        context/Source.cpp
        context/Undo.cpp

    # control commands -----------------
        control/Add.cpp
        control/AddAtomTypes.cpp
        control/AddPDBAtomMap.cpp
        control/AddPDBResMap.cpp
        control/AmberFF.cpp
        control/Assign.cpp
        control/BondByDistance.cpp
        control/Bond.cpp
        control/Check.cpp
        control/Clear.cpp
        control/Combine.cpp
        control/Copy.cpp
        control/CreateAtom.cpp
        control/CreateResidue.cpp
        control/CreateUnit.cpp
        control/DeleteBond.cpp
        control/Desc.cpp
        control/Release.cpp
        control/List.cpp
        control/Remove.cpp
        control/Sequence.cpp
        control/Set.cpp

    # input commands -------------------
        input/LoadAmberParams.cpp
        input/LoadAmberPrep.cpp
        input/LoadMol2.cpp
        input/LoadOFF.cpp
        input/LoadPDB.cpp

    # openbabel commands -------------------
        openbabel/ListOBFormats.cpp
        openbabel/LoadOB.cpp
        openbabel/SaveOB.cpp

    # output commands ------------------
        output/SaveAmberParams.cpp
        output/SaveAmberParm.cpp
        output/SaveGaussian.cpp
        output/SaveGromacs.cpp
        output/SaveGromos.cpp
        output/SaveMol2.cpp
        output/SavePDB.cpp

    # solvent/PBC commands -------------
#        shell.cpp
#        solvate.cpp
#        addions.cpp

    # geometry commands ----------------
        geometry/AlignAxes.cpp
        geometry/Center.cpp
        geometry/MeasureGeom.cpp
        geometry/Translate.cpp

    # properties commands --------------
        properties/Charge.cpp
        properties/Dipole.cpp
        properties/Energy.cpp

    # antechamber commands -------------
#        moloper.cpp
        )

ADD_LIBRARY(nleapcmds_shared SHARED ${NLEAP_CMDS_SOURCE})

SET_TARGET_PROPERTIES(nleapcmds_shared PROPERTIES
                        OUTPUT_NAME nleapcmds
                        CLEAN_DIRECT_OUTPUT 1
                        VERSION ${NLEAP_VERSION})

TARGET_LINK_LIBRARIES(nleapcmds_shared
                nleap
                ${ASL_LIB_NAME}
                ${OPEN_BABEL_LIB}
                ${SCIMAFIC_CLIB_NAME}
                ${HIPOLY_LIB_NAME}
                ${SYSTEM_LIBS}
                )

INSTALL(TARGETS
            nleapcmds_shared
        DESTINATION
            lib
        )

