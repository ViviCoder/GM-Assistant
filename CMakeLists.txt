#*************************************************************************
# Copyright © 2017-2022 Vincent Prat & Simon Nicolas
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
set (VERSION_MINOR 3)
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
    sources/engine/Note.cpp
    sources/engine/PropertyList.cpp
    sources/engine/Scenario.cpp
    sources/engine/Tree.cpp
    sources/engine/Version.cpp
    sources/engine/items/FileItem.cpp
    sources/engine/items/ImageItem.cpp
    sources/engine/items/Item.cpp
    sources/engine/items/ItemFactory.cpp
    sources/engine/items/NoteItem.cpp
    sources/engine/items/SoundItem.cpp
    sources/engine/modifications/CharacterModification.cpp
    sources/engine/modifications/MetadataModification.cpp
    sources/engine/modifications/Modification.cpp
    sources/engine/modifications/ModificationQueue.cpp
    sources/engine/modifications/NoteModification.cpp
    sources/engine/modifications/TreeModification.cpp
    sources/widgets/QCustomHeaderView.cpp
    sources/widgets/QCustomTabBar.cpp
    sources/widgets/QCustomTableWidget.cpp
    sources/widgets/QCustomTabWidget.cpp
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
    sources/windows/ReleaseNotesDialog.cpp
    sources/windows/RenameNoteDialog.cpp
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
    sources/windows/ReleaseNotesDialog.ui
    sources/windows/RenameNoteDialog.ui
    sources/windows/SelectCharacterDialog.ui)

# find libraries
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Multimedia REQUIRED)

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
find_library(POCOXML_LIBRARY
    NAMES PocoXML)
if (${POCOXML_LIBRARY} STREQUAL POCOXML_LIBRARY-NOTFOUND)
    message(FATAL_ERROR "Could not find PocoXML library.")
endif()

include_directories(
    ${POCO_INCLUDE_DIR})

