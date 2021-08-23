##    QLocalServer Test Tool
##    Copyright (C) 2021  Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
##
##    This program is free software: you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation, either version 3 of the License, or
##    (at your option) any later version.
##
##    This program is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with this program.  If not, see <http://www.gnu.org/licenses/>.

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        filenamedrophandler.h
        filenamedrophandler.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(local-server-test-tool
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(local-server-test-tool SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(local-server-test-tool
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(local-server-test-tool PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(local-server-test-tool PRIVATE Qt${QT_VERSION_MAJOR}::Network)

set_target_properties(local-server-test-tool PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(local-server-test-tool)
endif()
