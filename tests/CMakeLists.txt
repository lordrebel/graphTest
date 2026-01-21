function(add_test_executable name)
  add_executable(
    ${name}
    ${name}.cpp
  )
  add_test(NAME ${name} COMMAND ${name})
endfunction(add_test_executable)

add_test_executable(test_basic_graph)
add_test_executable(test_graph_walk)
add_test_executable(test_graph_check)
add_test_executable(test_toposort)
add_test_executable(test_mst)