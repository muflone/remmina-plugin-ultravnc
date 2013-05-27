# Project name : Remmina Plugin ULTRAVNC
# Remmina protocol plugin to connect via VNC using UltraVNC viewer.
#
# Copyright (C) 2013 Fabio Castelli <muflone@vbsimple.net>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

set(REMMINA_PLUGIN_SRCS
  src/remmina_plugin.c
)
add_library(remmina-plugin-ultravnc ${REMMINA_PLUGIN_SRCS})
set_target_properties(remmina-plugin-ultravnc PROPERTIES PREFIX "")
include_directories(${GTK_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/remmina-plugins)

install(TARGETS remmina-plugin-ultravnc DESTINATION ${REMMINA_PLUGINDIR})

set(APPICON16_EMBLEMS_DIR "${REMMINA_DATADIR}/icons/hicolor/16x16/emblems")
install(FILES 16x16/emblems/remmina-ultravnc.png DESTINATION ${APPICON16_EMBLEMS_DIR})

set(APPICON22_EMBLEMS_DIR "${REMMINA_DATADIR}/icons/hicolor/22x22/emblems")
install(FILES 22x22/emblems/remmina-ultravnc.png DESTINATION ${APPICON22_EMBLEMS_DIR})