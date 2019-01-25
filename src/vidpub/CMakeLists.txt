add_executable (vidpub_node main.cxx)

target_link_libraries (vidpub_node
                        ${catkin_LIBRARIES}
                        ${OpenCV_LIBRARIES}
)
