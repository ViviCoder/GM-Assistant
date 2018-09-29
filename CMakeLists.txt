#*************************************************************************
# Copyright © 2017-2018 Vincent Prat
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#************************************************************************

cmake_minimum_required (VERSION 3.1)

# project information
set (PROJECT_NAME GM-Assistant)
project (${PROJECT_NAME})

# handling version
set (VERSION_MAJOR 1)
set (VERSION_MINOR 2)
set (VERSION_PATCH 0)
configure_file(sources/engine/Version.h.in Version.h)

# options
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_AUTOUIC ON)

# source directory
include_directories(
    sources/engine
    sources/engine/items
    sources/engine/modifications
    sources/misc
    sources/widgets
    sources/windows)
set (PROJECT_SOURCES
    ressource.qrc
    sources/main.cpp
    sources/engine/Branch.cpp
    sources/engine/Character.cpp
    sources/engine/CharacterList.cpp
    sources/engine/FileMapping.cpp
    sources/engine/IOConfig.cpp
    sources/engine/Metadata.cpp
    sources/engine/PropertyList.cpp
    sources/engine/Scenario.cpp
    sources/engine/Tree.cpp
    sources/engine/Version.cpp
    sources/engine/items/FileItem.cpp
    sources/engine/items/ImageItem.cpp
    sources/engine/items/Item.cpp
    sources/engine/items/ItemFactory.cpp
    sources/engine/items/SoundItem.cpp
    sources/engine/modifications/CharacterModification.cpp
    sources/engine/modifications/MetadataModification.cpp
    sources/engine/modifications/Modification.cpp
    sources/engine/modifications/ModificationQueue.cpp
    sources/engine/modifications/NoteModification.cpp
    sources/engine/modifications/TreeModification.cpp
    sources/misc/FileDetector.cpp
    sources/misc/QAudioProxyModel.cpp
    sources/misc/QSoundEngine.cpp
    sources/widgets/QCustomHeaderView.cpp
    sources/widgets/QCustomSlider.cpp
    sources/widgets/QCustomTableWidget.cpp
    sources/widgets/QCustomTextEdit.cpp
    sources/widgets/QCustomTreeWidget.cpp
    sources/widgets/QCustomTreeWidgetItem.cpp
    sources/windows/AboutDialog.cpp
    sources/windows/ChangeCharacterDialog.cpp
    sources/windows/ChangePropertyDialog.cpp
    sources/windows/CombatDialog.cpp
    sources/windows/DiceDialog.cpp
    sources/windows/ImageWindow.cpp
    sources/windows/ItemDialog.cpp
    sources/windows/MainWindow.cpp
    sources/windows/MetadataDialog.cpp
    sources/windows/SelectCharacterDialog.cpp)
set (PROJECT_FORMS
    sources/windows/AboutDialog.ui
    sources/windows/ChangeCharacterDialog.ui
    sources/windows/ChangePropertyDialog.ui
    sources/windows/CombatDialog.ui
    sources/windows/DiceDialog.ui
    sources/windows/ItemDialog.ui
    sources/windows/MainWindow.ui
    sources/windows/MetadataDialog.ui
    sources/windows/SelectCharacterDialog.ui)

# find libraries
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Phonon4Qt5 REQUIRED)

pkg_check_modules(XMLPP REQUIRED libxml++-2.6)

# Poco
find_path(POCO_INCLUDE_DIR
    NAMES Poco.h
    PATH_SUFFIXES Poco)
if (${POCO_INCLUDE_DIR} STREQUAL POCO_INCLUDE_DIR-NOTFOUND)
    message(FATAL_ERROR "Could not find Poco.h.")
endif()
find_library(POCO_LIBRARY
    NAMES PocoFoundation)
if (${POCO_LIBRARY} STREQUAL POCO_LIBRARY-NOTFOUND)
    message(FATAL_ERROR "Could not find PocoFoundation library.")
endif()
find_library(POCOZIP_LIBRARY
    NAMES PocoZip)
if (${POCOZIP_LIBRARY} STREQUAL POCOZIP_LIBRARY-NOTFOUND)
    message(FATAL_ERROR "Could not find PocoZip library.")
endif()

# Magic
find_path(MAGIC_INCLUDE_DIR
    NAMES magic.h)
