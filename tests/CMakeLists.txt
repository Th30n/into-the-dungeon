set(MAIN_SRC_DIR ${ITD++_SOURCE_DIR}/src)

set(SERIALIZATION_IML_SOURCES serialization/iml/test.cpp
  ${MAIN_SRC_DIR}/iml/IMLComment.cpp ${MAIN_SRC_DIR}/iml/IMLNode.cpp
  ${MAIN_SRC_DIR}/iml/IMLParser.cpp ${MAIN_SRC_DIR}/iml/IMLTag.cpp
  ${MAIN_SRC_DIR}/iml/IMLValue.cpp ${MAIN_SRC_DIR}/iml/utils.cpp)

add_executable(serialization-iml-test ${SERIALIZATION_IML_SOURCES})

add_test(NAME SERIALIZATION_IML_TEST COMMAND serialization-iml-test)