# linguist tools
find_package(Qt5LinguistTools)
if (${Qt5LinguistTools_FOUND})
    # ts -> qm
    set (TRANSLATION_FILES
        translations/gmassistant_fr.ts
        translations/gmassistant_it.ts)
    qt5_add_translation(TRANSLATION_QM
        ${TRANSLATION_FILES})

    # update of ts
    add_custom_target(update
        COMMAND ${Qt5_LUPDATE_EXECUTABLE} -no-obsolete ${PROJECT_SOURCES} ${PROJECT_FORMS} -ts ${TRANSLATION_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Updating translation files"
        VERBATIM)
else()
    set (TRANSLATION_QM)
endif()

if (WIN32)
    set (RCFILE GMA.rc)
endif()

# executable
set (EXEC_NAME gm-assistant)
add_executable(${EXEC_NAME}
    ${PROJECT_SOURCES}
    ${TRANSLATION_QM})

if (WIN32)
    target_link_options(${EXEC_NAME} PRIVATE -mwindows)
endif()

# link libraries
target_link_libraries(${EXEC_NAME}
    Qt5::Widgets
    Qt5::Svg
    Qt5::Multimedia
    ${POCO_LIBRARY}
    ${POCOZIP_LIBRARY}
    ${POCOXML_LIBRARY})

# installation
set (BIN_DIR games)
set (DATA_DIR share/games/${EXEC_NAME})
set (DOC_DIR share/doc/${EXEC_NAME})
set (MAN_DIR share/man)

install(TARGETS ${EXEC_NAME}
    DESTINATION ${BIN_DIR})
install(FILES data/images/GMA.svg
    DESTINATION ${DATA_DIR})
if (${Qt5LinguistTools_FOUND})
    install(FILES ${TRANSLATION_QM} translations/languages
        DESTINATION ${DATA_DIR}/translations)
endif()
install(FILES doc/gm-assistant.6
    DESTINATION ${MAN_DIR}/man6)
install(FILES CHANGELOG
    DESTINATION ${DATA_DIR})

# metainfo file
set (METAINFO_ID fr.free.gmassistant)
set (METAINFO_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/${METAINFO_ID}.metainfo.xml.in)
set (METAINFO_FILE ${CMAKE_CURRENT_BINARY_DIR}/${METAINFO_ID}.metainfo.xml)
configure_file(${METAINFO_FILE_IN} ${METAINFO_FILE})

install(FILES ${METAINFO_FILE}
    DESTINATION share/metainfo)

# desktop file
set (DESKTOP_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/${METAINFO_ID}.desktop.in)
set (DESKTOP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${METAINFO_ID}.desktop)
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
find_package(LATEX COMPONENTS PDFLATEX LATEX2HTML)
option(BUILD_USERGUIDE_PDF "Build and install the user guide in PDF format (requires PDFLaTeX)" ${LATEX_PDFLATEX_FOUND})
option(BUILD_USERGUIDE_HTML "Build and install the user guide in HTML format (requires LaTeX2HTML)" ${LATEX_LATEX2HTML_FOUND})

if (${BUILD_USERGUIDE_PDF})
    if (NOT ${LATEX_PDFLATEX_FOUND})
        message(FATAL_ERROR "pdflatex is needed to build the user guide in PDF format.")
    endif()

    set (PDFLATEX_OPTIONS -halt-on-error -interaction=nonstopmode)

    # list of PDF files to build
    set (LIST_PDF)
endif()

if (${BUILD_USERGUIDE_HTML})
    if (NOT ${LATEX_LATEX2HTML_FOUND})
        message(FATAL_ERROR "latex2html is needed to build the user guide in HTML format.")
    endif()

    set (LATEX2HTML_OPTIONS -verbosity 0 -show_section_numbers -info 0 -no_auto_link)

    # list of HTML
    set (LIST_HTML)
else()
    # removes html.sty from the preamble
    set (HTML_STY "\\newcommand{\\latex}[1]{#1}%")
endif()

if (${BUILD_USERGUIDE_PDF} OR ${BUILD_USERGUIDE_HTML})
    foreach (LANG en fr)
        set (TEXFILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/userguide_${LANG}.tex)
        set (TEX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/doc/userguide/${LANG})

        configure_file(${TEX_PATH}/userguide.tex.in ${TEXFILE_NAME})

        if (${BUILD_USERGUIDE_PDF})
            set (TARGET_NAME userguide_${LANG}.pdf)
            list (APPEND LIST_PDF ${TARGET_NAME})

            add_custom_command(OUTPUT ${TARGET_NAME}
                COMMAND ${PDFLATEX_COMPILER} ${PDFLATEX_OPTIONS} ${TEXFILE_NAME}
                COMMAND ${PDFLATEX_COMPILER} ${PDFLATEX_OPTIONS} ${TEXFILE_NAME} > /dev/null
                COMMAND ${PDFLATEX_COMPILER} ${PDFLATEX_OPTIONS} ${TEXFILE_NAME} > /dev/null
                MAIN_DEPENDENCY ${TEXFILE_NAME}
                COMMENT "Generating PDF user guide: ${LANG}"
                VERBATIM)

            install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}
                DESTINATION ${DOC_DIR}/pdf)
        endif()

        if (${BUILD_USERGUIDE_HTML})
            set (DIR_NAME userguide_${LANG})
            set (TARGET_NAME ${DIR_NAME}/index.html)
            list (APPEND LIST_HTML ${TARGET_NAME})
            string(TOUPPER ${LANG} LANG_UPPER)

            add_custom_command(OUTPUT ${TARGET_NAME}
                COMMAND ${LATEX2HTML_CONVERTER} ${LATEX2HTML_OPTIONS} -iso_language ${LANG_UPPER} ${TEXFILE_NAME} > /dev/null
                MAIN_DEPENDENCY ${TEXFILE_NAME}
                COMMENT "Generating HTML user guide: ${LANG}"
                VERBATIM)

            install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${DIR_NAME}
                DESTINATION ${DOC_DIR}/html
                FILES_MATCHING
                PATTERN "*.html"
                PATTERN "*.png")
        endif()
    endforeach()
endif()

# generate generic targets
set (TARGETS_ALL)

if (${BUILD_USERGUIDE_PDF})
    set (TARGET_PDF userguide_pdf)
    add_custom_target(${TARGET_PDF}
        ALL
        DEPENDS ${LIST_PDF})
    list (APPEND TARGETS_ALL ${TARGET_PDF})
endif()

if (${BUILD_USERGUIDE_HTML})
    set (TARGET_HTML userguide_html)
    add_custom_target(${TARGET_HTML}
        ALL
        DEPENDS ${LIST_HTML})
    list (APPEND TARGETS_ALL ${TARGET_HTML})
endif()

if (TARGETS_ALL)
    add_custom_target(userguide
        ALL
        DEPENDS ${TARGETS_ALL})
endif()
