/*
 *     Project: Remmina Plugin ULTRAVNC
 * Description: Remmina protocol plugin to connect via VNC using UltraVNC viewer.
 *      Author: Fabio Castelli (Muflone) <muflone@vbsimple.net>
 *   Copyright: 2013-2016 Fabio Castelli (Muflone)
 *     License: GPL-2+
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of ERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "plugin_config.h"
#include <remmina/remmina_plugin.h>
#if GTK_VERSION == 3
  # include <gtk/gtkx.h>
#endif

static RemminaPluginService *remmina_plugin_service = NULL;

static void remmina_plugin_ultravnc_init(RemminaProtocolWidget *gp)
{
  TRACE_CALL("remmina_plugin_ultravnc_init");
  remmina_plugin_service->log_printf("[%s] Plugin init\n", PLUGIN_NAME);
}

static gboolean remmina_plugin_ultravnc_open_connection(RemminaProtocolWidget *gp)
{
  TRACE_CALL("remmina_plugin_ultravnc_open_connection");
  remmina_plugin_service->log_printf("[%s] Plugin open connection\n", PLUGIN_NAME);
  #define GET_PLUGIN_STRING(value) \
    g_strdup(remmina_plugin_service->file_get_string(remminafile, value))
  #define GET_PLUGIN_BOOLEAN(value) \
    remmina_plugin_service->file_get_int(remminafile, value, FALSE)
  #define GET_PLUGIN_PASSWORD(value) \
    g_strdup(remmina_plugin_service->file_get_secret(remminafile, value));

  RemminaFile *remminafile;
  gboolean ret;
  GError *error = NULL;
  gchar *argv[50];
  gint argc;
  gint i;
  GPid pid;
  gchar *option_str;

  remminafile = remmina_plugin_service->protocol_plugin_get_file(gp);

  argc = 0;
  argv[argc++] = g_strdup("ultravnc-viewer");

  option_str = GET_PLUGIN_PASSWORD("password");
  if (option_str)
  {
    argv[argc++] = g_strdup("-password");
    // Replace \$ in \\$ in the password
    if (GET_PLUGIN_BOOLEAN("replacedollar"))
      option_str = g_strdup(g_strjoinv("\\$", g_strsplit(option_str, "$", -1)));
    argv[argc++] = g_strdup(option_str);
  }

  argv[argc++] = g_strdup(GET_PLUGIN_STRING("server"));
  argv[argc++] = NULL;

  ret = g_spawn_async(NULL, argv, NULL, G_SPAWN_SEARCH_PATH,
    NULL, NULL, &pid, &error);

  for (i = 0; i < argc; i++)
    g_free (argv[i]);

  if (!ret)
    remmina_plugin_service->protocol_plugin_set_error(gp, "%s", error->message);

  return FALSE;
}

static gboolean remmina_plugin_ultravnc_close_connection(RemminaProtocolWidget *gp)
{
  TRACE_CALL("remmina_plugin_ultravnc_close_connection");
  remmina_plugin_service->log_printf("[%s] Plugin close connection\n", PLUGIN_NAME);
  remmina_plugin_service->protocol_plugin_emit_signal(gp, "disconnect");
  return FALSE;
}

/* Array of RemminaProtocolSetting for basic settings.
 * Each item is composed by:
 * a) RemminaProtocolSettingType for setting type
 * b) Setting name
 * c) Setting description
 * d) Compact disposition
 * e) Values for REMMINA_PROTOCOL_SETTING_TYPE_SELECT or REMMINA_PROTOCOL_SETTING_TYPE_COMBO
 * f) Unused pointer
 */
static const RemminaProtocolSetting remmina_plugin_ultravnc_basic_settings[] =
{
  { REMMINA_PROTOCOL_SETTING_TYPE_SERVER, NULL, NULL, FALSE, NULL, NULL },
  { REMMINA_PROTOCOL_SETTING_TYPE_PASSWORD, NULL, NULL, FALSE, NULL, NULL },
  { REMMINA_PROTOCOL_SETTING_TYPE_CHECK, "replacedollar", N_("Force replace $ in \\$ in the password"), FALSE, NULL, NULL },
  { REMMINA_PROTOCOL_SETTING_TYPE_END, NULL, NULL, FALSE, NULL, NULL }
};

/* Protocol plugin definition and features */
static RemminaProtocolPlugin remmina_plugin =
{
  REMMINA_PLUGIN_TYPE_PROTOCOL,                 // Type
  PLUGIN_NAME,                                  // Name
  PLUGIN_DESCRIPTION,                           // Description
  GETTEXT_PACKAGE,                              // Translation domain
  PLUGIN_VERSION,                               // Version number
  PLUGIN_APPICON,                               // Icon for normal connection
  PLUGIN_APPICON,                               // Icon for SSH connection
  remmina_plugin_ultravnc_basic_settings,       // Array for basic settings
  NULL,                                         // Array for advanced settings
  REMMINA_PROTOCOL_SSH_SETTING_NONE,            // SSH settings type
  NULL,                                         // Array for available features
  remmina_plugin_ultravnc_init,                 // Plugin initialization
  remmina_plugin_ultravnc_open_connection,      // Plugin open connection
  remmina_plugin_ultravnc_close_connection,     // Plugin close connection
  NULL,                                         // Query for available features
  NULL,                                         // Call a feature
  NULL                                          // Send a keystroke
};

G_MODULE_EXPORT gboolean remmina_plugin_entry(RemminaPluginService *service)
{
  TRACE_CALL("remmina-plugin-ultravnc::remmina_plugin_entry");
  remmina_plugin_service = service;

  if (!service->register_plugin((RemminaPlugin *) &remmina_plugin))
  {
    return FALSE;
  }
  return TRUE;
}
