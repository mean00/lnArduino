include(FindPatch)

# the patch file is absolute 
MACRO(APPLY_PATCH_IF_NEEDED3 markerFile absPatchFile absSubdir description)
  MESSAGE(STATUS "Checking for ${absSubdir}/${markerFile}")
  IF(NOT EXISTS "${absSubdir}/${markerFile}")
      #MESSAGE(STATUS "   Patching file in ${subdir} ${description}")
      #MESSAGE(STATUS "      patch_file_p(1::: ${absSubdir} <= ${absPatchFile}")
      patch_file_p(1 "${absSubdir}" "${absPatchFile}")
      file(WRITE "${absSubdir}/${markerFile}" "patched")
  ELSE()
    MESSAGE(STATUS "   Already patched")
  ENDIF()
ENDMACRO()
