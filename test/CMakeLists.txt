# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(perceptionTests LANGUAGES CXX)

# ---- Tests ----

add_executable(perception_test source/perception_test.cpp)
target_link_libraries(perception_test PRIVATE perception_lib)
target_compile_features(perception_test PRIVATE cxx_std_17)

add_test(NAME perception_test COMMAND perception_test)

# ---- End-of-file commands ----

add_folders(Test)
