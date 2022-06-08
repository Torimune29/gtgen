# --------------------------------------------------------------------------------
#                         Documentation (no change needed).
# --------------------------------------------------------------------------------
# Add a make target 'doc' to generate API documentation with Doxygen.
# You should set options to your liking in the file 'Doxyfile.in'.
add_custom_target(notice
    # rm -rf NOTICE && find ${CMAKE_CURRENT_BINARY_DIR}/_deps -name "LICENSE*" | xargs -I {} sh -c "echo '\\n\\n--\\n$(direname {} | awk -F \'/\' \'{ print $NF }\' | cut -f1 -d \'-\')' >> NOTICE cat {} >> NOTICE && echo '\\n\\n---\\n' >> NOTICE"
    rm -rf NOTICE && find ${CMAKE_CURRENT_BINARY_DIR}/_deps -name "LICENSE*" | xargs -I {} sh -c "echo '\\n--\\n' >> NOTICE && basename \$(dirname {}) | cut -f1 -d '-' >> NOTICE && echo '\\n---\\n' >> NOTICE && cat {} >> NOTICE"
    # rm -rf NOTICE  && find ${CMAKE_CURRENT_BINARY_DIR}/_deps -name "LICENSE*" | xargs cat >> NOTICE
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "${BoldMagenta}Generating API documentation with Doxygen (open ./html/index.html to view).${ColourReset}" VERBATIM
)

