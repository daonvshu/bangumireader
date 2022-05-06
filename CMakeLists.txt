cmake_minimum_required (VERSION 3.8)

project ("mikanbangumireaderproject")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(bugreport)

add_subdirectory(3rdparty/qtdao/src)
add_subdirectory(3rdparty/bugreportclient/lib)