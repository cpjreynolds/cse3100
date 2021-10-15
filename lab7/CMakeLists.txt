add_executable(analyze main.c analyze.c)
add_executable(wgethtml wgethtml.c analyze.c)
add_executable(wgettext wgettext.c analyze.c)
