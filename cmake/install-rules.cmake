install(
    TARGETS perception_exe
    RUNTIME COMPONENT perception_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
