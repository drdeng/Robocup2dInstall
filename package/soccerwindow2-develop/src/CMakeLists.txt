
add_subdirectory(model)
add_subdirectory(qt)

configure_file(sswindow2.in sswindow2)

install(PROGRAMS sswindow2
  DESTINATION bin)
