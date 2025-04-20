
add_library("sd-ibuffer" STATIC
        ibuffer.c
        ibuffer_write.c
        mnem.c
)

#Util dependency
target_link_libraries("sd-ibuffer" sd-util)
