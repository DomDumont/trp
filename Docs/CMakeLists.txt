

function(cat IN_FILE OUT_FILE)
  file(READ ${IN_FILE} CONTENTS)
  file(APPEND ${OUT_FILE} "${CONTENTS}")
endfunction()

set(DOC_FILES)
set(DOC_FILES ${DOC_FILES} en/Home.md)
set(DOC_FILES ${DOC_FILES} en/Installation.md)

# Prepare a temporary file to "cat" to:
file(WRITE docs.md.temp "")

# Call the "cat" function for each input file
foreach(DOC_FILE ${DOC_FILES})
  MESSAGE(" Add " ${DOC_FILE})
  cat(${DOC_FILE} docs.md.temp)
endforeach()

configure_file(docs.md.temp docs.md COPYONLY)

add_custom_command(
      OUTPUT docs.md
      DEPENDS ${DOC_FILES}
    )


add_custom_command(
      OUTPUT docs.html
      COMMAND pandoc docs.md -o docs.html
      DEPENDS ${DOC_FILES}
    )
add_custom_target(TRPDOCS DEPENDS docs.html docs.md)
