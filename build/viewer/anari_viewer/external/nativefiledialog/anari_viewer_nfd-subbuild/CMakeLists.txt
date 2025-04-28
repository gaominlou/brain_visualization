# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.22.1)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(anari_viewer_nfd-populate NONE)



include(ExternalProject)
ExternalProject_Add(anari_viewer_nfd-populate
                     "URL" "https://github.com/btzy/nativefiledialog-extended/archive/refs/tags/v1.2.1.zip" "DOWNLOAD_DIR" "/home/fgao/code/brain_visualization/.anari_deps/anari_viewer_nfd" "URL_MD5" "fe75247976f9a06d01218abb6f53ecca"
                    SOURCE_DIR          "/home/fgao/code/brain_visualization/build/deps/source/anari_viewer_nfd"
                    BINARY_DIR          "/home/fgao/code/brain_visualization/build/viewer/anari_viewer/external/nativefiledialog/anari_viewer_nfd-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