if (${MAGIC_INCLUDE_DIR} STREQUAL MAGIC_INCLUDE_DIR-NOTFOUND)
    message(FATAL_ERROR "Could not find magic.h.")
endif()
find_library(MAGIC_LIBRARY
    NAMES magic)
if (${MAGIC_LIBRARY} STREQUAL MAGIC_LIBRARY-NOTFOUND)
    message(FATAL_ERROR "Could not find magic library.")
endif()

include_directories(
    ${XMLPP_INCLUDE_DIRS}
    ${PHONON_INCLUDE_DIR}/phonon
    ${POCO_INCLUDE_DIR}
    ${MAGIC_INCLUDE_DIR})

# linguist tools
find_package(Qt5LinguistTools)
if (${Qt5LinguistTools_FOUND})
    set (TRANSLATION_FILES
        translations/gmassistant_fr.ts
        translations/gmassistant_it.ts)
    qt5_create_translation(TRANSLATION_QM
        ${PROJECT_FORMS}
        ${PROJECT_SOURCES}
        ${TRANSLATION_FILES})
else()
    set (TRANSLATION_QM)
endif()

# executable
set (EXEC_NAME gm-assistant)
add_executable(${EXEC_NAME}
    ${PROJECT_SOURCES}
    ${TRANSLATION_QM})

# link libraries
target_link_libraries(${EXEC_NAME}
    Qt5::Widgets
    Qt5::Svg
    ${XMLPP_LIBRARIES}
    ${POCO_LIBRARY}
    ${POCOZIP_LIBRARY}
    ${MAGIC_LIBRARY}
    ${PHONON_LIBRARY})

# installation
set (BIN_DIR games)
set (DATA_DIR share/games/${EXEC_NAME})
set (DOC_DIR share/doc/${EXEC_NAME})

install(TARGETS ${EXEC_NAME}
    DESTINATION ${BIN_DIR})
install(FILES data/images/GMA.svg
    DESTINATION ${DATA_DIR})
install(FILES ${TRANSLATION_QM} translations/languages
    DESTINATION ${DATA_DIR}/translations)
install(FILES doc/gm-assistant.6
    DESTINATION man/man6)

# desktop file
set (DESKTOP_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/${EXEC_NAME}.desktop.in)
set (DESKTOP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${EXEC_NAME}.desktop)
configure_file(${DESKTOP_FILE_IN} ${DESKTOP_FILE})

install(FILES ${DESKTOP_FILE}
    DESTINATION share/applications)

# API documentation with Doxygen
find_package(Doxygen)
option(BUILD_API_DOC "Create the HTML-based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if (${BUILD_API_DOC})
    if (NOT ${DOXYGEN_FOUND})
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set (doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/dox.conf.in)
    set (doxyfile ${CMAKE_CURRENT_BINARY_DIR}/dox.conf)

    configure_file(${doxyfile_in} ${doxyfile})

    add_custom_target(api
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()

# user guide with LATEX
find_package(LATEX COMPONENTS PDFLATEX)
option(BUILD_USERGUIDE "Build and install the PDF-based user guide (requires PDFLaTeX)" ${LATEX_PDFLATEX_FOUND})

if (${BUILD_USERGUIDE})
    if (NOT ${LATEX_PDFLATEX_FOUND})
        message(FATAL_ERROR "pdflatex is needed to build the user guide.")
    endif()

    # list of PDF files to build
    set (LIST_PDF)
    foreach (LANG en fr)
        set (TARGET_NAME userguide_${LANG}.pdf)
        set (TEXFILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/userguide_${LANG}.tex)
        set (TEX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/doc/userguide/${LANG})

        configure_file(${TEX_PATH}/userguide.tex.in ${TEXFILE_NAME})
        list (APPEND LIST_PDF ${TARGET_NAME})

        add_custom_target(${TARGET_NAME}
            COMMAND ${PDFLATEX_COMPILER} ${TEXFILE_NAME}
            COMMAND ${PDFLATEX_COMPILER} ${TEXFILE_NAME}
            COMMAND ${PDFLATEX_COMPILER} ${TEXFILE_NAME}
            COMMENT "Generating user guide: ${LANG}"
            VERBATIM)

        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME} DESTINATION ${DOC_DIR})
    endforeach()

    add_custom_target(userguide
        DEPENDS ${LIST_PDF})
endif()
