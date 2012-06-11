/*********************************************************************
 *
 * @brief Runs an internal httpd (WWW server) and exposes a WWW/WAP
 * @brief interface to Pidgin converstaions.  This allows the use of
 * @brief pidgin (aka gaim ) using a simple web browser such as those
 * @brief found in mobile phones and other minimal WAP devices.
 * @brief WebPidgin-Z by Shaorang ( shaorang@shaorang.com )
 * @brief http://www.shaorang.com/webpidginz/
 *
 * @copyright WebPidgin Original (C) 2006,2007 Sebastian Sobolewski ( spsobole@thirdmartini.com )
 * @url http://www.thirdmartini.com/index.php/WebPidgin_2.x
 *
 ********************************************************************** */
static char  *license = "\
    &nbsp;This program is free software; you can redistribute it and/or \
    modify it under the terms of the GNU General Public License as \
    published by the Free Software Foundation; either version 2 of \
    the License, or (at your option) any later version. \
 <BR><BR> \
    This program is distributed in the hope that it will be useful, \
    but WITHOUT ANY WARRANTY; without even the implied warranty of \
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU \
    General Public License for more details. \
 <BR><BR> \
    You should have received a copy of the GNU General Public \
    License along with this program; if not, write to the Free \
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, \
    MA 02111-1307, USA.";

 /** *********************************************************************
 * Compiling:
 *   - Follow the directions here: http://pidgin.sourceforge.net/
 *   - Copy webpidgin.c into the pidgin/plugins source directory.
 *   - Inside that directory run "make -f Makefile.mingw webpidginz.dll"
 *
 * Installing:
 *   - Follow the directions here: http://pidgin.sourceforge.net/faq.php#q94
 *   - Start Pidgin
 *   - By default the plugin launches the mini web server on port 8888
 *   - http://localhost:8888/ should display the Root/Home page
 */

#define WEBPIDGIN_VERSION "2.0-Z-8"

/**
 ******************************************* WEBPIDGIN-Z ***********************************************
 * 		SHAORANG'S CHANGES:
 *
 *		WebPidgin-Z v. 2.0.Z-8
 *
 *		- Several html elements replaced with CSS  
 *		- Several bugfixes
 *		- Fixed some issues with opera and konqueror
 *		- Added sound support with SWFSound and SWFObject
 *		- Added suport for custom CSS. 
 *				You must place a custom.css file in the user's .purple directory ,e.g., '/home/user/.purple' or '%APPDATA%\.purple'. 
 *				Sample custom.css included with dark theme
 *
 *
 *		WebPidgin-Z v. 2.0.Z-7
 *
 *		- Fixed a bug in the active chats list on win32
 *		- Added an "autoaccept" when requesting a group chat (it was so annoying when someone invites you and you cant accept...)
 *		- Improved the colorize in chats with several buddies (each buddy has a different color now)
 *		- Removed the "about" action
 *		- EXPERIMENTAL support to receive file transfers
 *		- Added the posibility of download custom binary files from webpidginz web server
 *		- Added favicon.ico and logo.png
 *		- Improved chats when grouping messages
 *		- Fixed a bug in ajax, the timeout variable didn't get back to its default value after an error/timeout within the ajax call
 *		- Added client_write_vargs function to support variable arguments like printf (with g_strdup_vprintf). Code should be rewrite to take advantage of this
 *		- Several bugfixes 
 *
 *
 *		WebPidgin-Z v. 2.0.Z-6
 *
 *		- Fixed the "Don't use Ajax when sending messages" option
 *		- Improved support for buddies repeated in different accounts
 *		- Changed the width of the left frame to 20%
 *		- Fixed bugs with buddy icons in XMPP accounts
 *		- Fixed several bugs in group chats
 *		- EXPERIMENTAL support for smileys
 *		- Fix a bug when debugging pidgin on WIN32
 *		- Changed config dialog
 *		- Added an option to restrict access to webserver by IP (requested by Andrew)
 *		- Several bugfixes
 *		- Reduced the size of the precompiled dll for win32 (Thanks to Andrei DAMIAN-FEKETE for his advice)
 *
 *
 *		WebPidgin-Z v. 2.0.Z-5
 *
 *		- Added support for buddy icons
 *		- Improved chats colorize
 *		- Fixed a bug when digest authentication with some annoying browsers (IE...)
 *		- Fixed a bug when showing offline buddies
 *		- Fixed several bugs when loading/unloading and with the prefs
 *		- Fixed a bug with Ajax in older Mozilla/Firefox/Gecko versions
 *		- Several fixes to Ajax engine for mobile browsers
 *		- Increased the size of the links in the commands menu
 *		- Added a notification if, while loading, the selected port cannot be allocated
 *
 *
 *		WebPidgin-Z v. 2.0.Z-4b
 *
 *		- Fixed a CRITICAL bug when genering the nonce value for the digest authentication
 *		- Fixed a bug with some web browsers when using digest authentication
 *
 *
 *		WebPidgin-Z v. 2.0.Z-4
 *
 *		- Rewrited once again the ajax engine. It's a lot better now.
 *      - Added digest authentication, basic authentication should be never used again because the user/passwd is sent in clear text
 *		- Changed the REALM in authentication
 *      - A lot of minor bugs fixed
 *
 *
 *		WebPidgin-Z v. 2.0.Z-3
 *
 *		- Fixed: "Active:" was shown even if there wasnt any chat active
 *		- Now the active chats window shows also the count of unseen messages, not only the total of messages.
 *		- Rewrited the server code which handles the ajax requests
 *		- Added an option to use Ajax only when refreshing and not when sending messages. Fixes some problems with slow connections and mobile browsers
 *		- Fixed a problem when you try to talk to a buddy with a disabled account
 *		- Now it changes the document title to show alerts to the user
 *		- Added ajax support for the active chats window and for the buddy window
 *		- A lot of minor bugs fixed
 *		- A lot of code rewrited
 *		- A lot of WIP stuff added
 *
 *
 *		WebPidgin-Z v. 2.0.Z-2
 *
 *		- Fixed a bug when there are two accounts which are buddies among themselves
 *		- Fixed some issues with Ajax and a few more changes
 *		- Improved the message grouping
 *
 *
 *		WebPidgin-Z v. 2.0.Z-1
 *
 *		- It shows the total of messages in a chat/IM in the active conversations window
 *		- Fixed: now the right META header is send for UTF8 encoding
 *		- It shows the right status for the buddies, not only online / idle / offline
 *		- The <input> element in the sendMessage form get the focus automatically when loading a conversation
 *		- Fixed: when sending messages, the URL with the GET request remains and it made the message to be send over and over again
 *		- Hyperlinks are generated automatically when typing a URL in a conversation
 *		- Removed the custom base64 encode/decode functions, webpidgin now uses libpurple ones
 *		- Replaced snprintf with g_snprintf
 *		- Added a option for logging the webserver activity
 *		- Added a record of the last established sessions to the main window. It shows information of IPs, proxies and User-Agent, very useful for security
 *		- Added a AJAX (javascript) engine in the conversation window, boosting the interactivity and the response time
 *		- Added an option to changes in real time the message limit to show in the conversation window
 *		- In the buddy list window, the buddies changes the color based on their status (online / idle / etc)
 *		- Added the option to group the messages from the same user in the conversation window (gtalk alike)
 *		- Some minor bugs fixed, and some code cleared
 *
 * POR HACER:
 *		- Mostrar el numero de correos en las cuentas
 *		- Identificar de que cuenta es cada contacto
 *		- Configurar colores		
 *
 *
 ******************************************* ORIGINAL WEBPIDGIN ***********************************************
 *
 * CHANGES:
 *     TODO:
 *       - Fix Colorize History
 *       - Add History Searching using regex?
 *       - Closing Chats/IM views
 *
 *     2.0-B19 (6/8/2008)
 *       - Fixed RSS feed
 *       - Fix Colorize History
 *     2.0-B18 (6/5/2008)
 *       - Add support for IRC and chat rooms.
 *       - Changed the send/display method for messages. Use libpurple internals
 *       - Support for / commands ( Mostly for IRC )
 *       - If in Chat room, display other chat room members
 *       - Display current conversation name in the conversation window
 *       - Broke: rss feed, colored display
 *     2.0-B17 (6/8/2007)
 *       - Update to use Pidgin interfaces
 *       - Renamed to WebPidgin
 *     2.0-B16
 *       - Buddies in the WebGaim Buddy List will now be laid out in the
 *          same manner as in Purple, including Groups. ( Buddies->Sort Buddies )
 *       - Removed support for Purple < 2.0.beta2; code was getting too messy
 *           Please upgrade to at least gaim 2.0beta3
 *       - Added Auto-Refresh with meta refresh tag
 *         + preference to enable/disable, set # seconds in plugin details
 *         + Added Dynamic support. Refresh interval will increase and decrease
 *           based on rate of conversation
 *       - Status stub
 *       - RSS Status feed available at http://<ip>:<port>/status.rss
 *         + Compatible with Akregator, Google
 *         + Passes validation ( http://www.feedvalidator.org/ )
 *         + Toggle through Plugin Options
 *       - Fixed
 *         + Couple Memory Leaks
 *         + Segfault on empty send message
 *     2.0-B15
 *       - Added Basic History Code
 *       - Added Option for frames ( Active Buddy List In Frames )
 *       - Moved Login/Account management to new screen
 *       - Options can be manipulated from the options screen
 *     2.0-B14
 *       - Integrate Patch from David Morse
 *         + Display status message (if set) next to buddies
 *          + preference to enable/disable this in plugin details
 *         + strip embedded HTML tags out of IMs
 *         + linefeeds after every IM in action_chat()
 *     2.0-B13
 *       - Use Name defined in gaim instead of "Self"
 *       - Add Buddy & Account name bolding in conversation view
 *     2.0-B12
 *       - Cleanup tabs. (tab stop == 4 spaces)
 *       - Integrate Patch from David Morse
 *         + root page shows idle time for idle buddies
 *         + root page shows "Away" for non-active, non-idle buddies
 *     2.0-B11
 *       - Make the code more strict in general
 *       - Avoid NULL pointer dereference on windows platforms
 *             ( since when does printf segfault on a NULL string parameter? )
 *       - Encode login account names ( spaces once again )
 *     2.0-B10
 *       - Add a timestamp to all changable items to make sure browsers refetch the data
 *     2.0-B9
 *       - Buddies with spaces now work, for real this time.
 *     2.0-B8
 *       - URL-ENCODE buddy names in post requests
 *          - ( Buddies With Spaces and Odd characters should work )
 *       - Disable coloring in history ( for silly devices that freak when they see FONT COLOR tags )
 *
 * Known to support :
 *   - Mozilla/Firefox
 *   - IE
 *   - Konqueror
 *   - Samsung i500 ( Handspring Blazer Browser )
 *   - Sony PSP
 *   - Sony PS3
 *   - Nintendo Wii
 *   - WAP 2.0 (Sanyo RL-4920, others)
 *   - Any others?
 *
 * Things That DO work:
 *   - display Buddies with active messages ontop of chat view
 *   - login and out of accounts
 *   - IM conversations
 *   - continuing a IM conversation started using pidgin
 *   - starting a new IM conversation using webpidgin
 *   - root page displays new messages: (3) BuddyName
 *   - timestamp messages
 *   - quick access keys for active conversations. ( IE the [0] | [1] keys indicate the numeric key to hit on a phone for a quick link )
 *   - simple color support
 *   - trim conversations at uOptionTrim messages in a conversation.  Dropping oldest messages
 *   - do our best to unmangle web form data
 *   - compiles for Purple 2.0+
 *   - Authentication support ( Basic aka base64 )
 *   - Configuration panel for plugin sets username/password
 *
 * Known Broken:
 *   - Account/Login takes a few seconds, so those buddies don't show up right away (requires refresh)
 *   - webforms mangle things.  We may not unmangle everything correctly yet
 *
 * A little about the UI:
 *  The "CMD:" bar.
 *    U = Update/Refresh Current View
 *    H = Home Takes you to the root page
 *    A = Account Login/Logout
 *    O = Options
 *    ? = Help
 *
 * Home Page:
 *   Active: Lists all active conversations ( IE ones that have new text in them )
 *   When Frames are enabled, the left frame contains the buddy list and Active Chat list
 *       Only the Left Frame will be auto updated/refreshed
 *
 * Chatting:
 *   If you click on a Online or Active buddy the chat page will show up
 *   From here you can send a IM to the buddy you selected.
 *   ( As a side effect it will also open a pidgin window on the PC running pidgin )
 *
 *
 */
#include "internal.h"
#include "version.h"
#include "gtkimhtml.h"
#include "gtkplugin.h"
#include "gtkprefs.h"
#include "gtkgaim-compat.h"

#include "conversation.h"
#include "network.h"
#include "eventloop.h"
#include "prefs.h"
#include "signals.h"
#include "version.h"
#include "util.h"
#include "debug.h"
#include "cmds.h"
#include "cipher.h"

#include "smiley.h"
#include "pidginstock.h"

#include "gtkplugin.h"
#include "gtkutils.h"
#include "gtkblist.h"
#include "gtkthemes.h"
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#ifndef _WIN32
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

#include "webpidginz_binary_data.c"


const char * gOptionUsername = NULL;
const char * gOptionPassword = NULL;
int gOptionPort = 8888;
static unsigned uOptionTrim = 100; // trim after 100 messages
static gboolean gOptionUseColor = 1;
static gboolean gOptionBoldNames = 1;
static gboolean gOptionStatusMessages = 1;
static gboolean gOptionWWWFrames = 0;
static gboolean gOptionMetaRefresh = 0;
static gboolean gOptionMetaRefreshDynamic = 0; // speed up refresh rate when we start getting messages
static int gOptionMetaRefreshSeconds = 180;    // refresh after 3 minutes (if enabled)
static int gLastRefreshInterval = 0;
static int gUnseenMessageCount = 0;
static int gMissedRefreshes = 0;
static int gOptionFontAdjust = 0;

//mio
static gboolean gSaveConnectionLog = 0;
static char * gConnectionLogFileName = "webpidgin";
static int gShowNLastSessions = 3;
static gboolean gShowNoAuthSessions = 1;
static gboolean gUseJavascript = 1;
static gboolean gUseJSOnlyRef = 0;
static gboolean gGroupMessages = 1;
static gboolean gOptionRSSFeed = 1;
static gboolean gOptionUseDigestAuth = 1;
static gboolean gOptionShowBuddyIcon = 0;
static gboolean gOptionRestrictAccessByIP = 1;
static const char* gAllowedIP = "127.0.0.1";
static gboolean gOptionShowSmileys = 1;
static gboolean gGroupMessagesAutoH = 1;
static gboolean gUseSounds = 1;
static gboolean gUseCustomCSS = 0;

#define DREFRESH_MIN    4
#define DREFRESH_STEP   2 /// Cut in half at each hit

#define NUM_COLORS 12
static char *array_colors[NUM_COLORS]={"#A4BEC8", "#55D", "#D55", "#080", "#FC0", "#952", "#F19", "#F50", "#2BA", "#D83", "#808", "#F87"};


typedef struct{
    const char * name;
    const char * shortOnline;
    const char * shortOffline;
}webpidgin_icon_t;

typedef struct{
    int iListenWatcher;
    int fdServerSocket;
    unsigned usListenPort;
    char * auth;        
}webpidgin_data_t;

typedef struct{
    webpidgin_data_t *webpidgin;
    int fd;
    int watcher; /** The purple_input_add handle on this socket **/
	char ip_client[INET_ADDRSTRLEN];
	char * ip_client_xff; //X-Forwarded-For
	char * user_agent;
	gboolean auth_ok;
	char * buffer;
	size_t buffer_len;
}webpidgin_client_t;

typedef int (*WebPidginActionCallback) (webpidgin_client_t *client, const char * extra);

typedef struct{
    const char * action;
    WebPidginActionCallback callback;
}webpidgin_parse_t;



typedef struct rss_item_s{
    struct rss_item_s *next;
    char *who;
    char *what;
    char *convName;
    time_t tm;
}rss_item_t;

typedef struct{
	char ip_client[INET_ADDRSTRLEN];
	char * ip_client_xff; //X-Forwarded-For
	char * user_agent;
	time_t last_connection;
	gboolean auth_ok;
}webpidgin_session;

GSList* last_sessions = NULL;


typedef struct{
	char to[256];
	char url[4096];
	int count;
	int count_new;
}webpidgin_emails;

static GList* emails = NULL;
static GHashTable* active_httpds = NULL;

static GHashTable* nonce_list = NULL;

static rss_item_t rssHead = { NULL, NULL, NULL, NULL, 0 };

static const char *empty_string ="";
static FILE *webpidgin_connection_log;

static PurplePlugin * webpidgin_plugin=NULL;

/// Prototypes of functions, so they can be called before they're defined
static void webpidgin_save_pref_bool( const char * pref, unsigned enabled );
static void webpidgin_load_pref();
static int action_active_list( webpidgin_client_t * httpd, const char * notused );
static int action_options( webpidgin_client_t * httpd, const char * extra );
static int action_accounts( webpidgin_client_t * httpd, const char * notused );
static int action_root( webpidgin_client_t * httpd, const char * notused );
static int action_login( webpidgin_client_t * httpd, const char * extra );
static int action_logout( webpidgin_client_t * httpd, const char * extra );
static int action_history( webpidgin_client_t * httpd, const char * extra );
static int action_sessions( webpidgin_client_t * httpd, const char * extra );

static int write_log_connection(int fd, char * buffer);
static void add_session (webpidgin_client_t *httpd);
static const char * get_buddy_name(PurpleBuddy *buddy);
static PurpleAccount* find_account( const char * from );
static const char* get_active_chats(char * hash);
static GHashTable* parse_get_extra(const char * extra);
static void show_ajax_engine( webpidgin_client_t * httpd );
static void show_active_list( webpidgin_client_t * httpd);
static PurpleConversation * sendMessage( webpidgin_client_t * httpd, const char * message, const char *to );
static const char* md5sum(const char* data);
static void client_end_connection (webpidgin_client_t *httpd);
static void get_keys(gpointer key, gpointer value, gpointer user_data);

static ssize_t client_write_vargs( webpidgin_client_t * httpd, const char* format, ... );

/**
 * @brief load all webpidgin prefs
 */
static void webpidgin_load_pref()
{
    gOptionUseColor = purple_prefs_get_bool("/plugins/webpidgin/use_color");
    gOptionBoldNames = purple_prefs_get_bool("/plugins/webpidgin/use_bold_names");
    gOptionWWWFrames = purple_prefs_get_bool("/plugins/webpidgin/use_www_frames");
    gOptionMetaRefresh = purple_prefs_get_bool("/plugins/webpidgin/use_meta_refresh");
    gOptionMetaRefreshDynamic = purple_prefs_get_bool("/plugins/webpidgin/use_meta_refresh_dynamic");
    gOptionStatusMessages = purple_prefs_get_bool("/plugins/webpidgin/use_status_messages");
    gOptionUsername = purple_prefs_get_string("/plugins/webpidgin/server_user");
    gOptionPassword = purple_prefs_get_string("/plugins/webpidgin/server_password");
    gOptionPort = purple_prefs_get_int("/plugins/webpidgin/server_port");
    gOptionMetaRefreshSeconds = purple_prefs_get_int("/plugins/webpidgin/meta_refresh_seconds");
    gOptionRSSFeed = purple_prefs_get_bool("/plugins/webpidgin/use_rss_feed");
    gOptionFontAdjust= purple_prefs_get_int("/plugins/webpidgin/font_adjust");
    gSaveConnectionLog = purple_prefs_get_bool("/plugins/webpidgin/save_connection_log");
    gShowNLastSessions= purple_prefs_get_int("/plugins/webpidgin/show_n_last_sessions");
    gShowNoAuthSessions = purple_prefs_get_bool("/plugins/webpidgin/show_no_auth_sessions");
    gUseJavascript = purple_prefs_get_bool("/plugins/webpidgin/use_jscript");
    gUseJSOnlyRef = purple_prefs_get_bool("/plugins/webpidgin/use_jscript_only_refresh");
    uOptionTrim= purple_prefs_get_int("/plugins/webpidgin/max_messages");
    gGroupMessages = purple_prefs_get_bool("/plugins/webpidgin/group_messages");
    gOptionUseDigestAuth= purple_prefs_get_bool("/plugins/webpidgin/use_digest_auth");
    gOptionShowBuddyIcon= purple_prefs_get_bool("/plugins/webpidgin/show_buddy_icon");
    gOptionRestrictAccessByIP= purple_prefs_get_bool("/plugins/webpidgin/restrict_access_by_ip");
    gAllowedIP = purple_prefs_get_string("/plugins/webpidgin/allowed_ip");
    gOptionShowSmileys= purple_prefs_get_bool("/plugins/webpidgin/show_smileys");
    gUseSounds= purple_prefs_get_bool("/plugins/webpidgin/use_sounds");
    gUseCustomCSS= purple_prefs_get_bool("/plugins/webpidgin/use_custom_css");
}

/**
 * @brief Common pref save code, since we now use it in multiple places
 */
static void webpidgin_save_pref_bool( const char * pref, unsigned enabled )
{
    gchar gcPref[256];
    g_snprintf(gcPref, sizeof(gcPref), "/plugins/webpidgin/%s",  pref );
    purple_prefs_set_bool(gcPref, enabled);
    purple_debug_info("WebPidgin 2","Saved-Pref: %s = [%d]\n",pref, enabled);
}

static void webpidgin_save_pref_int( const char * pref, int value )
{
    gchar gcPref[256];
    g_snprintf(gcPref, sizeof(gcPref), "/plugins/webpidgin/%s",  pref );
    purple_prefs_set_int(gcPref, value );
    purple_debug_info("WebPidgin 2","Saved-Pref: %s = [%d]\n",pref, value);
}

static const char* md5sum(const char* data)
{
	PurpleCipher *cipher = NULL;
	PurpleCipherContext *context = NULL;
	static char cdigest[33];
	gboolean ret = FALSE;

	cipher = purple_ciphers_find_cipher("md5");
	context = purple_cipher_context_new(cipher, NULL);
	purple_cipher_context_append(context, (guchar *)(data), strlen((data)));

	ret = purple_cipher_context_digest_to_str(context, sizeof(cdigest), cdigest, NULL);

	purple_cipher_context_destroy(context);

	return cdigest;
}

/// find a smiley
static GtkIMHtmlSmiley * find_smiley(const char * protocol_id, const char* gesto)
{
	GSList * smileylist= pidgin_themes_get_proto_smileys(protocol_id);
	GtkIMHtmlSmiley *gtksmiley=NULL;
			
	while (smileylist)
	{				
		gtksmiley= smileylist->data;
		 
		if (purple_strequal( gtksmiley->smile, gesto))
			return gtksmiley;		
			
		smileylist=smileylist->next;
	}
	
	return NULL;
}


/**
 * @brief Find a PurpleBuddy object based on the buddy name
 */
static PurpleBuddy * find_buddy( const char *name )
{
	PurpleBuddy *buddydisconnected=NULL;
    PurpleBlistNode *gnode, *cnode, *bnode;
    long long unsigned int bid=0;    
    gboolean isbuddyid = FALSE;    
    char *name_noslash = NULL;
    
    char *punt= g_strrstr(name, "/");
    if (punt)			
		name_noslash= g_strndup (name, strlen(name)-strlen(punt));	
		
	//purple_debug_info("WebPidgin 2/DFG","%s \n",name_noslash);

	if (sscanf(name,"bd=%llx&",&bid) == 1) 
	{
        isbuddyid = TRUE;
    }

    for (gnode = purple_get_blist()->root; gnode != NULL; gnode = gnode->next)
    {
        if (!PURPLE_BLIST_NODE_IS_GROUP(gnode))
            continue;

        for (cnode = gnode->child; cnode != NULL; cnode = cnode->next)
        {
            if (!PURPLE_BLIST_NODE_IS_CONTACT(cnode))
                continue;

            for (bnode = cnode->child; bnode != NULL; bnode = bnode->next)
            {
                PurpleBuddy *buddy = (PurpleBuddy *)bnode;


				if (isbuddyid)
				{					
					//purple_debug_info("WebPidgin 2/DFG","%p %p %llx\n",buddy, (void *)bid, bid);
					if (buddy == (void *)bid)
						return buddy;            
				}
				else
				{									
					if( strcmp(buddy->account->username,name)== 0)
						continue;											

					//purple_debug_info("WebPidgin 2/DFG","%s %s %s %s\n",buddy->name, name, buddy->account->username, name_noslash);

					if( strcmp(buddy->name,name)== 0 )
					{
						if (!purple_account_is_connected(buddy->account))
						{
							buddydisconnected = (PurpleBuddy *)bnode;
							continue;
						}						
						else
							return buddy;
					}
					else if(name_noslash && strcmp(buddy->name,name_noslash)== 0 )
					{							
						return buddy;
					}									
				}
            }
        }
    }
    return buddydisconnected;
}

/**
 * @brief Find a conversation by name
 *        // Format should be: type=%d&name
 */

static PurpleConversation * find_conversation(const char *name)
{
    PurpleConversation *conv = NULL;
    //uint64_t id;
	long long unsigned int id=0;	

	if (! name)
		return NULL;

    if (sscanf(name,"id=%llx&",&id) == 1) {
        GList *cnv;
        for (cnv = purple_get_conversations(); cnv != NULL; cnv = cnv->next) {
            conv = (PurpleConversation *)cnv->data;
            //purple_debug_info("WebPidgin 2.x","check [%p = %llx]\n",conv,id);
            if ((long long unsigned int) conv==id) 
            {
                return conv;
            }
        }
    } else {
    	
    	PurpleBuddy *b = NULL;
            
		b=find_buddy(name);
		if (b)
		{                
			conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, b->name, b->account);
			if (conv)
				return (conv);
		}
    	
    	
    	///kizas pueda borrar este codigo, ya vere
        /**GList *cnv;
        for (cnv = purple_get_conversations(); cnv != NULL; cnv = cnv->next)
        {
            const char *convName = NULL;
            conv = (PurpleConversation *)cnv->data;
                        					
            convName = purple_conversation_get_name(conv);
            if ( (convName) && (strcmp(convName,name) == 0)) {
                return conv;
            }
            //purple_debug_info("WebPidgin 2.x/ASD","%s\n",convName);
            
            convName = purple_conversation_get_title(conv);
            if ( (convName) && (strcmp(convName,name) == 0)) {
                return conv;
            }
        } **/       
    }
    return NULL;
}

static const char * get_buddy_name(PurpleBuddy *buddy)
{
	if( buddy->alias )
	{
		return buddy->alias;
	}
	else if( buddy->server_alias )
	{
		return buddy->server_alias;
	}
	else
	{
		return buddy->name;
	}
}


/**
 * @brief retreive the name that should appear on the UI based on the
 * @brief buddy.
 */
static const char * get_self_name(PurpleConversation *conv)
{
    static const char *gSelf = "";
    const char *self;
    PurpleAccount *account;

    account = purple_conversation_get_account(conv);
    if (!account) {
        return gSelf;
    }

    if (purple_conversation_get_type(conv) == PURPLE_CONV_TYPE_CHAT) {
        PurpleConvChat *chat;

        chat = PURPLE_CONV_CHAT(conv);
        self = purple_conv_chat_get_nick(chat);
        if (self) {
            return self;
        }
    }

/*    self = purple_account_get_alias(account);
    if (self) {
        return self;
    }*/

    self = purple_account_get_username(account);
    if (self) {
        return self;
    }

    return gSelf;
}


static const char * time_stamp()
{
    static char now[255];
    sprintf(now,"time=%u&",(unsigned)time(NULL) + rand());
    return now;
}

static char * get_host(const char *url)
{
	char *begin;
	char *end;

	if (!url)
		return g_strdup("");

	begin = strstr(url, "://");

	if (!begin)
		return g_strdup("");

	begin +=3;

	end = strstr (begin, "/");

	if (end == NULL)
		end += strlen(begin);

	return g_strndup(begin, end-begin);
}

//la real que se ocupa de los emails
static void notify_email_cb_real(char *subject, char *from, char *to, char *url, guint count, gboolean clear)
{
	gboolean exists=0;
	webpidgin_emails *em = g_malloc(sizeof(webpidgin_emails));
	GList *tmp = emails;
	webpidgin_emails *em2 = NULL;
	int max=0;

	// compruebo los datos
	if(url)
	{
		if (g_ascii_strncasecmp (url,"http",4))
			return;

		g_snprintf(em->url, sizeof(em->url), "%s", url);
	}
	else
		g_stpcpy(em->url, "");

	if(to)
		g_snprintf(em->to, sizeof(em->to), "%s",to);
	else
		return;

	em->count = count;
	em->count_new=0;

	max = g_list_length (purple_accounts_get_all ());

	purple_debug_misc("WebPidginZ notify_email_cb_real", "[%s](%d)\n", em->to, count);

	//busco si ya existe
	while (tmp && !exists && max--)
	{
		em2=tmp->data;
		purple_debug_misc("WebPidginZ notify_email_cb_real", "[%s] <-> [%s] %s\n", em->to, em2->to, em2->url);

		if (!strcmp (em2->to, em->to))
		{
			char *url1;
			char *url2;

			url1=get_host(em->url);
			url2=get_host(em2->url);
			purple_debug_misc("WebPidginZ notify_email_cb_realz2", "%s %s\n", url1, url2);

			if (!g_ascii_strcasecmp (url2, url1) || !strcmp (url2, ""))
			{
				exists=1;
			}
			else if (!strcmp (url1, ""))
			{
				g_snprintf(em->url, sizeof(em->url), "%s", em2->url);
				exists=1;
			}

			g_free(url1);
			g_free(url2);
		}

		if (!exists)
			tmp = tmp->next;
	}

	if (exists)
	{
		if (!clear)
		{
			em->count += em2->count;
			em->count_new = em2->count_new + 1;
		}

		emails = g_list_delete_link(emails, tmp);
	}

	emails = g_list_append (emails, em);
}

static void notify_email_cb(char *subject, char *from, char *to, char *url)
{
	purple_debug_misc("WebPidginZ signals test", "notify_email_cb\n");
	notify_email_cb_real(subject, from, to, url, 1, 0);
}

static void notify_emails_cb(char **subjects, char **froms, char **tos, char **urls, guint count)
{
	purple_debug_misc("WebPidginZ signals test", "notify_emails_cb\n");

	notify_email_cb_real(subjects ? *subjects : NULL, froms ? *froms  : NULL, tos ? *tos : NULL, urls ? *urls : NULL, count, 1);
}


static gint chat_invited_cb(PurpleAccount *account, const char *inviter,
				const char *room_name, const char *message,
				const GHashTable *components, void *data)
{
	purple_debug_misc("WebPidginZ signals test", "chat-invited (%s, %s, %s, %s)\n",
					purple_account_get_username(account), inviter,
					room_name, message);

	return 1;
}

static void auto_accept_complete_cb(PurpleXfer *xfer, PurpleXfer *my)
{		
	if (xfer) 
	{
		purple_xfer_ref(xfer);
		
		PurpleConversation *conv=purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, xfer->who, xfer->account);
		if(conv)
		{
			char *message = g_strdup_printf(_("Received file transfer of <a href='getFile?ftid=%p'>%s</a> %d completed."),xfer, xfer->filename, xfer->ref);
			purple_conversation_write(conv, "WebPidgin-Z", message, PURPLE_MESSAGE_SYSTEM , time(NULL));
			g_free(message);
		}		
	}
}



/**
 * @brief normalize a post-method encoded data to normal "ASCII"
 * @note FIXME: this is not the correct normalize function, just a quick hack
 */
static char * webpidgin_encode( const char * object )
{
    char * coded = g_malloc( (strlen(object)*3)+1 );
    int idx;
    int current = 0;

    memset(coded,0, (strlen(object)*3)+1 );
    for( idx=0;idx<strlen(object);idx++)
    {
        if( isalnum(object[idx]) )
        {
            coded[current] = object[idx];
            current+=1;
        }
        else
        {
            /** Encode reserved characters */
            switch( object[idx] ){
                case '\'':
                case '<':
                case '>':
                case '#':
                case '{':
                case '"':
                case '}':
                case '|':
                case '\\':
                case '^':
                case '~':
                case '[':
                case ']':
                case '`':
                case ' ':
                case '$':
                case '&':
                case '+':
                case ',':
                case '/':
                case ':':
                case ';':
                case '=':
                case '?':
                case '@':
                case '%':{
                    char hex[8];
                    purple_debug_info("WebPidgin 2.x","key [%c][%2x]\n",object[idx],object[idx]);
                    g_snprintf(hex,8,"%%%2x",object[idx]);
                    /* YUCK: but why loop? */
                    coded[current]   = hex[0];
                    coded[current+1] = hex[1];
                    coded[current+2] = hex[2];
                    current+=3;
                }break;

                default:
                    coded[current] = object[idx];
                    current+=1;
                break;
            }
        }
    }
    coded[current]='\0';
    purple_debug_info("WebPidgin 2.x","coded [%s]\n",coded);
    return coded;
}


/**
 * @brief normalize a post-method encoded data to normal "ASCII"
 * @note FIXME: this is not the correct normalize function, just a quick hack
 */
static char * webpidgin_normalize( const char * message )
{
    char * normal = g_malloc( strlen(message)*2+1 );
    int idx;
    int current = 0;
    for( idx=0;idx<strlen(message);idx++)
    {
        switch( message[idx] ){
            case '+':
                normal[current] = ' ';
            break;

            case '%':{
                unsigned ch=' ';;
                sscanf(&message[idx],"%%%2x",&ch);
                idx+=2;
                purple_debug_info("WebPidgin 2.x","Got [%d] [%c]\n",ch,(unsigned char)ch);
                normal[current]=(unsigned char)ch;
            }break;

            default:
                normal[current] = message[idx];
            break;
        }
        current++;
    }
    normal[current]='\0';
    purple_debug_info("WebPidgin 2.x","Normal [%s]\n",normal);
    return normal;
}

/**
 * @brief send a string ( const char* ) message to the http client socket
 * @brief for this session.  the message must be NULL ('\0' ) terminated as
 * @brief we use strlen to trunc the size.  We do not send the '\0'
 */
static ssize_t client_write( webpidgin_client_t * httpd, const char *buffer )
{
    /// FIXME: We need to LOOP_EINTR here
    //(void) write( httpd->fd,buffer,strlen(buffer));
    if (httpd->buffer_len > 0 && httpd->buffer != NULL)
    	return g_strlcat(httpd->buffer, buffer, httpd->buffer_len);
    else
    	return write(httpd->fd, buffer, strlen(buffer));
}

/// NEW IMPLEMENTATION WITH VARGS
static ssize_t client_write_vargs( webpidgin_client_t * httpd, const char* format, ... )
{
	va_list list;
	char *buffer;
	ssize_t ret;
	
	va_start( list, format);
	buffer=g_strdup_vprintf(format, list);	
	va_end(list);
		
	ret=client_write(httpd, buffer);
	
	g_free(buffer);
	return ret;
}

/**
 * @brief Get the current time and insert it into the client stream
 */
static void client_write_date(webpidgin_client_t * httpd)
{
	const char *rfc822_time = NULL;

    rfc822_time = purple_utf8_strftime("Date: %a, %d %b %Y %H:%M:%S %z\n", NULL);
    if (rfc822_time != NULL)
    {
        client_write(httpd, rfc822_time);
    }
    else
    {
        client_write(httpd,"Date: Sun, 21 Nov 2004 20:56:06 GMT\n");
    }
}

/**
 * @brief Write a HTTPD header and additional WebPidgin "toolbar" to the client stream
 */
static void client_write_http_header( webpidgin_client_t * httpd )
{
    client_write( httpd,"");
    client_write( httpd,"HTTP/1.1 200 OK\n");
    client_write_date(httpd);
    client_write( httpd,"Server: WebPidginZ\n");
    client_write( httpd,"Connection: close\n");
    client_write( httpd,"Content-type: text/html;charset=utf-8\n\n");
}

static void client_write_http_header_redirect( webpidgin_client_t * httpd , const char *url )
{
    client_write( httpd,"");
    client_write( httpd,"HTTP/1.1 303 See Other\n");
	client_write( httpd,"Location: ");
	client_write( httpd,url);
	client_write( httpd,"\n");
	client_write( httpd,"Content-type: text/html;charset=utf-8\n");
    client_write_date(httpd);
    client_write( httpd,"Server: WebPidginZ\n");
    client_write( httpd,"Connection: close\n");    
    client_write( httpd,"\n");
}



static void client_write_cmds( webpidgin_client_t * httpd, const char *update )
{
	char buffer[4096];

	if( gOptionWWWFrames )
    {
        if( ( strcmp(update,"/") == 0 ) || ( strcmp(update,"/ActiveList") == 0 ) )
        {
            /*if (gOptionFontAdjust >= 0) 
            {
                g_snprintf(buffer,sizeof(buffer),"<FONT SIZE=+%d>", gOptionFontAdjust);
            }else if(gOptionFontAdjust < 0) 
            {
                g_snprintf(buffer,sizeof(buffer),"<FONT SIZE=%d>", gOptionFontAdjust);
            }
            client_write(httpd, buffer);*/

            g_snprintf(buffer,sizeof(buffer),"<A HREF='Status' target=\"conv\">CMD</A>: <A HREF='%s' target=\"list\">U</A> | <A HREF='/Accounts' target=\"conv\">A</A> | <A HREF='/Options' target=\"conv\">O</A> | <A HREF='/Help' target=\"conv\">?</A><HR>\n",update);
        }
        else
        {
            /*if (gOptionFontAdjust >= 0) 
            {
                g_snprintf(buffer,sizeof(buffer),"<FONT SIZE=+%d>", gOptionFontAdjust);
            }else if(gOptionFontAdjust < 0) 
            {
                g_snprintf(buffer,sizeof(buffer),"<FONT SIZE=%d>", gOptionFontAdjust);
            }
            client_write(httpd, buffer);*/
            
            g_snprintf(buffer,sizeof(buffer),"CMD: <A HREF='%s' target=\"conv\">Update</A><HR> \n",update);
        }
    }
    else
    {
        //
        // Not framed; use the "Normal Way"
        //
        /*if (gOptionFontAdjust >= 0) {
            g_snprintf(buffer,sizeof(buffer),"<FONT SIZE=+%d>", gOptionFontAdjust);
        } else if(gOptionFontAdjust < 0) {
            g_snprintf(buffer,sizeof(buffer),"<FONT SIZE=%d>", gOptionFontAdjust);
        }
        client_write(httpd, buffer);*/

        g_snprintf(buffer,sizeof(buffer),"<A HREF='Status'>CMD</A>: <A HREF='%s'>&nbsp;U&nbsp;</A> | <A HREF='/'>&nbsp;H&nbsp;</A> | <A HREF='/Accounts'>&nbsp;A&nbsp;</A> | <A HREF='/Options'>&nbsp;O&nbsp;</A> | <A HREF='/Help'>&nbsp;?&nbsp;</A><HR>\n",update);
    }

    client_write( httpd,buffer);
}



static void client_write_header( webpidgin_client_t * httpd, const char *update )
{
    char buffer[4096];
    client_write_http_header( httpd );
    client_write( httpd,"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
    client_write( httpd,"<html>\n");
    client_write( httpd," <head>\n");    
    client_write( httpd," <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
    client_write( httpd," <meta http-equiv=\"Pragma\" content=\"no-cache\">\n");
    client_write( httpd,"  <link rel=\"shortcut icon\" href=\"favicon.ico\" type=\"image/x-icon\" />\n");
    
    client_write( httpd,"  <link rel=\"icon\" href=\"favicon.ico\" type=\"image/x-icon\" />\n");        
    
    client_write( httpd,"<style type='text/css'>");
    client_write( httpd,".logo{margin:5px 10px; float:left; border:1px solid black} #divError{}\n\
a {color:#01E} .msggroup{color:#fff; padding:2px 5px; -moz-border-radius:7px; } .divmsgme{background-color:#ddf;}\n\
#conversation{overflow:auto; padding:5px;margin:5px 0px; background-color:#f0f0f0; border:1px solid #ddd; }\n\
.divmsgsystem{background-color:#dedede} .divmsgg{-moz-border-radius:10px; padding:5px; margin:2px 3px 10px 3px;}\n\
.buddyin1{color:#66E} .buddyin2{color:#666} .buddycmz{color:#D55; font-weight:bold;} .taimsg{-wap-input-format: *m;}\n\
.divmsgother{background-color:#fdd} .buddyimg{float:right; border:3px outset #D55; margin:0px 10px; max-width:20%} .aClose{margin-right: 10px} ");
		
	client_write_vargs( httpd,"body{overflow:auto; font-size:%d%%}", (100 + gOptionFontAdjust*10));	
	
	client_write( httpd,"</style>");
	
	///custom css
	if (gUseCustomCSS)
		client_write( httpd,"  <link rel=\"stylesheet\" type=\"text/css\" href=\"custom.css\" />\n");    

    if ( gOptionMetaRefresh && !gUseJavascript )
    {
        if( gOptionMetaRefreshDynamic )
        {
            /// Dynamically adjust our web browser refresh speed up and down
            /// as messages start to come in
            if( gLastRefreshInterval == 0 )
                gLastRefreshInterval = gOptionMetaRefreshSeconds;

            if( gUnseenMessageCount > 0 )
            {
                gLastRefreshInterval = gLastRefreshInterval / DREFRESH_STEP;
                gMissedRefreshes = 0;
            }
            else if( gMissedRefreshes > 1 )
            {
                // Delay upstep by 1 refresh cycle
                gLastRefreshInterval = gLastRefreshInterval * DREFRESH_STEP;
                gMissedRefreshes = 0;
            }
            else
            {
                gMissedRefreshes++;
            }

            gUnseenMessageCount = 0;

            if( gLastRefreshInterval > gOptionMetaRefreshSeconds )
                gLastRefreshInterval = gOptionMetaRefreshSeconds;

            if( gLastRefreshInterval < DREFRESH_MIN )
                gLastRefreshInterval = DREFRESH_MIN;
        }
        else
        {
            gLastRefreshInterval = gOptionMetaRefreshSeconds;
        }

        // Only refresh on root, active, chat, and status pages
        if( ( strcmp(update,"/") == 0 ) ||
            ( strcmp(update,"/ActiveList") == 0 ) ||
            //We don't want chat auto refreshing when using frames
            ( ( strstr(update,"/chat") != NULL ) && ( gOptionWWWFrames == 0 ) ) ||
            ( strstr(update,"/Status") != NULL ) || (strstr(update,"/conversation") != NULL ))
        {
            g_snprintf(buffer,sizeof(buffer),"  <meta http-equiv=\"refresh\" content=\"%d\">\n",gLastRefreshInterval);
            client_write( httpd,buffer);
        }
    }
    client_write( httpd,"  <title>WebPidginZ</title>\n");
    client_write( httpd," </head>\n");

    //
    //  If we Are In Frames We Behave Differently!!
    //
    if( gOptionWWWFrames && ( strcmp(update,"/") == 0  || strcmp(update,"/ActiveList") == 0 ) )
        client_write( httpd,"<body bgcolor=#EEEEEE>\n");
    else
        client_write( httpd,"<body>\n");

	client_write_cmds( httpd, update );
}

/**
 * @brief write end of the httpd stream
 */
static void client_write_tail( webpidgin_client_t * httpd )
{
    //client_write(httpd, "</FONT>");
    client_write( httpd,"</body>\n");
    client_write( httpd,"</html>\n");
    client_write( httpd,"\n\n");
}


static void webpidgin_show_buddy(webpidgin_client_t * httpd,const char * extra_html,PurpleBuddy *buddy )
{
    char buffer[4096];    
    char * name = g_markup_escape_text (buddy->name, -1);
    const char * alias;
	PurpleStatus *status;
    const char *status_msg = NULL;
    char extra[128]= "";
    char extra3[128]= "";
    time_t idle_secs ;

	//const char * proto = purple_account_get_protocol_name( buddy->account );
	//client_write(httpd, proto);

	alias =get_buddy_name(buddy);

    /* Retrieve and display status message, if one exists */
    status = purple_presence_get_active_status(buddy->presence);

	if (!purple_presence_is_available(buddy->presence))
	{
		client_write(httpd, " <span class='buddy_not_avail'> ");
		g_snprintf(extra, sizeof(extra), " class='buddy_not_avail' ");
	}
    else // buddy is available - use a different CSS class
    {
		client_write(httpd, " <span class='buddy_avail'> ");
		g_snprintf(extra, sizeof(extra), " class='buddy_avail' ");
    }        

    if (gGroupMessages)
		g_snprintf(extra3, sizeof(extra3), "#isend");

    //g_snprintf(buffer,sizeof(buffer),"&nbsp;&nbsp; <A HREF=\"conversation?%s%s%s\" %s %s>%s</A>", time_stamp(), name, extra3, extra, extra_html, alias);
    //g_snprintf(buffer,sizeof(buffer),"&nbsp;&nbsp; <A HREF=\"conversation?%s%s\" %s %s>%s</A>", name, extra3, extra, extra_html, alias);
    g_snprintf(buffer,sizeof(buffer),"&nbsp;&nbsp; <A HREF=\"conversation?bd=%p%s\" title=\"%s\" %s %s>%s</A>", buddy, extra3, name, extra, extra_html, alias);
    client_write(httpd,buffer);

    if (gOptionStatusMessages)
    {
    	PurpleValue* value= purple_status_get_attr_value(status, "message");
    	if (value)
    	{
			status_msg = purple_value_get_string(value);
			if ((status_msg != NULL) && (*status_msg != '\0')) {
				char *stripped_status_msg = purple_markup_strip_html(status_msg);
				client_write(httpd," ");
				client_write(httpd,stripped_status_msg);
				g_free(stripped_status_msg);
			}
    	}
    }

    /* Idle - lifted largely from gaim/src/gtkblist.c */
    //purple_debug_info("WebPidgin 2/SHAOZZZ", "%s -> %p \n", alias, buddy);


    if (purple_presence_is_idle(buddy->presence) && (idle_secs = purple_presence_get_idle_time(buddy->presence))>0)
    {
        //time_t idle_secs = purple_presence_get_idle_time(buddy->presence);
        //if (idle_secs > 0) {

            int ihrs, imin;
            time_t t;

            time(&t);
            purple_debug_info("WebPidgin 2","Buddy %s has been idle for %d sec\n", buddy->name, (int) (t - idle_secs));
            ihrs = (t - idle_secs) / 3600;
            imin = ((t - idle_secs) / 60) % 60;

            if (ihrs)
                g_snprintf(buffer,sizeof(buffer)," (Idle %dh %02dm)", ihrs, imin);
            else
                g_snprintf(buffer,sizeof(buffer)," (Idle %dm)", imin);

        /*}
        else
            g_snprintf(buffer,sizeof(buffer)," (Idle)");
*/

        client_write(httpd,buffer);
    } else if (!purple_presence_is_available(buddy->presence))
    {		
		client_write_vargs(httpd, " (%s)", purple_status_get_name (status));
    }

    //if (!purple_presence_is_available(buddy->presence))
    client_write(httpd, " </span> ");

    client_write(httpd,"<BR>\n");

    g_free(name);
}


static void webpidgin_buddy_list_walk( webpidgin_client_t * httpd,const char * extra_html,GtkTreeModel *model,  GtkTreeIter * parent)
{
    char buffer[4096];
    GValue val;
    PurpleBlistNode *node;

    do{
        val.g_type = 0;
        gtk_tree_model_get_value (model, parent, NODE_COLUMN, &val);
        node = g_value_get_pointer(&val);
        switch (node->type)
        {
            case PURPLE_BLIST_GROUP_NODE:{
                GtkTreeIter child;
                PurpleGroup *group = (PurpleGroup*) node;
                g_snprintf(buffer,sizeof(buffer),"<B>%s</B><BR>\n",group->name);
                client_write(httpd,buffer);

                if( gtk_tree_model_iter_children(model,&child,parent) )
                    webpidgin_buddy_list_walk( httpd,extra_html,model,&child);
            }break;


            case PURPLE_BLIST_CONTACT_NODE:{
                PurpleContact *contact = (PurpleContact *)node;
                if ( contact->alias )
                {
                    /// not sure yet
                    //g_snprintf(buffer,sizeof(buffer),"<B>type=%d ( not sure )</B><BR>\n",node->type);
                    //client_write(httpd,buffer);
                }
                else
                {
                    PurpleBuddy *buddy = purple_contact_get_priority_buddy(contact);
                    webpidgin_show_buddy( httpd, extra_html, buddy );
                }
            }break;

            case PURPLE_BLIST_BUDDY_NODE:{
                PurpleBuddy *buddy = (PurpleBuddy*) node;
                webpidgin_show_buddy( httpd, extra_html, buddy );
            }break;

            case PURPLE_BLIST_CHAT_NODE:{
                PurpleChat * chat = (PurpleChat*)node;
                const char * name = purple_chat_get_name(chat);
                //g_snprintf(buffer,sizeof(buffer),"&nbsp;&nbsp; <A HREF=conversation?%s%s %s>%s</A><BR>",time_stamp(),name,extra_html,name);
                client_write_vargs(httpd, "&nbsp;&nbsp; <A HREF=conversation?%s%s %s>%s</A><BR>",time_stamp(),name,extra_html,name);
            }break;

            default :
                //g_snprintf(buffer,sizeof(buffer),"<B>type=%ds</B><BR>\n",node->type);
                client_write_vargs(httpd, "<B>type=%ds</B><BR>\n",node->type);
            break;
        }
    }while( gtk_tree_model_iter_next(model,parent) );
}


//SHOW SESSIONS
static void show_last_sessions( webpidgin_client_t * httpd, int nsessions )
{
	char buffer[1024];
    GSList * gpointer;
	webpidgin_session * session;
	int i = 0;

	if (nsessions == INT_MAX)
		g_snprintf(buffer,1024,"<B>Last sessions:</B><BR />\n");
	else
		g_snprintf(buffer,1024,"<B>Last %d sessions:</B><BR />\n", nsessions );
	client_write(httpd,buffer);

	client_write(httpd,"<ul>");
	gpointer = last_sessions;
	while (gpointer && (i < nsessions))
	{
		char color[10];
		char extra[256]="";

		session = (webpidgin_session *) gpointer->data;

		if (session->auth_ok)
			g_snprintf (color, sizeof (color), "#5555DD");
		else
		{
			if (! gShowNoAuthSessions)
			{
				gpointer = g_slist_next(gpointer);
				continue;
			}

			g_snprintf (color, sizeof (color), "#DD5555");
			g_snprintf (extra, sizeof (extra), "<font color='%s'><u><b> (NO AUTH) </b></u></font> ", color);
		}

		g_snprintf(buffer,1024,"<li><font size=-1><font color='#888888'>(%s)</font> %s <font color='%s'><b>%s</b> ", purple_date_format_long(localtime ( & session->last_connection)), extra, color, session->ip_client);
		client_write(httpd,buffer);

		if (session->ip_client_xff)
		{
			g_snprintf(buffer,1024," ( %s ) ", session->ip_client_xff);
			client_write(httpd,buffer);
		}

		if (session->user_agent)
		{
			g_snprintf(buffer,1024," [<i> %s </i>] ", session->user_agent);
			client_write(httpd,buffer);
		}

		client_write(httpd, "</font></font>");
		client_write(httpd, "</li>\n");


		//pidgin_blist_set_headline  	("<a href='http://127.0.0.1:8888'>link</a>", NULL, NULL, NULL, NULL);


		gpointer = g_slist_next(gpointer);
		i++;
	}
	client_write(httpd,"</ul>");

	if (i == nsessions && gpointer)
	{
		client_write(httpd, "<a href='Sessions'>More...</a><BR />\n");
	}

	client_write(httpd, "<HR />\n");
}


/**
 * @brief list all buddies that have active chat sessions and how many unread mesages we have
 * @brief (x) BuddyName [n] :: Where x is #unread messages, and n is a quick access key for mobile phones
 */
static void show_active_chats( webpidgin_client_t * httpd, const char * except )
{
	const char *active_chats= get_active_chats(NULL);
	client_write( httpd,active_chats);
	//g_free(active_chats);
}




static const char* get_active_chats(char * hash)
{
	static char ret[4096];
    char buffer[4096];
    char extra_html[512];
    GList *cnv;
    char bufferhash[4096]="";
    char *punt;

	if (hash != NULL)
		strcpy(hash, "undefined");

    strcpy(extra_html, "");

    //ret = g_malloc(tamanioret);
    strcpy(ret, "");
    punt = ret;

    if (!purple_get_conversations())
    	return ret;

    if( gOptionWWWFrames )
    {
        strncat(extra_html," target=\"conv\" ",511-strlen(extra_html));
    }

    /** Now see if what active conversations we have */
    g_strlcat(ret, "<B>Active:</B><BR>", sizeof(ret));

    //purple_debug_info("WebPidgin 2-SHAO","%s\n",ret);

    for (cnv = purple_get_conversations(); cnv != NULL; cnv = cnv->next)
    {
        PurpleConversation *conv;
        const char * encoded_name = NULL;
		const char * encoded_title = NULL;
        const char *name = NULL;
		const char *title = NULL;
        unsigned totalm = 0;
        char extra[128] = "";
        char extra2[128] = "";
        char extra3[128] = "";
        unsigned unseen_count=0;

        PurpleBuddy * buddy;
		PurpleStatus *status = NULL;

        conv = (PurpleConversation *)cnv->data;
        name =  purple_conversation_get_name(conv);
		title =  purple_conversation_get_title(conv);
        encoded_name = purple_url_encode( name );
		encoded_title = purple_url_encode( title );
        if (!encoded_name || !encoded_title)
            continue;

		///El puntero para calcular el hash ignorando los cambios de estado
		g_strlcat(bufferhash, punt, sizeof(bufferhash));


		buddy = find_buddy( (char*) name);
		if (!buddy)
			buddy = find_buddy( (char*) title);

		if (buddy)
			status = purple_presence_get_active_status(buddy->presence);

		if (buddy && !purple_presence_is_available(buddy->presence))
		{
			g_snprintf(extra, sizeof(extra), " class='buddyin2' ");
			g_snprintf(extra2, sizeof(extra2), " class='buddyin1' ");
		}

		//purple_debug_info("WebPidgin 2-SHAO","%s %d\n",purple_conversation_get_title(conv), conv->type);

        totalm = g_list_length (conv->message_history);

		if (gGroupMessages)
			g_snprintf(extra3, sizeof(extra3), "#isend");


		unseen_count= GPOINTER_TO_INT(purple_conversation_get_data(conv, "webpidgin_unseen_count"));
		if (totalm >= unseen_count)
			unseen_count=totalm - unseen_count;
		else
			unseen_count=0;

		if (unseen_count > 0)
			g_snprintf(extra2, sizeof(extra2), " class='buddycmz' ");

		///concateno antes del cambio de estado
		g_strlcat(bufferhash, punt,sizeof(bufferhash));

		g_snprintf(buffer,sizeof(buffer)," \n<div><A class=\"aClose\" HREF=\"sendMessage?to=id=%p&msg=%%2Fwp+quit\" title=\"Close\">X</A> ", conv);
		g_strlcat(ret, buffer, sizeof(ret));

        g_snprintf(buffer,sizeof(buffer),"<span %s>", extra);
        g_strlcat(ret, buffer, sizeof(ret));

        g_snprintf(buffer,sizeof(buffer),"<A HREF=\"conversation?id=%p%s\" %s title=\"%s\" %s>%s</A> (%d/%d)", conv, extra3, extra2, name, extra_html, title, unseen_count, totalm);
        g_strlcat(ret, buffer, sizeof(ret));

		///concateno sin contar los cambios de estado para el hash
		g_snprintf(buffer,sizeof(buffer),"<A HREF=\"conversation?id=%p%s\" title=\"%s\" %s>%s</A> (%d/%d)", conv, extra3, name, extra_html, title, unseen_count, totalm);
        g_strlcat(bufferhash, buffer, sizeof(bufferhash));



	    if (buddy && !purple_presence_is_available(buddy->presence))
	    {
			g_snprintf (buffer, sizeof(buffer), " (%s) ", purple_status_get_name (status));
			g_strlcat(ret, buffer, sizeof(ret));
		}

/*
		g_snprintf(buffer,sizeof(buffer)," <A HREF=\"sendMessage?to=id=%p&msg=%%2Fwp+quit\" title=\"Close\">X</A> ", conv);
		g_strlcat(ret, buffer, sizeof(ret));
		*/

		g_strlcat(ret, "</span></div>\n", sizeof(ret));

		///AUMENTO El puntero para calcular el hash ignorando los cambios de estado		
		punt += strlen(punt);			
		
    }

    g_strlcat(ret, "<HR>", sizeof(ret));

    ///El puntero para calcular el hash ignorando los cambios de estado
	g_strlcat(bufferhash, punt,sizeof(bufferhash));

	//purple_debug_info("WebPidgin 2-SHAOMMAB","%s \n", g_strescape (bufferhash, ""));

	if (hash != NULL)
	{
		//char *md5=g_compute_checksum_for_string(G_CHECKSUM_MD5, bufferhash, -1);

		//strcpy(hash, md5);

		//g_free(md5);

		strcpy(hash, md5sum(bufferhash));
		//purple_debug_info("WebPidgin 2-SHAOMMAB2","%s \n",hash);
	}

    return ret;
}


static void client_write_last_update( webpidgin_client_t * httpd )
{
    if( gOptionMetaRefresh )
    {
        char buffer[1280];
        time_t tme = time(NULL);
        struct tm *tm = localtime( &tme );
        g_snprintf(buffer,128,"%d:%02d:%02d&nbsp;(%d)",tm->tm_hour,tm->tm_min,tm->tm_sec,gLastRefreshInterval);

        client_write(httpd,buffer);
    }
}

static void show_active_list( webpidgin_client_t * httpd)
{
	GaimGtkBuddyList *gtkblist = gaim_gtk_blist_get_default_gtk_blist();
    GtkTreeIter iter;
    char extra_html[512];


    strcpy(extra_html,"");
    if( gOptionWWWFrames )
    {
        strncat(extra_html," target=\"conv\" ", 511-strlen(extra_html));
    }

    /// Walk through all the buddy lists and display them
    /// Our buddy display should now match whatever Pidgin chooses :)
    if( gtk_tree_model_get_iter_first(GTK_TREE_MODEL(gtkblist->treemodel),&iter) )
    {
        webpidgin_buddy_list_walk(httpd,extra_html,GTK_TREE_MODEL(gtkblist->treemodel),&iter);
    }
}


static int action_active_list( webpidgin_client_t * httpd, const char * notused )
{
    purple_debug_info("WebPidgin 2","%s\n",__FUNCTION__);

    client_write_header( httpd,"/ActiveList" );


    client_write(httpd,"<div id='active_chats'>\n");
    show_active_chats( httpd , NULL);
    client_write(httpd,"</div>\n");

	client_write(httpd,"<div id='active_list'>\n");
    show_active_list(httpd);
    client_write(httpd,"</div>\n");

	client_write(httpd,"<HR>");
	
	if (!gOptionWWWFrames)
		show_last_sessions (httpd, gShowNLastSessions);


    if (gUseJavascript)
    {
    	char buffer[1024];

	    show_ajax_engine(httpd);
	    client_write(httpd,"<script type='text/javascript'>");

		client_write(httpd,"window.onload=function(e){\n");
		
		g_snprintf(buffer,sizeof(buffer), "update(new Array ('active_list', 'active_chats'), \"\");");
		client_write(httpd, buffer);

		client_write(httpd,"}\n");

		client_write(httpd,"</script>");
	}

    client_write_last_update( httpd );
    client_write_tail( httpd );
    return 1;
}


/**
 * @brief Given a WWW encoded post string, see if a "checkbox" is set with the given name
 */
static unsigned webpidgin_get_param_bool( const char *data, const char * param )
{
    char *key = NULL;
    char search[255];
    g_snprintf(search,255,"%s=",param); /// in forms, the = always follows making this key unique

    /// If a form object has the named key then we are indeed selected
    key = strstr(data, search );
    if( ! key )
        return 0;

    return 1;
}

static int webpidgin_get_param_int( const char *data, const char * param )
{
	char *key = NULL;
	char search[255];
    int value = 0;
	g_snprintf(search,255,"%s=",param); /// in forms, the = always follows making this key unique

    /// If a form object has the named key then we are indeed selected
    key = strstr(data, search );
    if( ! key )
        return 0;

    g_snprintf(search,255,"%s=%%d&",param); /// in forms, the = always follows making this key unique
    if( sscanf(key,search,&value) != 1)
        return 0;

    purple_debug_info("WebPidgin 2","search=[%s] in=[%s] value=[%d]\n",search,key,value);

    return value;
}


static int action_options( webpidgin_client_t * httpd, const char * extra )
{
    char buffer[1024];

    if( extra != NULL && (strlen(extra) > 0 ) )
    {
        int value;

        //client_write(httpd,"Extra:");
        //client_write(httpd,extra);
        ///
        /// Parse the options we just got in
        ///
        /// Bool Prefs:   use_color,use_bold_names,use_www_frames,use_status_messages,use_meta_refresh
        /// String Prefs: server_user,server_password,server_port
        /// Int Prefs:    meta_refresh_seconds
        ///
        webpidgin_save_pref_bool( "use_color",          webpidgin_get_param_bool( extra,"use_color") );
        webpidgin_save_pref_bool( "use_bold_names",     webpidgin_get_param_bool( extra,"use_bold_names") );
        webpidgin_save_pref_bool( "use_status_messages",webpidgin_get_param_bool( extra,"use_status_messages") );
        webpidgin_save_pref_bool( "use_www_frames",     webpidgin_get_param_bool( extra,"use_www_frames") );
        webpidgin_save_pref_bool( "use_meta_refresh",   webpidgin_get_param_bool( extra,"use_meta_refresh") );
        webpidgin_save_pref_bool( "use_meta_refresh_dynamic",   webpidgin_get_param_bool( extra,"use_meta_refresh_dynamic") );
        webpidgin_save_pref_bool( "use_rss_feed",   webpidgin_get_param_bool( extra,"use_rss_feed") );
        webpidgin_save_pref_bool( "save_connection_log",   webpidgin_get_param_bool( extra,"save_connection_log") );
        webpidgin_save_pref_bool( "show_no_auth_sessions",   webpidgin_get_param_bool( extra,"show_no_auth_sessions") );
        webpidgin_save_pref_bool( "use_jscript",   webpidgin_get_param_bool( extra,"use_jscript") );
        webpidgin_save_pref_bool( "use_jscript_only_refresh",   webpidgin_get_param_bool( extra,"use_jscript_only_refresh") );
        webpidgin_save_pref_bool( "group_messages",   webpidgin_get_param_bool( extra,"group_messages") );
        webpidgin_save_pref_bool( "use_digest_auth",   webpidgin_get_param_bool( extra,"use_digest_auth") );
        webpidgin_save_pref_bool( "show_buddy_icon",   webpidgin_get_param_bool( extra,"show_buddy_icon") );
        webpidgin_save_pref_bool( "show_smileys",   webpidgin_get_param_bool( extra,"show_smileys") );
        webpidgin_save_pref_bool( "use_sounds",   webpidgin_get_param_bool( extra,"use_sounds") );
        webpidgin_save_pref_bool( "use_custom_css",   webpidgin_get_param_bool( extra,"use_custom_css") );

        value = webpidgin_get_param_int( extra,"meta_refresh_seconds");
        if( value >=1 && value <= 3600 )
        {
            webpidgin_save_pref_int( "meta_refresh_seconds", value );
        }

        value = webpidgin_get_param_int( extra,"font_adjust");
        if( value >=-10 && value <= 10 )
        {
            webpidgin_save_pref_int( "font_adjust", value );
        }

		value = webpidgin_get_param_int( extra,"show_n_last_sessions");
        if( value >=1 && value <= 10 )
        {
            webpidgin_save_pref_int( "show_n_last_sessions", value );
        }


        value = webpidgin_get_param_int( extra,"max_messages");
        if( value >=10 && value <= 20000 )
        {
            webpidgin_save_pref_int( "max_messages", value );
        }

        webpidgin_load_pref();
        //return action_root(httpd,NULL); // return to root page after preferences saved & reloaded
        client_write_http_header_redirect(httpd, "/");
        return 1;
    }

    client_write_header( httpd,"/Options" );
    client_write(httpd,"<B>WebServer Options:</B><BR>\n");
    client_write(httpd,"<form method=\"get\" target=\"_top\" action=\"/Options?\">\n");
    client_write(httpd,"<div>\n");

    ///
    /// Don't let people change their username and password
    /// over the web
    ///
    client_write(httpd,"&nbsp;&nbsp;Username: N/A<BR>\n");
    client_write(httpd,"&nbsp;&nbsp;Password: N/A<BR>\n");
    
    g_snprintf(buffer,1024,"&nbsp;&nbsp;Only allow access from the next IP/host: ( %s )  N/A<BR>\n", gOptionRestrictAccessByIP ? gAllowedIP : "option disabled");    
    client_write(httpd, buffer);
    
    client_write(httpd,"<BR>\n");

    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_digest_auth %s>Use digest authentication instead of basic authentication<BR>\n",gOptionUseDigestAuth ? "checked" : "");
    client_write(httpd,buffer);

    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_rss_feed %s>Enable RSS Feed<BR>\n",gOptionRSSFeed ? "checked" : "");
    client_write(httpd,buffer);

	g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=save_connection_log %s>Save a connection log in .purple directory<BR>\n",gSaveConnectionLog ? "checked" : "");
    client_write(httpd,buffer);


	client_write(httpd,"<BR><B>Extra Options:</B><BR>\n");

    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_color %s>Color Coded History<BR>\n",gOptionUseColor ? "checked" : "");
    client_write(httpd,buffer);

    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_bold_names %s>Bold Buddy Names<BR>\n",gOptionBoldNames ? "checked" : "");
    client_write(httpd,buffer);

    g_snprintf(buffer,1024,"&nbsp;&nbsp;&nbsp;&nbsp;Font Size Adjustment: <input type=text value=\"%d\" size=4 maxlength=4 name=font_adjust>(+1,+2,-1,-2 etc)<BR>\n",gOptionFontAdjust);
    client_write(httpd,buffer);

    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_status_messages %s>Buddy Status Messages<BR>\n",gOptionStatusMessages ? "checked" : "" );
    client_write(httpd,buffer);
    
    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=show_buddy_icon %s>Show buddy icon<BR>\n",gOptionShowBuddyIcon ? "checked" : "" );
    client_write(httpd,buffer);
    
    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=show_smileys %s>Show smileys<BR>\n",gOptionShowSmileys ? "checked" : "" );
    client_write(httpd,buffer);

    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_www_frames %s>Enable Frames<BR>\n",gOptionWWWFrames ? "checked" : "");
    client_write(httpd,buffer);

    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_meta_refresh %s>Auto-refresh buddy list/status (unset this if javascript enabled)<BR>\n",gOptionMetaRefresh ? "checked" : "");
    client_write(httpd,buffer);

	client_write(httpd, "<div style='padding-left:20px'>");

    g_snprintf(buffer,1024,"&nbsp;&nbsp;&nbsp;&nbsp;Seconds: <input type=text value=\"%d\" size=4 maxlength=4 name=meta_refresh_seconds><BR>\n",gOptionMetaRefreshSeconds);
    client_write(httpd,buffer);


    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_meta_refresh_dynamic %s>Dynamic Refresh<BR>\n",gOptionMetaRefreshDynamic ? "checked" : "");
    client_write(httpd,buffer);

	client_write(httpd, "</div>");

    g_snprintf(buffer,1024,"&nbsp;&nbsp;&nbsp;&nbsp;Last sessions to show<input type=text value=\"%d\" size=4 maxlength=4 name=show_n_last_sessions><BR>\n",gShowNLastSessions);
    client_write(httpd,buffer);

	client_write(httpd, "<div style='padding-left:20px'>");
	
    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=show_no_auth_sessions %s>Show non-authenticated sessions<BR>\n",gShowNoAuthSessions ? "checked" : "");
    client_write(httpd,buffer);
    
    client_write(httpd, "</div>");

    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_jscript %s>Use Javascript(Ajax) in conversations (increases the ammount of data to transfer)<BR>\n",gUseJavascript ? "checked" : "");
    client_write(httpd,buffer);

	client_write(httpd, "<div style='padding-left:20px'>");
    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_jscript_only_refresh %s>Don't use Ajax when sending messages (solves some issues with slow connections and mobile browsers)<BR>\n",gUseJSOnlyRef? "checked" : "");
    client_write(httpd,buffer);    
    client_write_vargs(httpd, "&nbsp;&nbsp;<input type=checkbox name=use_sounds %s>Enable sounds (requires Flash 8.0 or later)<BR>\n",gUseSounds? "checked" : "");
    client_write(httpd, "</div>");

    g_snprintf(buffer,1024,"&nbsp;&nbsp;&nbsp;&nbsp;Trim conversations after<input type=text value=\"%d\" size=5 maxlength=5 name=max_messages>messages<BR>\n",uOptionTrim);
    client_write(httpd,buffer);

    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=group_messages %s>Group messages in conversations<BR>\n", gGroupMessages ? "checked" : "");
    client_write(httpd,buffer);
    
    g_snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_custom_css %s>Use custom.css file<BR>\n",gUseCustomCSS ? "checked" : "");
    client_write(httpd,buffer);

    client_write(httpd,"<BR>\n");
    client_write(httpd,"&nbsp;&nbsp;<input type=submit name=submit value=Save>\n");
    client_write(httpd,"</div>\n");
    client_write(httpd,"</form>\n");

    client_write_tail( httpd );
    return 1;
}



static int action_accounts( webpidgin_client_t * httpd, const char * notused )
{
    int accountCount = 0;
    char buffer[4096]; 
    GList *account_iter = NULL;
    client_write_header( httpd,"/Accounts" );
    client_write(httpd,"Accounts:<BR>\n");

    for (account_iter = purple_accounts_get_all() ; account_iter != NULL ; account_iter = account_iter->next)
    {
        char *encoded_user = NULL;
        const char * user  = purple_account_get_username( ( (PurpleAccount*) account_iter->data ) );
        const char * proto = purple_account_get_protocol_name( ( (PurpleAccount*) account_iter->data ) );
        char display[1024];

        accountCount++;
        g_snprintf(display,1024,"%s (%s)",user,proto);

        encoded_user = webpidgin_encode( user );
        if( encoded_user == NULL )
            return 0;

        if( purple_account_is_connected( (PurpleAccount*) account_iter->data ) )
        {
            purple_debug_info("WebPidgin 2","[%s] connected\n",display);
            g_snprintf(buffer,sizeof(buffer),"&nbsp;<A HREF=logout?%s&%s target=\"_top\">(logout)</A> %s<BR>\n",
                                 encoded_user,proto,display);

            client_write(httpd,buffer);
        }
        else
        {
            purple_debug_info("WebPidgin 2","[%s] disconnected\n",display);
            g_snprintf(buffer,sizeof(buffer),"&nbsp;<A HREF=login?%s&%s target=\"_top\">(login)</A> %s<BR>\n",
                            encoded_user,proto,display);
            client_write( httpd,buffer);
        }
        g_free(encoded_user);
    }


    GList *tmp=emails;
    while (tmp)
    {
    	webpidgin_emails *em = tmp->data;
    	g_snprintf(buffer,1280,"c:%d/%d u:%s <a href='%s'>url</a><br>", em->count_new, em->count, em->to, em->url);
    	client_write(httpd,buffer);
    	tmp=tmp->next;
    }


    client_write_tail( httpd );
    return 1;
}


/**
 * @brief Display the / ( Home ) page of WebPidgin.
 * @brief this mainly displays account status, online and active buddies
 */
static int action_root( webpidgin_client_t * httpd, const char * notused )
{
    if( gOptionWWWFrames )
    {
        client_write_http_header( httpd );
        client_write( httpd,"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
        client_write( httpd,"<html>\n");
        client_write( httpd," <head>\n");
        client_write( httpd,"  <title>WebPidginZ</title>\n");
        client_write( httpd,"  <link rel=\"shortcut icon\" href=\"/favicon.ico\" type=\"image/x-icon\" />\n");    
        client_write( httpd,"  <noframes><body>Your browser can not handle frames.<BR>\n");
        client_write( httpd,"  Please <A HREF=/Options>click here</A> and uncheck Enable Frames</body></noframes>\n");
        client_write( httpd,"   <FRAMESET COLS=\"20%, *\" BORDER=1 ID=fs1>");
        client_write( httpd,"    <FRAME SRC=\"/ActiveList\" NAME=\"list\">");
        client_write( httpd,"    <FRAME SRC=\"/Status\" NAME=\"conv\">");
        client_write( httpd,"   </FRAMESET>");
        client_write( httpd," </head>\n");
        client_write( httpd,"</html>\n");
        client_write( httpd,"\n\n");
    }
    else
    {
        return action_active_list( httpd,  notused );
    }
    return 1;
}


/**
 * @brief Find a PurpleAccount object based on a buddy name and account type
 */
static PurpleAccount* find_account( const char * from )
{
    GList *account_iter = NULL;
    const char * findName = NULL;
    char * findProto = NULL;

    findName = from;
    findProto = strstr(from,"&");
    if( !findProto )
        return NULL;

    *findProto='\0';
    findProto++;

    for (account_iter = purple_accounts_get_all() ; account_iter != NULL ; account_iter = account_iter->next)
    {
        const char * user  = purple_account_get_username( ( (PurpleAccount*) account_iter->data ) );
        const char * proto = purple_account_get_protocol_name( ( (PurpleAccount*) account_iter->data ) );

        if( ( strcmp(user,findName) == 0 ) && ( strcmp(proto,findProto) == 0 ) )
            return ((PurpleAccount*) account_iter->data );
    }
    return NULL;
}

/**
 * @brief handle a click on the (Login) account link to login to an account
 */
static int action_login( webpidgin_client_t * httpd, const char * extra )
{
    char * decoded = NULL;
    PurpleAccount * account;
    purple_debug_info("WebPidgin 2","%s (%s)\n",__FUNCTION__,extra);

    decoded = webpidgin_normalize( extra );
    account=find_account(decoded);
    g_free(decoded);
    if( account )
    {
        purple_account_set_enabled(account, PIDGIN_UI, TRUE);
        purple_account_connect( account );
    }
/*
    /// FIXME: Purple needs (a long) time to login...
#ifdef _WIN32
    Sleep(5000);
#else
    usleep(5000);
#endif
*/
    //return action_root(httpd,NULL); // return to root page
    client_write_http_header_redirect(httpd, "/");
	return 1;
}

/**
 * @brief handle a click on the (Logout) account link to logout of an account
 */
static int action_logout( webpidgin_client_t * httpd, const char * extra )
{
    char * decoded = NULL;
    PurpleAccount * account;
    purple_debug_info("WebPidgin 2","%s (%s)\n",__FUNCTION__,extra);
    decoded = webpidgin_normalize( extra );
    account=find_account(decoded);
    g_free(decoded);
    if( account )
    {
        purple_account_disconnect( account );
        purple_account_set_enabled(account, PIDGIN_UI, FALSE);
    }
    
    //return action_root(httpd,NULL); // return to root page
    client_write_http_header_redirect(httpd, "/");
	return 1;
}


static void show_conversation ( webpidgin_client_t * httpd, PurpleConversation * conv)
{
	char buffer[1024];
	GList *iter;
    const char * self;
    unsigned msgCount=0;
    char lastNick [256]="";
    char lastDate [256]="";
    char lastStyleNick [256]="";


    /// Now if this is a chat room, get a list of people in the chat:
    purple_conversation_get_type(conv);
    if (purple_conversation_get_type(conv) == PURPLE_CONV_TYPE_CHAT)
    {
        PurpleConvChat *chat;
        const char* topic;
        
        chat = PURPLE_CONV_CHAT(conv);
        topic=purple_conv_chat_get_topic(chat);

		//purple_debug_info("WebPidgin 2-TOPIXXX","(%s)\n",purple_conv_chat_get_topic(chat));
		

		if (topic && strlen(topic)>0)
		{        
			g_snprintf(buffer,1024,"<B>Topic:</B>&nbsp;%s<BR>", topic);
			client_write(httpd, buffer);
		}
        
        client_write(httpd, "<B>Members:</B>");

        for (iter=purple_conv_chat_get_users(chat);iter!=NULL;iter=iter->next)
        {
            PurpleConvChatBuddy *buddy;
            buddy = (PurpleConvChatBuddy *) iter->data;
            
            g_snprintf(buffer,1024,"&nbsp;%s%s", (buddy->alias)?(buddy->alias):(buddy->name), (iter->next)?", ":"");
            client_write(httpd, buffer);
        }
    }
    //client_write(httpd, "<HR>");

	self =  get_self_name(conv);


	iter=purple_conversation_get_message_history(conv);

	purple_conversation_set_data(conv, "webpidgin_unseen_count", GUINT_TO_POINTER(g_list_length (conv->message_history)));

	if (gGroupMessages)
	{
		if (uOptionTrim > g_list_length(iter))
			iter=g_list_last (iter);
		else
			iter=g_list_nth (iter, uOptionTrim-1);
	}

	while (iter!=NULL && msgCount<uOptionTrim)
    {
        PurpleConvMessage *msg = iter->data;
        time_t when;
        struct tm *tm;
        char stylenick[150] = "";
        char stylemsg[150] = "";
        char nick [256]= "";
        char date [256]= "";        


        purple_debug_info("WebPidgin 2","who=%s self=%s\n", msg->who, self);
        if (gOptionUseColor)
        {           	         					
			///I EXPECT THIS WILL WORK FINE
			if (purple_conversation_message_get_flags(msg) & PURPLE_MESSAGE_SEND)
			{
                g_snprintf(stylenick,sizeof(stylenick),"color:%s", array_colors[1]);
                g_snprintf(stylemsg,sizeof(stylemsg),"divmsgme");
            } 
            else if (purple_conversation_message_get_flags(msg) & PURPLE_MESSAGE_RECV)
            {            								
				int nbuddy=2;
				
				///COLORIZAR LOS CHATS
				if (purple_conversation_get_type(conv) == PURPLE_CONV_TYPE_CHAT)
				{
					GList *itertmp;
										
					for (itertmp=purple_conv_chat_get_users(PURPLE_CONV_CHAT(conv));itertmp!=NULL;itertmp=itertmp->next)
					{
						PurpleConvChatBuddy *buddy;
						buddy = (PurpleConvChatBuddy *) itertmp->data;
						
						//purple_debug_info("WebPidgin 2-COLORIZE","buddy->name=%s msg->who=%s nbuddy=%d\n", buddy->name, msg->who, nbuddy);
						
						if(purple_strequal(msg->who, buddy->name))
							break;
						else if(purple_strequal(buddy->name, self))
							continue;
						
						nbuddy++;					
					}					
					//purple_debug_info("WebPidgin 2-COLORIZE2","msg->who=%s nbuddy=%d\n", msg->who, nbuddy);
					if (nbuddy >= NUM_COLORS)
						nbuddy = (nbuddy%NUM_COLORS)+2;
				}						
				
				g_snprintf(stylenick,sizeof(stylenick),"color:%s", array_colors[nbuddy]);
				g_snprintf(stylemsg,sizeof(stylemsg),"divmsgother");
            }
				///MENSAJE DEL SISTEMA
            else if (purple_conversation_message_get_flags(msg) & PURPLE_MESSAGE_SYSTEM)
            {
            	g_snprintf(stylenick,sizeof(stylenick),"color:%s", array_colors[0]);
            	g_snprintf(stylemsg,sizeof(stylemsg),"divmsgsystem");
            }else
            {
            	g_snprintf(stylenick,sizeof(stylenick),"color:%s", array_colors[0]);
            	g_snprintf(stylemsg,sizeof(stylemsg),"divmsgsystem");
            }
        }

        when = msg->when;
        tm = localtime( &when );

		if (gGroupMessages)
		{
	        g_snprintf(date,sizeof(date),"<font color=#444444 size=-1>(%d:%02d)</font> ",tm->tm_hour,tm->tm_min);
    	}else
    	{
    		g_snprintf(date,sizeof(date),"<font color=#444444 size=-1>(%d:%02d:%02d)</font> ",tm->tm_hour,tm->tm_min,tm->tm_sec);
    	}

    	//purple_debug_info("WebPidgin 2/ASDD","%s %s\n", msg->who, msg->alias);

		if (msg->alias)
			g_snprintf(nick,sizeof(nick), "%s", msg->alias);
		else
			g_snprintf(nick,sizeof(nick), "%s", msg->who);
		

		///muestro la fecha y el nick
		if (!gGroupMessages || g_ascii_strcasecmp (lastDate, date) || g_ascii_strcasecmp (nick, lastNick) || g_ascii_strcasecmp (stylenick, lastStyleNick))
	    {
	    	if (msgCount>0)
				client_write(httpd,"\n</div>\n");									
	    	
	       	g_snprintf(lastDate,sizeof(lastDate), "%s", date);
	       	g_snprintf(lastNick,sizeof(lastNick), "%s", nick);
	       	g_snprintf(lastStyleNick,sizeof(lastStyleNick), "%s", stylenick);
			
	       	if (!gGroupMessages)
	       	{
	       		client_write(httpd,"\n<div>\n");
	    		client_write(httpd,date);			
	       	}

	       	if( gOptionBoldNames )
            	client_write(httpd,"<B>");
			
        	if (!gGroupMessages || !gOptionUseColor)
				g_snprintf(buffer,sizeof(buffer),"<span style='%s'>%s</span>", stylenick, nick);
			else
				g_snprintf(buffer,sizeof(buffer),"<span style='background-%s' class='msggroup'>%s</span>", stylenick, nick);
        	client_write(httpd,buffer);

        	if( gOptionBoldNames )
            	client_write(httpd,"</B>");


            if (gGroupMessages)
            {
            	client_write(httpd, "&nbsp;");
				client_write(httpd, date);
				client_write(httpd, "<BR />\n");
				
				g_snprintf(buffer,sizeof(buffer),"\n<div class='%s divmsgg'>\n", stylemsg);
				client_write(httpd,buffer);
			}
	    	else
            	client_write(httpd, ":&nbsp;"); /// Add a space, otherwise things blend in to much            				
	    }


		/// THE MESSAGE
        {

            char * tmpstr2=NULL;
			char * tmpstr=NULL; 
					
						
			///chapu
			tmpstr2 = purple_strreplace (msg->what, "\n", "{{{:::BR:::}}}");											
			//tmpstr2 = purple_strreplace (msg->what, "\n", "&nbsp;\n&nbsp;");
						
			if (purple_strequal (stylemsg,"divmsgsystem"))
			{
				
				GData *attributes;
				char *start;
				char *end;
				if (purple_markup_find_tag("a", tmpstr2, &start, &end, &attributes))
				{
					const char *tmp = g_datalist_get_data(&attributes, "href");
					
					
					purple_debug_info("WebPidgin 2/MSGSYSTEM","aki2 %s %s\n", start, tmp);
					///FOUND!
					//if (tmp && !g_ascii_strncasecmp(tmp, "file://", strlen ("file://")))
					if (tmp)
					{												
						//*end='\0';

						//tmpstr=g_strconcat(tmpstr2, 
						
						//tmpstr=g_strconcat(start, NULL);
						tmpstr=g_strconcat(tmpstr2, NULL);
					}
					
					g_datalist_clear(&attributes);
				}
			}
			
			if (!tmpstr)
			{
				tmpstr = purple_markup_strip_html (tmpstr2);				
				g_free(tmpstr2);					
													
						
				tmpstr2 = g_markup_escape_text (tmpstr, -1);
				g_free(tmpstr);
							
				tmpstr = purple_strreplace (tmpstr2, "\n", "<BR />\n");
				g_free(tmpstr2);
			}
			
			tmpstr2 = purple_strreplace (tmpstr, "{{{:::BR:::}}}", "<BR />\n");			
			g_free(tmpstr);			
			
				///SMILEYS BETA SUPER CHAPU
				//tmpstr2 = tmpstr;
				if (gOptionShowSmileys)
				{								
					char * ctemp;
					
					purple_debug_info("WebPidgin 2/MASDD","BETA\n");
					///CUSTOM SMILEYS PROPIOS
					if (purple_conversation_message_get_flags(msg) == PURPLE_MESSAGE_SEND)
					{
						GList * customsmileys= purple_smileys_get_all ();
						
						while (customsmileys)
						{					
							const char * sc= purple_smiley_get_shortcut((PurpleSmiley*)customsmileys->data);
							char imgsmiley[1024];
							char *enc64= purple_base16_encode (sc, strlen(sc));
							g_snprintf(imgsmiley, sizeof(imgsmiley), "<img src='image?smiley=%s' />", enc64);
							g_free (enc64);
							
							ctemp = tmpstr2;	
							tmpstr2 = purple_strreplace (ctemp, sc, imgsmiley);
							g_free(ctemp);
					
							customsmileys=g_list_delete_link(customsmileys, customsmileys); 
						}
					}else
					{					
						///CUSTOM SMILEYS RECIBIDOS
						GHashTable *csmileysr;								
						csmileysr= purple_conversation_get_data(conv, "webpidgin_custom_smileys_received");
						
						
						if (csmileysr)
						{						
							
							GList* gl = NULL;	
							/// DEBO HACER ESTO POR CULPA DE WIN32 Y SUS ANTICUADAS LIBRERIAS
							//gl = g_hash_table_get_keys(active_httpds);
							g_hash_table_foreach(csmileysr, get_keys, &gl);
											
							while (gl)
							{							
								char *smtmp = gl->data;
								char imgsmiley[1024];
								ctemp = tmpstr2;	
								char *enc64= purple_base16_encode (smtmp, strlen(smtmp));
								g_snprintf(imgsmiley, sizeof(imgsmiley), "<img src='image?cid=id=%p&smiley=%s' />", conv, enc64);	
								g_free (enc64);				
								tmpstr2 = purple_strreplace (ctemp, smtmp , imgsmiley);
								g_free(ctemp);
								gl=gl->next;
							}
						}
							
						
						purple_debug_info("WebPidgin 2/MASDD","BETA2\n");
						PidginConversation * pconv =PIDGIN_CONVERSATION(conv);
						GtkIMHtml* imhtml = GTK_IMHTML(pconv->imhtml);
						//GtkIMHtml* imhtml = GTK_IMHTML(pconv->entry);					
						
						GtkSmileyTree *t1;					
											
						t1 = g_hash_table_lookup(imhtml->smiley_data, purple_account_get_protocol_name(conv->account));
						purple_debug_info("WebPidgin 2/MASDD","imhtml %p datasmil %p tree %p\n", imhtml, imhtml->smiley_data, t1);
						
						if (t1)
						{
							GSList *list = g_slist_prepend (NULL, t1);
							int cint=0;
							while (list)
							{
								GtkSmileyTree *t = list->data;
								gsize i;
								list = g_slist_remove(list, t);
								GtkIMHtmlSmiley *gtksmil=t->image;
								
								purple_debug_info("WebPidgin 2/MASDD","BETA3 [%p]\n", gtksmil);
								//purple_debug_info("WebPidgin 2/MASDD","%s %s\n", dsad->smile, msg->alias);
								if (gtksmil)
								{														
									purple_debug_info("WebPidgin 2-MASDD2","[%d] %p smiley[%s][%p][%s][%u]\n",++cint,  gtksmil, gtksmil->file, gtksmil->data, gtksmil->smile, gtksmil->datasize);
									
									char imgsmiley[1024];
									ctemp = tmpstr2;	
									char *enc64= purple_base16_encode (gtksmil->smile, strlen(gtksmil->smile));
									g_snprintf(imgsmiley, sizeof(imgsmiley), "<img src='image?cid=id=%p&smiley=%s' />", conv, enc64);					
									g_free(enc64);
									tmpstr2 = purple_strreplace (ctemp, gtksmil->smile, imgsmiley);
									g_free(ctemp);
								}
								
								if (t && t->values) 
								{
									for (i = 0; i < t->values->len; i++)
										list = g_slist_prepend (list, t->children [i]);								
								}						
							}	
						}
					}
					
					PurpleAccount *account =conv->account;
					GSList * smileylist= pidgin_themes_get_proto_smileys(account->protocol_id);
					GtkIMHtmlSmiley *gtksmiley=NULL;
					
					
					while (smileylist)
					{	
						ctemp = tmpstr2;	
						gtksmiley= smileylist->data;
						
						//purple_debug_info("WebPidgin 2/MASDD4","%s [%s]\n", gtksmiley->smile, ctemp);
						
						char imgsmiley[1024];
						char *enc64= purple_base16_encode (gtksmiley->smile, strlen(gtksmiley->smile));
						g_snprintf(imgsmiley, sizeof(imgsmiley), "<img src='image?protoid=%s&smiley=%s' />", account->protocol_id, enc64);
						g_free(enc64);
						
						tmpstr2 = purple_strreplace (ctemp, gtksmiley->smile, imgsmiley);
						 
						g_free(ctemp);
						smileylist=smileylist->next;
					}
				}
				//tmpstr = tmpstr2;
				///BETA SUPER CHAPU			
			
			
			
			if (purple_conversation_message_get_flags(msg) & PURPLE_MESSAGE_NO_LINKIFY)
				tmpstr = g_strdup(tmpstr2);
			else
				tmpstr = purple_markup_linkify (tmpstr2);

			if (gGroupMessages)
				client_write(httpd, "&nbsp;");

            client_write(httpd,tmpstr);
            //client_write(httpd,msg->what);
            

            g_free(tmpstr);
            g_free(tmpstr2);
        }
			
		client_write(httpd,"<BR />\n");
		//client_write(httpd,"\n</div>\n");
			
        msgCount++;

        if (gGroupMessages)
        	iter=iter->prev;
        else
        	iter=iter->next;
    }
        
	//if (gGroupMessages)
		client_write(httpd,"\n</div>\n");

    //client_write(httpd,"<HR id=\"isend\" />\n");    
    client_write(httpd,"<span id=\"isend\" />\n");    
}


static void show_ajax_engine ( webpidgin_client_t * httpd )
{		
	///SOUND
	//gUseSounds=0;	
	if (gUseSounds)
	{
		client_write(httpd,"\n<script type=\"text/javascript\" src=\"swf.js\"></script>\n");		
	}

	client_write(httpd,"<script type=\"text/javascript\">var reqU = newxmlhttpreq(); var reqU2 =newxmlhttpreq(); \
		var title1=document.title; var timeout=100; var urlbase= location.protocol + '//' + location.host + '/';");
	
	client_write_vargs(httpd, "\nfunction get(req, page, fready){ try { \
			req.open('GET', page, true); req.onreadystatechange=function(){if (checkstatus(req)) eval (fready)}; \
			req.setRequestHeader( 'If-Modified-Since', 'Sat, 1 Jan 2000 00:00:00 GMT' ); \
			req.send(null);} catch (err) { %s req.abort(); } return false; }\n", purple_debug_is_enabled ()?"alert ('URL: [ ' + page + ' ], { ' + err+' }');":"");	

	client_write(httpd,"\nfunction checkstatus(req){if(req.readyState==4)return true; else return false; }\n");
	
	client_write(httpd,"\nfunction newxmlhttpreq(){ var ret= null; \
		if(window.XMLHttpRequest) {try {ret=new XMLHttpRequest();} catch(err1) { ret=null; }}\
		else if (window.ActiveXObject){try {ret=new ActiveXObject(\"Msxml2.XMLHTTP\");} catch(err2){ret=null;} } return ret; }\n");

	client_write(httpd, "\nfunction update(div, extra) { \
		var urivars='&'; if(div.length == div.toString().length) div=div.split(',') ;\
		if (reqU.readyState>0 && reqU.readyState<4) return; for (var i=0; i < div.length; i++){\
		var divc=document.getElementById(div[i]); urivars = urivars + div[i] + '=' + encodeURIComponent(divc.wpuc)+ '&';} \
		get(reqU, urlbase+ 'ajax?action=count' + urivars + extra, 'update1(\"'+ div +'\", \"'+ extra +'\");')}\n");	

	client_write(httpd, "\nfunction update1(div, extra) {\
		var divc = null; if(reqU.status == 200){ \
		var tmp = reqU.responseText.split(':::', 3); divc=document.getElementById(tmp[0]); var alertar=false; }else if (timeout<15000) timeout+=timeout;\
		if (divc && tmp.length == 3 && tmp[2] != divc.wpucSA) { if (typeof(divc.wpucSA) != 'undefined'){ alertar=true;} divc.wpucSA = tmp[2];}\
		if (divc && tmp[1] != divc.wpuc) { divc.wpuc=tmp[1]; get(reqU2, urlbase+ 'ajax?action='+ tmp[0] +'&'+ extra , 'update2(\"'+ tmp[0] +'\", '+ alertar +', \"'+ div +'\", \"'+ extra +'\");'); }\
		else {update_again(div,extra);} }\n");	

	client_write(httpd,"function update2(divU, alertar, div, extra) { \
		var divC=document.getElementById(divU); if(reqU2.status==200) { ");
		
	client_write(httpd,"var divconv=document.getElementById('conversation'); \n");	
	
	if (gGroupMessages && gGroupMessagesAutoH)
		client_write(httpd,"if (divconv) comp_scroll_conversation();\n");
	
	client_write(httpd,"divC.innerHTML=reqU2.responseText; timeout=100;\n");	
	
	if (gGroupMessages && gGroupMessagesAutoH)
		client_write(httpd,"if (document.body.clientHeight>440 && screen.height>500 && divconv) reajust_conversation(); else \n");
	client_write(httpd,"if (divconv) reajust_conversation(true);\n");
	
	client_write(httpd,"if(alertar) showalert('*** '+title1+' ***');} {update_again(div,extra);} ;}\n");

	client_write_vargs(httpd,"\nfunction update_again(div, extra){ try {setTimeout ('update(\"'+ div +'\", \"'+ extra +'\")', timeout);} \
		catch(e) {%s update(div ,extra);} }\n", purple_debug_is_enabled ()?"alert('ERROR update_again setTimeout:'+ e);":"");

	client_write(httpd,"\nvar showinterval=null; var blurred=false; \n ");
	client_write_vargs(httpd,"\nfunction showalert(text){ \n %s \n if(!blurred || showinterval) return; showinterval = setInterval('alertar(\"'+text+'\")', 600); }\n",  (gUseSounds)?"if(blurred){play_click()}":"");
	client_write(httpd,"\nfunction stopalert(){if (showinterval) clearInterval (showinterval); showinterval=null; document.title=title1;}\n");
	client_write(httpd,"\nfunction alertar(text){ if (document.title == title1) document.title=text; else document.title=title1; }");	
	client_write(httpd,"\n\
	window.document.onfocus=window.onfocus=function(e){blurred=false;stopalert();};\
	window.document.onblur=window.onblur=function(e){blurred=true;}; \n ");

	if (gUseSounds)
	{		
		client_write(httpd,"\nfunction play_click(){ if (typeof(click_sound1) != 'undefined') swfsound.startSound( click_sound1 ); /*else alert(typeof(click_sound1))*/;}");
	}

	client_write(httpd,"</script>");
}


/**
 * @brief handle a click on a buddy, which opens the "chat" view
 * @brief then handle any new message updates, etc.
 */
static int action_conversation( webpidgin_client_t * httpd, const char * extra )
{
	char header[2048];
    char buffer[2048];
    char form[4096];
    char * name;
    char buddyname[1024];
    char imgbuddyicon[1024]="";
    PurpleConversation *conv;
    PurpleAccount * account = NULL;

    purple_debug_info("WebPidgin 2","conversation::Extra[%s]\n",extra);

    name = webpidgin_normalize( extra );

	conv = find_conversation(name);
    if (conv) ///LA CONVERSACION YA EXISTE
    {
    	char buffermail[1024]="";
    	
    	account = conv->account;
    	g_snprintf(buddyname, sizeof(buddyname), "%s", purple_conversation_get_name(conv));
    
		///si son iguales no muestro el buddyname
    	if (!purple_strequal(buddyname, purple_conversation_get_title(conv)))
			g_snprintf(buffermail, sizeof(buffermail), " (%s) ", buddyname);
    	
    	if (purple_conversation_get_type(conv) == PURPLE_CONV_TYPE_IM)
		{
			g_snprintf(buffer,sizeof(buffer),"Chatting With:&nbsp;<b>%s</b> %s from <i>%s</i>", purple_conversation_get_title(conv), buffermail, account->username);
		}else
		{			
			g_snprintf(buffer,sizeof(buffer),"<b>%s</b> %s from <i>%s</i>", purple_conversation_get_title(conv), buffermail, account->username);
		}    	
    }
    else 
    {    	
		PurpleBuddy *buddy = find_buddy( name );
		
		if (buddy)
		{
			char buffermail[1024]="";
			
			g_snprintf(buddyname, sizeof(buddyname), "%s", buddy->name);
			
			///si son iguales no muestro el buddyname
			if (!purple_strequal(buddyname, get_buddy_name(buddy)))
				g_snprintf(buffermail, sizeof(buffermail), " (%s) ", buddyname);
						
			account = (buddy->account);
			g_snprintf(buffer,sizeof(buffer),"Chatting With:&nbsp;<b>%s</b> %s from <i>%s</i>", get_buddy_name(buddy), buffermail, account->username);
		}
    }
    
    if (!account)
	{
		//return action_root(httpd, extra);
		
		client_write_http_header_redirect (httpd, "ActiveList");
		return 1;
	}		    
    
    ///BUDDY ICON
	if( gOptionShowBuddyIcon && purple_buddy_icons_find (account, buddyname))
		g_snprintf(imgbuddyicon, sizeof(imgbuddyicon), "<img src='image?buddy=%s' class='buddyimg' />\n", buddyname);
		//g_snprintf(imgbuddyicon, sizeof(imgbuddyicon), "<img src='image?biconid=%p' class='buddyimg' />\n", purple_buddy_icons_find (account, buddyname));
		

    if (gGroupMessages)
    	g_snprintf(header,sizeof(header),"/conversation?%s%s#isend",time_stamp(),extra);
    else
    	g_snprintf(header,sizeof(header),"/conversation?%s%s",time_stamp(),extra);
    client_write_header( httpd,header);
	
	if (!gGroupMessages)
		client_write(httpd, imgbuddyicon);
		
	
	client_write(httpd, buffer);
    client_write(httpd, "<BR>\n");    


    /// Now our web form for the chat
    if (purple_account_is_connected(account))
    {    	
	    if (gUseJavascript && !gUseJSOnlyRef)
	    {
	    	g_snprintf(form,sizeof(form), "<form method=\"get\" action=\"/sendMessage?\" onsubmit=\"return send()\" >\n ");
	    	g_strlcat(form, "<div>\n <textarea class='taimsg' id=\"imsg\" name=\"msg\" tabindex=\"1\" cols=\"48\" rows=\"1\" onkeyup='keyp_imsg(this)' /></textarea>\n", sizeof(form));
	    }
	    else
	    {
	    	g_snprintf(form,sizeof(form), "<form method=\"get\" action=\"/sendMessage?\" >\n ");
	    	g_strlcat(form, "<div>\n <input style=\"-wap-input-format: *m\" id=\"imsg\" name=\"msg\" tabindex=\"1\" size=\"48\" maxlength=\"1280\" autocomplete=\"off\" />\n", sizeof(form));
	    }
		

	    g_snprintf(buffer,sizeof(buffer), "\n <input type=\"hidden\" name=\"to\" value=\"%s\" />\n", name);
	    g_strlcat(form, buffer, sizeof(form));

	    g_strlcat(form, "\n<input type=\"submit\" value=\"Send\" tabindex=\"2\" />\n", sizeof(form));
	    g_strlcat(form, "\n</div></form>\n", sizeof(form));
	}else
	{
		g_snprintf(form,sizeof(form), "<p>The account <b>%s (%s)</b> is not active</p>", account->username, purple_account_get_protocol_name( account ));
	}

	if (!gGroupMessages)
	{
    	client_write(httpd, form);
    	client_write(httpd,"<div id='divInfo'></div><div id='divError'></div>");
    }


	client_write(httpd,"<div id='conversation'>\n");
    if (conv)
    {
    	show_conversation (httpd, conv);
    }else
    {
        purple_debug_info("WebPidgin 2","conversation::Could not locate conversation for %s\n", name);
    }
    client_write(httpd,"</div>\n");


    if (gGroupMessages)
    {
    	client_write(httpd, imgbuddyicon);
    	client_write(httpd, form);
    	//client_write(httpd, "<hr />");
    	client_write_cmds( httpd, header );
    	client_write(httpd,"<div id='divInfo'></div> <div id='divError'></div>");
    }

    /// Show any active chats from other users. IE we're talking to bob, but in the meantime sally IM-ed us
    /// show sally has an unread message for us. ( Just like the Active: buddies section on the root page )
    client_write(httpd,"<div id='active_chats'>\n");
    if (!gOptionWWWFrames)
		show_active_chats( httpd, name );		
    
    client_write(httpd,"</div>\n");

    if (gUseJavascript)
    {
    	const char *encoded_name = purple_url_encode(name);    	

    	show_ajax_engine(httpd);    	

		client_write(httpd,"<script type='text/javascript'>var sending=false; var dcscrolled=false; var dcscheightold=0;\n \
			var reqsend = null; var cola=new Array(); \n\n");

		client_write(httpd,"\nfunction showmsg(msg, div, borrar){var divC=document.getElementById(div); \
			if (borrar) divC.innerHTML=''; divC.innerHTML+=msg }\n");
			
		///captura el onkeyup y recalcula el tamaño de imsg
		client_write(httpd,"\nfunction keyp_imsg(imsg){ var i=0;\
	while(imsg.value.length>i && (imsg.value.charCodeAt(i)==13 || imsg.value.charCodeAt(i)==10)){i++}\
	if (i >= imsg.value.length)imsg.value=''; \
	var nlines=Math.floor(imsg.value.length/48)+1;\n\
	var nlines2=imsg.value.split('\\n'); if(nlines2.length > nlines) nlines=nlines2.length;\n\
	if (nlines > 3)nlines=3; imsg.rows=nlines; }\n");

		client_write_vargs(httpd,"\nfunction true_send(){ if (sending || cola.length==0){ return;}; \n\
			sending=true; var msg=cola.shift(); reajust_conversation(true);\
			if (msg) get(reqsend, urlbase+ 'ajax?action=sendMessage&to=%s&msg=' + msg, 'if (req.status!=200) showmsg(\"<b>Error sending: '+ msg +'</b><br/>\", \"divError\"); true_send(); sending=false; /*update(\"conversation\", \"id=%s\");*/ ');  \
			else sending=false; }\n" , encoded_name, encoded_name);		


		///envia el mensaje mediante ajax
		client_write(httpd, "\nfunction send(){ \n var msg=document.getElementById('imsg'); \
			var nmsg= encodeURIComponent(msg.value).replace(/%20/g,'+'); msg.value=''; msg.focus();\n");		
		client_write(httpd, "if (nmsg == '') return false; cola.push(nmsg); return false; }\n");

		///pone el foco en el cuadro de escritura
		client_write(httpd, "\nfunction enfocar(){ try {document.getElementById('imsg').focus();}catch(err){}}\n");
		
		
		///REAJUSTA EL DIV CONVERSATION Y LO MUEVE AL NUEVO FINAL SI FUERZO O SI ANTES ESTABA AL FINAL
		if (gGroupMessages && gGroupMessagesAutoH)
		{
			client_write(httpd, "\nfunction reajust_conversation(forzar){ try {\n\
				var cambiado=false; var divconv=document.getElementById('conversation'); \
				if (document.body.clientHeight>440 && screen.height>500){ \n\
				if (dcscheightold != document.body.clientHeight || document.body.scrollHeight>document.body.clientHeight) \
				{ comp_scroll_conversation(); divconv.style.height=(document.body.clientHeight) + 'px'; \n\
				var overflow=document.body.scrollHeight-document.body.clientHeight+30; if (overflow<0)overflow=0;");
				
//client_write(httpd, "showmsg(divconv.style.height +' c- ' +typeof(click_sound1) + ' - ' + divconv.clientHeight + ' - ' + document.body.scrollHeight + ' - ' + document.body.clientHeight + ' - ' + dcscheightold + '<br>', 'divError'); ");

			client_write(httpd, ";cambiado=true; divconv.style.height=(divconv.clientHeight-overflow) + \"px\";");

//client_write(httpd, "showmsg(divconv.style.height +' d- ' +typeof(click_sound1) + ' - ' + divconv.clientHeight + ' - ' + document.body.scrollHeight + ' - ' + document.body.clientHeight + ' - ' + dcscheightold + '<br>', 'divError'); ");			

			client_write_vargs(httpd, ";dcscheightold = document.body.clientHeight; } }\
				if (cambiado || !dcscrolled || forzar) scroll_conversation();\
				} catch(e){%s;}}\n", purple_debug_is_enabled ()?"alert('ERROR reajust_conv:'+ e);":"");
		}else
		{
			client_write(httpd, "\nfunction reajust_conversation(forzar){if(forzar) scroll_conversation()}\n");
		}
	
		///COMPRUEBA EL SCROLL DE DIV CONV PARA VER SI HE MOVIDO LA BARRA Y EN ESE CASO NO SCROLLEAR AUTOMATICAMENTE
		client_write(httpd, "\nfunction comp_scroll_conversation(){ var divconv=document.getElementById('conversation');\
	dcscrolled=true; if (divconv.scrollTop+divconv.clientHeight >= divconv.scrollHeight && (document.body.clientHeight>440 && screen.height>500)) dcscrolled=false; }\n");
		
		///SCROLLEA AL FINAL
		if (gGroupMessages)
		{
			client_write(httpd, "\nfunction scroll_conversation(){ ");
			if (gGroupMessagesAutoH)
			{
				client_write(httpd, "if (document.body.clientHeight>440 && screen.height>500){\
					var divconv=document.getElementById('conversation');\
					divconv.scrollTop=divconv.scrollHeight; return;}\n");
			}
			client_write(httpd, "var msg=document.getElementById('imsg'); try{msg.scrollIntoView(true)}catch(e){}}");
		}
		else
			client_write(httpd, "\nfunction scroll_conversation(){}");
		
		///funcion a ejecutarse a intervalos regulares que realiza los envios
		client_write(httpd, "\nfunction interval_function(){try {\
			comp_scroll_conversation(); true_send(); reajust_conversation();\
			if (cola.length > 0) showmsg (\"Pending messages in queue: \" + cola.length, \"divInfo\", true);\
			else showmsg (\"\", \"divInfo\", true); setTimeout(\"interval_function()\",100);}\
			catch (err){alert('ERROR SI: '+ err);}} \n");
		
		
		///ONLOAD EVENT
		client_write(httpd,"window.onload=function(e){enfocar();\n");		


		if (!gOptionWWWFrames)
			client_write_vargs(httpd, "update(new Array ('conversation', 'active_chats'), \"id=%s\");", encoded_name);
		else
			client_write_vargs(httpd, "update(new Array ('conversation'), \"id=%s\");", encoded_name);		

		//client_write(httpd, "try {setInterval ('interval_function(); ', 100);} catch (err){alert('ERROR SI: '+ err);}");
		///hack for pocket IE
		if (!gUseJSOnlyRef)
			client_write(httpd, "interval_function(); \n");

//client_write(httpd, "var divconv=document.getElementById('conversation'); showmsg(divconv.style.height +' a- ' +screen.height + ' - ' + divconv.clientHeight + ' - ' + document.body.scrollHeight + ' - ' + document.body.clientHeight + ' - ' + dcscheightold + '<br>', 'divError'); ");
//client_write(httpd,"var divconv=document.getElementById('conversation'); alert(divconv.style.height +' a- ' +screen.height + ' - ' + divconv.clientHeight + ' - ' + document.body.scrollHeight + ' - ' + document.body.clientHeight + ' - ' + dcscheightold);\n");

		client_write(httpd,";dcscheightold=0;reajust_conversation(true);\n");
		
//client_write(httpd, "var divconv=document.getElementById('conversation'); showmsg(divconv.style.height +' b- ' +typeof(click_sound1) + ' - ' + divconv.clientHeight + ' - ' + document.body.scrollHeight + ' - ' + document.body.clientHeight + ' - ' + dcscheightold + '<br>', 'divError'); ");
//client_write(httpd,"var divconv=document.getElementById('conversation'); alert(divconv.style.height +' b- ' +screen.height + ' - ' + divconv.clientHeight + ' - ' + document.body.scrollHeight + ' - ' + document.body.clientHeight + ' - ' + dcscheightold);\n");				

		client_write(httpd,";}\n");					
					
		client_write(httpd,"window.onbeforeunload=function(e){try {if (cola.length > 0 || sending) return 'There are '+ (cola.length+1) + ' messages still in queue pending to be sent';}catch(err){}}\n");		
		
		///TENGO QUE VER SI AÑADIR UNA FUNCION Q ELIMINE LOS REQ AL UNLOAD
		///client_write(httpd,"window.onunload=function(e){try {alert(reqsend.readyState);alert(reqsend.abort());alert(reqsend.readyState);}catch(err){}}\n");		
		
		if (!gUseJSOnlyRef)
		{
			client_write(httpd,"\ntry {var imsg=document.getElementById('imsg'); imsg.onkeydown=function(e){ \
			var keynum; var ev=(window.event)?event:e; keynum = ev.keyCode; \
			if ((keynum==13 || keynum==10) && !ev.shiftKey) {send(); return false;}else {return true;} \
			}} catch (err){};\n");
									
			client_write(httpd,"\n var reqsend = newxmlhttpreq();\n");
			
			/*client_write(httpd,"\ntry {var imsg=document.getElementById('imsg'); imsg.onkeypress=function(e){ \
			var keynum; var ev=(window.event)?event:e; keynum = ev.keyCode; alert(keynum);\
			if ((keynum==13 || keynum==10) && !ev.shiftKey) {send(); return false;}else {return true;} \
			}} catch (err){};\n");*/
		}
		
		client_write(httpd,"</script>\n");

	}


    g_free( name );

    client_write_last_update( httpd );
    client_write_tail( httpd );
    return 1;
}

//parsea una cadena de variables de url y las devuelve en un hastable
static GHashTable* parse_get_extra(const char * extra)
{
	int i;
	GHashTable* hasht = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	char ** vars = g_strsplit(extra, "&", -1);

	//purple_debug_info("WebPidgin 2-DSA","%s\n",extra);
	for (i=0; i<g_strv_length(vars); i++)
	{
		char *igual=strstr(vars[i],"=");

		if (igual)
		{
			*igual='\0';
			igual++;
		}

		g_hash_table_insert(hasht, g_strdup(vars[i]), g_strdup(igual));
	}

	g_strfreev (vars);

	return hasht;
}


//callback function to process ajax calls
static gboolean ajax_only_count (gpointer data)
{
	webpidgin_client_t * httpd= (webpidgin_client_t *)data;
	GHashTable *hasht;
	char *hash;
	char buffer[1024];
	gboolean closec=TRUE;


	hasht = g_hash_table_lookup(active_httpds, httpd);
	if ( ! hasht  )
	{
		purple_debug_info("WebPidgin 2-SHAOZ1B","\nhttpd->watcher[ %d ] CLOSING\n\n",httpd->watcher);
		return FALSE;
	}

	//purple_debug_info("WebPidgin 2-SHAOZ1","httpd->fd[%d] httpd->ip_client[%s] [%s]\n",httpd->fd, httpd->ip_client, httpd->user_agent);
    //purple_debug_info("WebPidgin 2-SHAOZ1B","\nhttpd->watcher[ %d ] httpd[%d]\n\n",httpd->watcher, httpd);


	/// Conversation
	hash = g_hash_table_lookup(hasht, "conversation");
	if (closec && hash)
	{
		PurpleConversation *conv;
		const char *id;
		id = purple_url_decode(g_hash_table_lookup(hasht, "id"));
		conv = find_conversation(id);

		if (conv)
		{
		    unsigned unread = g_list_length (conv->message_history);
		    g_snprintf(buffer, sizeof(buffer), "%u", unread);

			if (strcmp (buffer, hash))
		    {
		    	purple_debug_info("WebPidgin 2-SHAOZ1","conversation>> httpd->fd[%d] md5[%s] hash[%s]\n",httpd->fd, buffer, hash);

		    	g_snprintf(buffer, sizeof(buffer), "conversation:::%u:::%u", unread, unread);
		    	client_write(httpd, buffer);

		    	closec=FALSE;
		    }
		}
	}


	hash = g_hash_table_lookup(hasht, "active_list");
	if (closec && hash)
	{
		char text[12288]="";
		const char *md5;

	    httpd->buffer=text;
	    httpd->buffer_len=sizeof(text);

	    show_active_list(httpd);

	    httpd->buffer=NULL;
	    httpd->buffer_len=-1;
	    md5=md5sum(text);

	    if (strcmp (md5, hash))
	    {
	    	purple_debug_info("WebPidgin 2-SHAOZ1","active_list>> httpd->fd[%d] md5[%s] hash[%s]\n",httpd->fd, md5, hash);

	    	g_snprintf(buffer, sizeof(buffer), "active_list:::%s:::-", md5);

			client_write(httpd, buffer);

			closec=FALSE;
	    }
	}

	hash = g_hash_table_lookup(hasht, "active_chats");
	if (closec && hash)
	{
		char md5[256];
    	const char *active_chats;
    	const char *md5_b;

    	active_chats=get_active_chats(md5);
    	md5_b=md5sum(active_chats);

	    if (strcmp (md5_b, hash))
	    {
	    	purple_debug_info("WebPidgin 2-SHAOZ1","active_chats>> httpd->fd[%d] md5[%s] hash[%s]\n",httpd->fd, md5, hash);

	    	g_snprintf(buffer, sizeof(buffer), "active_chats:::%s:::%s", md5_b, md5);
	    	client_write(httpd, buffer);

			closec=FALSE;
	    }
	}


	//Hago la limpieza
	if(!closec)
	{
		int w=purple_input_remove(httpd->watcher);

		if (w)
			client_end_connection(httpd);

		return FALSE;
	}

	return TRUE;

}

// Handles ajax requests
static int action_ajax( webpidgin_client_t * httpd, const char * extra )
{
    //const char * name;
    PurpleConversation *conv;

    char * action;
    GHashTable* hasht;

	client_write_http_header (httpd);

    purple_debug_info("WebPidgin 2","ajax::Extra[%s]\n",extra);

    //name = purple_url_decode( extra);
    hasht=parse_get_extra(extra);

    ///busco la accion a realizar
    action = g_hash_table_lookup(hasht, "action");
    if (action)
    {
    	purple_debug_info("WebPidgin 2/Shao","[%s]\n",action);

    	if (!strcmp(action, "count") )
    	{
    		g_hash_table_insert (active_httpds, httpd, hasht);
    		//purple_timeout_add_seconds(1, ajax_only_count, httpd);
    		purple_timeout_add(500, ajax_only_count, httpd);
    		return 10;
    	}else
    	///pido una conversacion
    	if (!strcmp(action, "conversation") )
    	{
    		const char *id;
		    id = purple_url_decode(g_hash_table_lookup(hasht, "id"));
		    conv = find_conversation(id);

		    if (conv)
		    {
		    	show_conversation(httpd, conv);		    	
		    }else
		    {
		        purple_debug_info("WebPidgin 2","conversation::Could not locate conversation for %s\n", id);
		    }
    	}
    	///las conversaciones activas
    	else if (!strcmp(action, "active_chats") )
    	{
    		char md5[256];
    		const char *active_chats;

    		active_chats=get_active_chats(md5);

    		client_write(httpd, active_chats);

    	}else if (!strcmp(action, "active_list") )
    	{
    		show_active_list(httpd);
    	}else if (!strcmp(action, "sendMessage") )
    	{
			char * msg= g_hash_table_lookup(hasht, "msg");
			char * to= g_hash_table_lookup(hasht, "to");

			//g_snprintf(extra, sizeof(extra), "%s=%s",  	purple_url_encode(to), msg);

			sendMessage( httpd, msg, to );
		}
    }

	g_hash_table_destroy(hasht);
    return 1;
}

static char * strtrim(const char* cad)
{
	const char *begin;
	const char *end;
	
	begin = cad;
	end=begin + strlen(begin) - 1;
	
	while( (g_ascii_isspace (*begin) || g_ascii_iscntrl(*begin)) && *begin != '\0' )
		begin++;
	
	if (strlen(begin)>1)
	{
		while(g_ascii_isspace (*end) || g_ascii_iscntrl(*end))
			end--;
	}
	
	return g_strndup(begin, end- begin +1);
}


/**
 * @brief Execute the http-post click
 */
static PurpleConversation * sendMessage( webpidgin_client_t * httpd, const char * message, const char *to )
{
    purple_debug_info("WebPidgin 2","sendMessage::msg[%s] to[%s]\n",message, to);


    /// FIRST we need to see if we can find a conversation window with this chat already in there
    gMissedRefreshes = 0;
    gLastRefreshInterval = DREFRESH_MIN;

    if( message )
    {
        const char * normal;
        const char * todec;
        char *msgtmp;
        PurpleConversation *c;
        PurpleConversationType type;
        PurpleCmdStatus status = PURPLE_CMD_STATUS_OK;

		todec = purple_url_decode (to);

        c = find_conversation(todec);
        if (!c)
        {
            PurpleBuddy *b = NULL;
            // Create the conversation ( assume extra is a buddy )
            b=find_buddy(todec);
            if (!b)
            {
                return 0;
            }

			///no deberia ser necesario mas, puesto que he incluido el codigo en find_conversation
            /*c = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, b->name, b->account);
            if (!c)*/            
            {
                c = purple_conversation_new(PURPLE_CONV_TYPE_IM, b->account, b->name);                                
            }

            if (!c)
            {
                return 0;
            }
        }

        msgtmp = g_strdup(message);
        normal = purple_url_decode (g_strdelimit (msgtmp, "+", ' '));
        g_free(msgtmp);
        msgtmp=strtrim(normal);

		//HACK MIO DE PRUEBA
		if (g_ascii_strcasecmp (msgtmp, "/wp quit") == 0)
			purple_conversation_destroy(c);
		else if (g_ascii_strcasecmp (msgtmp, "/wp showoffline") == 0)
			purple_prefs_set_bool ("/pidgin/blist/show_offline_buddies", !purple_prefs_get_bool ("/pidgin/blist/show_offline_buddies"));

        if (msgtmp[0]=='/')
        {
            char * error = NULL;
            char * cmdline;

            cmdline = g_strdup (&msgtmp[1]);
            status = purple_cmd_do_command(c, cmdline, cmdline, &error);
            if (error)
            {
                purple_debug_info("WebPidgin 2","sendMessage::do command[%d:%s]\n",status, error);
                g_free(error);
            }
            g_free(cmdline);
        } else
        {

            type = purple_conversation_get_type(c);
            switch (type) {
                case PURPLE_CONV_TYPE_IM:
                    purple_conv_im_send( PURPLE_CONV_IM(c), msgtmp);
                break;

                case PURPLE_CONV_TYPE_CHAT:
                    purple_conv_chat_send( PURPLE_CONV_CHAT(c), msgtmp);
                break;

                default:
                    purple_debug_info("WebPidgin 2","sendMessage::UnhandledType[%d]\n",type);
                break;
            }
        }

		g_free(msgtmp);
		return c;

    }
    else
    {
        return 0;
    }
}

static int action_sendMessage( webpidgin_client_t * httpd, const char * extra )
{
	GHashTable* hasht;
	PurpleConversation * c;
	//const char * name;
	char * msg;
	char * to;

	//name = purple_url_decode( extra);
	hasht=parse_get_extra(extra);

	msg= g_hash_table_lookup(hasht, "msg");
	to= g_hash_table_lookup(hasht, "to");

	c= sendMessage( httpd, msg, to);

	g_hash_table_destroy(hasht);

    if (c)
	{
		char buffer[4096];
		
		if (gGroupMessages)
			g_snprintf(buffer, sizeof(buffer), "/conversation?%sid=%p#isend",time_stamp(),c);
		else
			g_snprintf(buffer, sizeof(buffer), "/conversation?%sid=%p",time_stamp(),c);

		client_write_http_header_redirect (httpd, buffer);

		return 1;
	}else
		return action_active_list(httpd,NULL);
}


static int action_sessions( webpidgin_client_t * httpd, const char * extra )
{
	purple_debug_info("WebPidgin 2","%s\n",__FUNCTION__);
    client_write_header( httpd,"/Sessions" );
	show_last_sessions (httpd, INT_MAX);
	client_write_tail( httpd );
	return 1;
}

static const char* get_mime_type(const char* extension)
{
	if (!(strcmp (extension, "jpe") && strcmp (extension, "jpeg") && strcmp (extension, "jpg")))						
		return "image/jpeg";
	else if (!(strcmp (extension, "gif")))						
		return "image/gif";
	else if (!(strcmp (extension, "png")))										
		return "image/png";
	else if (!(strcmp (extension, "tiff") && strcmp (extension, "tif")))						
		return "image/tiff";
	else if (!(strcmp (extension, "bmp")))
		return "image/bmp";
	else if (!(strcmp (extension, "ico")))
		return "image/x-icon";
	else if (!(strcmp (extension, "js")))
		return "application/x-javascript";
	else if (!(strcmp (extension, "swf")))
		return "application/x-shockwave-flash";
	else if (!(strcmp (extension, "mp3")))
		return "audio/mpeg";
	else
		return "application/octet-stream";
}

/// RECIBE: buddy=buddyname Or smiley
static int action_image( webpidgin_client_t * httpd, const char * extra )
{
	PurpleBuddy *buddy ;
	GHashTable* vars = parse_get_extra(extra);
	gpointer p;
	const char * name;
	const char *ext=NULL;
	gconstpointer icon=NULL;
	size_t tam;
	
	
	purple_debug_info("WebPidgin 2","%s\n",__FUNCTION__);	
	purple_debug_info("WebPidgin 2-SHAOI","%s\n", extra);

    client_write( httpd,"");
    client_write( httpd,"HTTP/1.1 200 OK\n");
    client_write_date(httpd);
    client_write( httpd,"Server: WebPidginZ\n");
    client_write( httpd,"Connection: close\n");
        
    if ( (p = g_hash_table_lookup(vars, "buddy")))
    {		    
    	name = purple_url_decode (p);
		buddy = find_buddy(name);
		
		//purple_debug_info("WebPidgin 2-SHAOX2","%d\n",buddy);
		
		if (buddy)
		{			
			PurpleBuddyIcon* pbi;
					
			pbi = purple_buddy_icons_find(buddy->account, name);
			
			purple_debug_info("WebPidgin 2-SHAOX3","%d %s\n",pbi, 	purple_buddy_icon_get_full_path(pbi));
			
			///FOUND BUDDY ICON!!!
			if (pbi)
			{							
				icon= purple_buddy_icon_get_data(pbi, &tam);
				ext= purple_buddy_icon_get_extension (pbi);
					
				//purple_debug_info("WebPidgin 2-SHAOX4","%d %d\n",icon, tam);							
			}
		} 
    }
    else if ((p = g_hash_table_lookup(vars, "smiley")) && !purple_strequal(p,""))
    {	    	    	 
    	gsize sizeg;
    	guchar *cadg;
    	const char *gesto;
    	
    	cadg=purple_base16_decode(p, &sizeg);
    	if (cadg)
    	{
    		gesto=g_strndup(cadg, sizeg);
    		purple_debug_info("WebPidgin 2-SHAOSMILEY","smiley coded:[%s] dec:[%s]\n", p, gesto);
    	}
    	else
    	{    		
    		gesto=purple_url_decode (p);
    		purple_debug_info("WebPidgin 2-SHAOSMILEY","smiley NO coded:[%s] cod:[%s]\n", gesto, purple_base16_encode(gesto, strlen(gesto)));
    	}
    		
    	
    	
    	char *convid = g_hash_table_lookup(vars, "cid");
			
			///SMILEYS POR CONV
		if (convid)
		{
			PurpleConversation * conv = find_conversation(convid);	
					
			
			if (conv)
			{
				GHashTable *csmileysr;			
				PurpleStoredImage * newicon=NULL;
				csmileysr= purple_conversation_get_data(conv, "webpidgin_custom_smileys_received");
				
				if (csmileysr)
				{
					newicon=g_hash_table_lookup(csmileysr, gesto);
					if (newicon)
					{						
						icon= purple_imgstore_get_data(newicon);
						tam=purple_imgstore_get_size(newicon);	
						ext= purple_util_get_image_extension(icon, tam);
						purple_debug_info("WebPidgin 2-SHAOWCSR","%s i %p %u\n", gesto, icon, tam);
					}
				}
				
				PidginConversation * pconv =PIDGIN_CONVERSATION(conv);
				GtkIMHtml* imhtml = GTK_IMHTML(pconv->imhtml);
				GtkIMHtmlSmiley* gtksmil=gtk_imhtml_smiley_get(imhtml, purple_account_get_protocol_name(conv->account), gesto);
				
				
				if (gtksmil)
				{
					purple_debug_info("WebPidgin 2-SHAOSMILBA","smil[%s][%p][%s][%u]\n",gtksmil->file, gtksmil->data, gtksmil->smile, gtksmil->datasize);
					
					
					///SMILEY EN MEMORIA
					if (gtksmil->data)
					{						
						newicon=purple_imgstore_add(gtksmil->data, gtksmil->datasize, NULL);
					}
					///SMILEY EN DISCO
					else if ( gtksmil->file)
					{
						newicon=purple_imgstore_new_from_file(gtksmil->file);												    			
					}	
															
					if (newicon)
					{
						if (!csmileysr)							
							csmileysr=g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
						
						//purple_debug_info("WebPidgin 2-SHAOWCSR2","%p i %p\n", csmileysr, newicon);	
						g_hash_table_replace(csmileysr, g_strdup(gesto), newicon);
						//purple_debug_info("WebPidgin 2-SHAOWCSR2b","%p i %p\n", csmileysr, newicon);							
						purple_conversation_set_data(conv, "webpidgin_custom_smileys_received", csmileysr);
						
						icon= purple_imgstore_get_data(newicon);
						tam=purple_imgstore_get_size(newicon);							
						ext= purple_util_get_image_extension(icon, tam);
					}
					
				}
			}				
		}
    	
    	
			///CUSTOM SMILEYS 
		if (!icon ||!tam || !ext)
		{
			PurpleSmiley * smiley = purple_smileys_find_by_shortcut(gesto);
			
			if (smiley)
			{
				purple_debug_info("WebPidgin 2-SHAOSMIL","smiley [%s] -> %p\n", gesto, smiley);
				icon = purple_smiley_get_data(smiley, &tam);
				ext=purple_smiley_get_extension (smiley);
			}else
			{				
				///SMILEYS NO CUSTOM BY PROTOCOL
				GtkIMHtmlSmiley *gtksmiley= find_smiley(g_hash_table_lookup(vars, "protoid"), gesto);
				
				if (gtksmiley)
				{
					PurpleStoredImage * newicon=purple_imgstore_new_from_file(gtksmiley->file);
					
					purple_debug_info("WebPidgin 2->SHAOSMILAB2","%s %s \n",gtksmiley->smile, gtksmiley->file);  	    	
			
					if (newicon)
					{
						icon= purple_imgstore_get_data(newicon);
						tam=purple_imgstore_get_size(newicon);
						ext=purple_imgstore_get_extension(newicon);
					}
				}
			}
		}
		
	/// ITS A BINARY STORED FILE
    }else if ((p = g_hash_table_lookup(vars, "file")) && !purple_strequal(p,""))
	{
		int index;
		
		sscanf(p,"%d",&index);		
		purple_debug_info("WebPidgin 2-SHAOFILE","file %s [%d]\n", p, index);
		
		if (index > -1 && index < sizeof(binary_data_array))
		{
			icon = binary_data_array[index].binary_data;
			tam = binary_data_array[index].binary_data_size;
			ext=g_strrstr(binary_data_array[index].filename, ".");
			
			if (ext)
				ext++; /// quito el punto
			else
				ext="";
			
			//ext= purple_util_get_image_extension(icon, tam);
		}
	}
    
    
    if (icon && tam && ext)
	{		
		client_write_vargs(httpd, "Content-type: %s\n", get_mime_type(ext));		
		client_write_vargs(httpd, "Content-Length: %d\n\n", tam);	
							
		if (write(httpd->fd, icon, tam) < 1)
			purple_debug_info("WebPidgin 2-SHAO","Error writing buddy image\n");
	}				
    
    
	g_hash_table_destroy(vars);
	return 1;
}


///RECIBE ftid = PurpleXfer
static int action_getfileTransfer( webpidgin_client_t * httpd, const char * extra )
{	
	GHashTable* vars = parse_get_extra(extra);
	gpointer p;
	const char * name;	
	size_t tam;
	
	
	purple_debug_info("WebPidgin 2","%s\n",__FUNCTION__);	
	purple_debug_info("WebPidgin 2-SHAOI","%s\n", extra);

    client_write( httpd,"");
    client_write( httpd,"HTTP/1.1 200 OK\n");
    client_write_date(httpd);
    client_write( httpd,"Server: WebPidginZ\n");
    client_write( httpd,"Connection: close\n");
        
    if ( (p = g_hash_table_lookup(vars, "ftid")))
    {	
    	PurpleXfer *xfer=NULL;
    	const char *pdec=purple_url_decode (p);
    	long long unsigned int ftid=0;
    	
    	//purple_debug_info("WebPidgin 2-SHAOa","%p %llx %s\n", xfer, ftid, pdec);
    	
    	if (sscanf(pdec,"%llx",&ftid) == 1) 
		{
			GList* iter=purple_xfers_get_all();
		
			while (iter)
			{
				xfer = iter->data;
				
				purple_debug_info("WebPidgin 2-SHAO","XFER:%p FTID:%llx %s\n", xfer, ftid, xfer->local_filename);
				
				if (xfer == (void *)ftid)
					iter=NULL;
				else
					iter=iter->next;
			}
		}
    	    	
		///FOUND!!!
    	if (xfer)
    	{
    		char bufferfile[4096];
    		size_t readed=0;
    		FILE *f;
    		
    		f= fopen(xfer->local_filename, "rb");
    		
    		if (f)
    		{    							    			
    			char *header = g_strdup_printf("Content-Disposition: attachment; filename=\"%s\"\n", xfer->filename);
    			client_write( httpd, header);
    			g_free(header);
    			
    			header = g_strdup_printf("Content-Length: %d\n", xfer->size);
    			client_write( httpd, header);
    			g_free(header);
    				
				client_write( httpd,"Content-type: application/octet-stream\n\n");    				
    			
    			///Leo el fichero hasta el final
				do
				{
					readed=fread(bufferfile, sizeof(char), sizeof(bufferfile)/sizeof(char), f);
				
					if (write(httpd->fd, bufferfile, readed) < 1)
						purple_debug_info("WebPidgin 2-SHAO","Error writing file\n");										
				}while (readed == (sizeof(bufferfile)/sizeof(char)));								
				
				fclose(f);
    		}
    	}
    }
    
    /*
    if (icon && tam && ext)
	{									
		if (!(strcmp (ext, "jpe") && strcmp (ext, "jpeg") && strcmp (ext, "jpg")))						
			
		else if (!(strcmp (ext, "gif")))						
			client_write( httpd,"Content-type: image/gif\n\n");
		else if (!(strcmp (ext, "png")))										
			client_write( httpd,"Content-type: image/png\n\n");
		else if (!(strcmp (ext, "tiff") && strcmp (ext, "tif")))						
			client_write( httpd,"Content-type: image/tiff\n\n");
		else if (!(strcmp (ext, "bmp")))
			client_write( httpd,"Content-type: image/bmp\n\n");
		else
			client_write( httpd,"Content-type: image/x-icon\n\n");											
							
		if (write(httpd->fd, icon, tam) < 1)
			purple_debug_info("WebPidgin 2-SHAO","Error writing buddy image\n");
	}				
    
    */
	g_hash_table_destroy(vars);
	return 1;
}


static int action_customcss( webpidgin_client_t * httpd, const char * extra )
{	
	if (!gUseCustomCSS)
		return 0;
		
	purple_debug_info("WebPidgin 2","%s\n",__FUNCTION__);	
	purple_debug_info("WebPidgin 2-SHAOI","%s\n", extra);

    client_write( httpd,"");
    client_write( httpd,"HTTP/1.1 200 OK\n");
    client_write_date(httpd);
    client_write( httpd,"Server: WebPidginZ\n");
    client_write( httpd,"Connection: close\n");
    client_write( httpd,"Content-type: text/css;charset=utf-8\n\n");
        
    
    char bufferfile[4096];
    size_t readed=0;
    FILE *f;   
    char filecss[4096]; 
    g_snprintf(filecss, sizeof(filecss), "%s%s%s",  purple_user_dir(), G_DIR_SEPARATOR_S, "custom.css");
    
    f= fopen(filecss, "rb");
    		
    if (f)
	{   
		struct stat fileStat;
		fstat(f,&fileStat);
		//if(fstat(f,&fileStat) >= 0)
		{
			/*char *header = g_strdup_printf("Content-Length: %d\n\n", fileStat.st_size);
			client_write( httpd, header);
			g_free(header);*/
							
			do
			{
				readed=fread(bufferfile, sizeof(char), sizeof(bufferfile)/sizeof(char), f);
			
				if (write(httpd->fd, bufferfile, readed) < 1)
					purple_debug_info("WebPidgin 2-SHAO","Error writing file\n");										
			}while (readed == (sizeof(bufferfile)/sizeof(char)));								
			
			fclose(f);
		}
	}
    
	return 1;
}

static char * webpidgin_log_read_reverse(PurpleLog *log, PurpleLogReadFlags *flags)
{
    char * data = purple_log_read( log, flags );
    if( data )
    {
        unsigned max=strlen(data);
        char *p= NULL;
        char *reversed =  g_malloc( max+1  );

        if( ! reversed )
        {
            g_free(data);
            return NULL;
        }

        strcpy(reversed,"");
        while( ( p = strrchr(data,'\n') ) )
        {
            *p='\0';
            p++;
            if( strlen(p) > 0 )
            {
                strncat(reversed,p,max-strlen(reversed));
                strncat(reversed,"\n",max-strlen(reversed));
            }
        }
        strncat(reversed,data,max-strlen(reversed));

        g_free( data );
        return reversed;
    }
    return NULL;
}

static int action_history( webpidgin_client_t * httpd, const char * extra )
{
    char buffer[1024];
    PurpleBuddy *buddy = NULL;
    char * name;

    name = webpidgin_normalize( extra );
    buddy = find_buddy( name );


    g_snprintf(buffer,1024,"/history?%s%s",time_stamp(),extra);
    client_write_header( httpd,buffer);


    if( buddy && buddy->account )
    {
        GList *logs = purple_log_get_logs(PURPLE_LOG_IM, name, buddy->account );

        if( logs )
        {
            logs = g_list_concat( logs,  purple_log_get_logs(PURPLE_LOG_CHAT, name, buddy->account ) );
        }
        else
        {
            logs = purple_log_get_logs(PURPLE_LOG_CHAT, name, buddy->account );
        }

        if( logs )
        {
            GList *log;

            for (log = logs; log != NULL; log = log->next)
            {
                PurpleLog * gaimLog = log->data;
                char *data = webpidgin_log_read_reverse(gaimLog, NULL);

                if( data )
                {
                    const char *timestamp = purple_utf8_strftime("\n%H:%M", localtime(&gaimLog->time));
                    client_write(httpd, timestamp);
                    client_write(httpd,"<PRE>\n");
                    client_write(httpd,data);
                    client_write(httpd,"</PRE>\n");
                    g_free(data);
                }
            }
        }
        else
        {
            client_write(httpd,"<BR>No History Available</BR>\n");
        }
    }
    else
    {
        client_write(httpd,"<BR>No History Available</BR>\n");
    }
    client_write_tail( httpd );
    return 1;
}

/**
 * @brief This is the Initial Page when opening WebPidgin
 */
static int action_status( webpidgin_client_t * httpd, const char * unused )
{
    char buffer[1024];
    const char * my_addr = NULL;
    g_snprintf(buffer,1024,"/Status?%s",time_stamp());
    client_write_header( httpd,buffer);
        
	client_write(httpd,"<img src='logo.png' class='logo'/>\n");
    client_write(httpd,"WebPidgin-Z Version: ");
    client_write(httpd,WEBPIDGIN_VERSION);
    client_write_vargs(httpd,"<BR>Pidgin Version: %s", VERSION);
    client_write(httpd,"<BR><BR>\n");
    
    
//    my_addr = purple_network_get_local_system_ip(httpd->fd);
//    g_snprintf(buffer,1024,"System IP: %s<BR>\n", my_addr? my_addr : "N/A" );
//    client_write(httpd,buffer);
	
    my_addr = purple_network_get_my_ip(-1);
    g_snprintf(buffer,1024,"Public IP: %s<BR>\n", my_addr? my_addr : "N/A" );
    client_write(httpd,buffer);
    
	g_snprintf(buffer,1024,"Remote IP: %s<BR>\n", httpd->ip_client );
    client_write(httpd,buffer);
        
	g_snprintf(buffer,1024,"Remote IP X-Forwarded-For: %s<BR>\n", httpd->ip_client_xff ? httpd->ip_client_xff : "N/A");
    client_write(httpd,buffer);
    
	g_snprintf(buffer,1024,"RSS Feed: %s<BR>\n",gOptionRSSFeed ? "Enabled" : "Disabled");
    client_write(httpd,buffer);

    g_snprintf(buffer,1024,"Auto Refresh: %s",gOptionMetaRefresh ? "Enabled" : "Disabled");
    client_write(httpd,buffer);

    if( gOptionMetaRefresh )
    {
        g_snprintf(buffer,1024," (%d sec)",gOptionMetaRefreshSeconds);
        client_write(httpd,buffer);
    }

    if( gOptionMetaRefreshDynamic )
    {
        g_snprintf(buffer,1024," *Dynamic*");
        client_write(httpd,buffer);
    }
    
    client_write(httpd,"<BR>\n");
    client_write(httpd,"<HR>\n");

    show_last_sessions (httpd, gShowNLastSessions);
    

    //More stuff here like, e-mail notices etc
    //client_write(httpd,"<B>New E-Mail:</B>");
    //client_write(httpd,"Updates");    
    show_active_chats( httpd, NULL );
    
    client_write_tail( httpd );
    
    return 1;
}

/**
 * @brief strip out all elements and illegal character from the message
 * @brief so that it confirms to the RSS specification
 * @param msg the old message
 * @returns the striped message ( it must be freed )
 */
static char * webpidgin_rss_strip_html( char * msg )
{
    char * tmp = purple_markup_strip_html( msg );
    if( tmp )
    {
        /// We first used Pidgin's internal stip mechanism
        /// now we postsrtip everything else
        unsigned i;
        unsigned cur = 0;
        unsigned len = strlen(tmp);
        for( i = 0; i < len ; i++ )
        {
            switch( tmp[i] ){
                case '<' :
                case '>' :
                case '&' :
                case '%' :
                break;

                default:
                    if( cur != i )
                        tmp[cur] = tmp[i];
                    cur++;
                break;
            };
        }

        tmp[cur] = '\0';
    }
    return tmp;
}

static void rss_free(rss_item_t *item)
{
    if (!item) {
        return;
    }

    if (item->who) {
        g_free(item->who);
    }

    if (item->what) {
        g_free(item->what);
    }

    if (item->convName) {
        g_free(item->convName);
    }
    g_free(item);
}

static void rss_add(PurpleConversation *conv, char *sender, char *buffer )
{
    rss_item_t *item;

    if( gOptionRSSFeed == 0 ) {
        return;
    }

    item = (rss_item_t*) g_malloc(sizeof(*item));
    if (item) {
        /* Suping the strings here wastes abit of memory, but we can;t controll
           what pidgin does with the conv pointers after this point so we need
           our own copies  */
        item->who  = strdup(sender);
        item->what = strdup(buffer);
        if (conv) {
            item->convName = strdup(purple_conversation_get_name(conv));
        }
        if ((item->who==NULL) || (item->what==NULL)) {
            rss_free(item);
        } else {
            item->tm      = time(NULL);
            item->next = rssHead.next;
            rssHead.next  = item;
        }
    }
}



static int action_rss( webpidgin_client_t * httpd, const char * unused )
{
    rss_item_t *item;
	const char *my_addr = NULL;
	const char *rfc822_time = NULL;
    char buffer[1024];

    purple_debug_info("WebPidgin 2","%s\n",__FUNCTION__);

    if( gOptionRSSFeed == 0 )
    {
        client_write_header( httpd,"/status.rss");
        client_write(httpd,"<B>RSS Feed is Disabled</B>\n");
        client_write_tail(httpd);
        return 1;
    }

    client_write( httpd,"");
    client_write( httpd,"HTTP/1.1 200 OK\n");
    client_write_date(httpd);
    client_write( httpd,"Server: WebPidgin\n");
    client_write( httpd,"Connection: close\n");
    client_write( httpd,"Content-type: application/rss+xml\n\n");

    client_write(httpd,"<?xml version=\"1.0\"?>\n");
    client_write(httpd,"<rss version=\"2.0\">\n");
    client_write(httpd," <channel>\n");
    client_write(httpd,"  <title>WebPidgin Status</title>\n");

	my_addr = purple_network_get_my_ip(-1);
	if (my_addr != NULL)
	{
        g_snprintf(buffer,sizeof(buffer),"  <link>http://%s:%d/</link>\n",my_addr,gOptionPort);
        client_write(httpd,buffer);
    } else {
        client_write(httpd,"  <link>http://sourceforge.net/projects/webpidgin/</link>\n");
    }

    client_write(httpd,"  <description>WebPidgin Updates</description>\n");

    for( item = rssHead.next; item != NULL; item = item->next )
    {
        if ( item == rssHead.next )
        {
            // Write a <pubDate> for the <channel> = the date the last IM was received
            rfc822_time = purple_utf8_strftime("%a, %d %b %Y %H:%M:%S %z", localtime(&item->tm));
            if (rfc822_time != NULL)
            {
                g_snprintf(buffer,sizeof(buffer),"  <pubDate>%s</pubDate>\n", rfc822_time);
                client_write( httpd,buffer);
            }

            // Write a <lastBuildDate> = now, since we generate the feed on-the-fly
            rfc822_time = purple_utf8_strftime("%a, %d %b %Y %H:%M:%S %z", NULL);
            if (rfc822_time != NULL)
            {
                g_snprintf(buffer,sizeof(buffer),"  <lastBuildDate>%s</lastBuildDate>\n", rfc822_time);
                client_write( httpd,buffer);
            }
        }
        if (item->what != NULL)
        {
            client_write(httpd,"  <item>\n");
            g_snprintf(buffer,sizeof(buffer),"   <guid isPermaLink=\"false\">%d-%p</guid>\n",(int)item->tm,item);
            client_write(httpd,buffer);
            client_write(httpd,"   <title>");
            g_snprintf(buffer,sizeof(buffer),"New Message From %s",item->who);
            client_write(httpd,buffer);
            client_write(httpd,"</title>\n");
            if (my_addr != NULL)
            {
                /** initialize buffer qith generic link */
                g_snprintf(buffer,sizeof(buffer),"http://%s:%d/", my_addr, gOptionPort);

                if (item->convName) {
                    char * name = webpidgin_encode(item->convName);
                    if (name) {
                        /** We have the specific link use that instead */
                        g_snprintf(buffer,sizeof(buffer),"http://%s:%d/conversation?%s",my_addr,gOptionPort,name);
                        g_free(name);
                    }
                }
                client_write(httpd,"   <link>");
                client_write(httpd,buffer);
                client_write(httpd,"</link>\n");
            }
            client_write(httpd,"   <description>");

            {
                char * tmpstr = webpidgin_rss_strip_html(item->what);
                g_snprintf(buffer,sizeof(buffer),"%s",tmpstr);
                g_free( tmpstr );
            }

            client_write(httpd,buffer);
            client_write(httpd,"</description>\n");
            rfc822_time = purple_utf8_strftime("%a, %d %b %Y %H:%M:%S %z", localtime(&item->tm));
            if (rfc822_time != NULL)
            {
                g_snprintf(buffer,sizeof(buffer),"   <pubDate>%s</pubDate>\n", rfc822_time);
                client_write( httpd,buffer);
            }
            client_write(httpd,"  </item>\n");
        }
    }
    client_write(httpd," </channel>\n</rss>\n");
    client_write(httpd,"\n\n");
    return 1;
}

static int action_help( webpidgin_client_t * httpd, const char * unused )
{
    const char * my_addr = NULL;
    char buffer[1024];
    g_snprintf(buffer,1024,"/Help?%s",time_stamp());
    client_write_header( httpd,buffer);
    client_write(httpd,"<B>Help:</B><BR><BR>\n");
        
    client_write(httpd,"1. <B>RSS Feed</B>\n");
    client_write(httpd,"<div style=\"margin-left: 20px;\">\n");
    my_addr = purple_network_get_my_ip(-1);

    g_snprintf(buffer,1024,"When Enabled the RSS feed is available at http://%s:%d/status.rss<BR>\n", my_addr ? my_addr : "localhost", gOptionPort);
    client_write(httpd,buffer);
    client_write(httpd,"If your Pidgin client is behind a NAT/PAT firewall you may need to manually set the external <B>internet</b> address of the firewall in the Pidgin Tools:Preferences:Network:Public IP dialog and forward the webpidgin port to the internal address.<BR><BR>");
    client_write(httpd,"Example<BR>\n");
    client_write(httpd,"&nbsp;&nbsp;Firewall IP: 64.23.12.12<BR>\n");
    client_write(httpd,"&nbsp;&nbsp;PURPLE/WebPidgin PC IP: 10.0.0.1<BR>\n");
    client_write(httpd,"&nbsp;&nbsp;PURPLE/WebPidgin PC Port: 5888<BR>\n");
    client_write(httpd,"<BR>\n");
    client_write(httpd,"In this configuration you should set the PURPLE <B>Public IP</b> to 64.23.12.12 and your Firewall should port forward TCP port 5888 to IP 10.0.0.1<BR>\n");
    client_write(httpd,"</div><BR>\n");

    client_write(httpd,"2. <B>Credits</B>\n");
    client_write(httpd,"<div style=\"margin-left: 20px;\"><p>\n");
    client_write(httpd,"<img src='logo.png' class='logo' />\n");
    client_write(httpd,"<b>WebPidgin-Z:</b><BR>\n");
    client_write(httpd,"&nbsp;&nbsp;by Shaorang<BR>\n");
    client_write(httpd,"<BR>\n");
    client_write(httpd,"WebPidgin Version: ");
    client_write(httpd,WEBPIDGIN_VERSION);
    client_write(httpd,"<BR>\n");
    client_write(httpd,"<A HREF=http://www.shaorang.com/webpidginz/>WebPidgin-Z Home Page</A><BR>\n");
    client_write(httpd,"</p><BR><p>\n");
    
    client_write(httpd,"<b>WebPidgin Original:</b><BR>\n");
    client_write(httpd,"Copyright (C) 2006,2007 Sebastian Sobolewski<BR>\n");
    client_write(httpd,"Additional Contributors:<BR>\n");
    client_write(httpd,"&nbsp;&nbsp;David Morse<BR>\n");        
    client_write(httpd,"<A HREF=http://www.thirdmartini.com/index.php/WebPidgin_2.x>Original WebPidgin Home Page</A><BR>\n");
    client_write(httpd,"</p></div><BR>\n");

    client_write(httpd,"3. <B>License</B>\n");
    client_write(httpd,"<div style=\"margin-left: 20px;\">\n");
    client_write(httpd,license);
    client_write(httpd,"</div><BR>\n");

    client_write_tail( httpd );
    return 1;
}


/**
 * @brief Action dispatcher  structure, matches the http-post request to
 * @brief the appropriate action_xxx call for handling
 */
static webpidgin_parse_t webpidgin_actions[] = {
	{ "/ajax",action_ajax },
    { "/", action_root },
    { "/ActiveList", action_active_list },
    { "/login",action_login },
    { "/logout",action_logout },
    { "/conversation",action_conversation },
    { "/sendMessage",action_sendMessage },
    { "/Accounts",action_accounts },
    { "/Options",action_options },    
    { "/history",action_history },
    { "/Status",action_status },
    { "/status.rss",action_rss },
    { "/Help", action_help },
    { "/Sessions", action_sessions },
    { "/image", action_image },    
    { "/getFile", action_getfileTransfer },
    { "/custom.css", action_customcss }
};
///****************************************************************************************************///
///  WEBSERVER CODE:  The code below deals with the webserver micro engine
///****************************************************************************************************///


/**
 * @brief send a AUTH-REJECT message to the client webbrowser if the user/pass
 * @brief was incorrect
 */
static int client_auth_reject( webpidgin_client_t * client )
{
    purple_debug_info("WebPidgin 2/SHAO","Auth Rejected\n");

    client_write(client,"HTTP/1.1 401 Authorization Required\n");
    /// FIXME: stamp the time and server name correctly
    client_write_date(client);
    client_write(client,"Server: WebPidginZ \n");
    client_write(client,WEBPIDGIN_VERSION);
    client_write(client,"\nWWW-Authenticate: Basic realm=\"WebPidginZLoginBasic\"\n");
    client_write(client,"Connection: close\n");
    client_write(client,"Content-type: text/html\n\n\n\n");
    return 1;
}

static int client_auth_reject_digest( webpidgin_client_t * client, gboolean stale )
{
    char *nonce;
    char buffer[512];

    purple_debug_info("WebPidgin 2/SHAO","Auth Rejected\n");
    g_snprintf(buffer, sizeof(buffer), "%u:%s:%u:%d", (unsigned)clock(), client->ip_client, GPOINTER_TO_UINT(client), rand());
    nonce=g_strdup(md5sum(buffer));
    purple_debug_info("WebPidgin 2/SHAO","New nonce [%s]\n", nonce);
    g_hash_table_insert (nonce_list, nonce, GUINT_TO_POINTER(0));

    client_write(client,"HTTP/1.1 401 Authorization Required\n");
    client_write_date(client);
    client_write(client,"Server: WebPidginZ \n");
    client_write(client,WEBPIDGIN_VERSION);
    client_write(client,"\nWWW-Authenticate: Digest realm=\"WebPidginZLoginDigest\", ");

    g_snprintf(buffer, sizeof(buffer), "nonce=\"%s\", opaque=\"0000000000000000\", stale=%s, algorithm=MD5, qop=\"auth\"\n", nonce, stale?"true":"false");
    client_write(client, buffer);

    client_write(client,"Connection: close\n");
    client_write(client,"Content-type: text/html\n\n\n\n");
    return 1;
}

static int write_log_connection(int fd, char * buffer)
{
	if(gSaveConnectionLog)
	{
		char logfile[1024];
		g_snprintf(logfile, sizeof(logfile), "%s%s%s",  purple_user_dir(), G_DIR_SEPARATOR_S, gConnectionLogFileName);
		webpidgin_connection_log = fopen(logfile, "a");

		if (webpidgin_connection_log)
		{
			purple_debug_info("WebPidgin 2/Shao","Opening %s for logging\n", logfile);

			if ( fprintf( webpidgin_connection_log, "(%s)(%d) %s\n", purple_date_format_long(NULL), fd, buffer) < 0)
			{
				purple_debug_info("WebPidgin 2/SHAO","Error saving logging data to %s\n", gConnectionLogFileName);
				fclose (webpidgin_connection_log);
				return 1;
			}
			else
			{
				purple_debug_info("WebPidgin 2/SHAO","Saving logging data to %s\n", gConnectionLogFileName);
				fclose (webpidgin_connection_log);
				return 0;
			}

		}else
			purple_debug_info("WebPidgin 2/Shao","Error opening %s for logging\n", logfile);
	}

	return 1;
}


static char* parse_auth_digest(const char * auth, const char * value)
{
	char * begin;
	char * end;
	char buffer[1024];
	char *ret=NULL;

	g_snprintf(buffer, sizeof(buffer), "%s=", value);

	//busco el valor
    begin = strstr(auth, buffer);
    if (begin)
    {
    	begin+= strlen(buffer);

    	if (*begin == '\"')
    	{
    		begin++;
    		end = strstr(begin,"\"");
    	}
    	else
    	{
    		char * coma=strstr(begin,",");
    		char * spc=strstr(begin," ");
    		end= strstr(begin,"\n");

			if (end)
			{
				if (coma && coma<end)
					end=coma;

			}else if (coma)
				end = coma;

			if (spc)
			{
				if ((end && spc<end) || !end)
					end = spc;
			}
    	}

    	if (end)
    	{
    		ret = g_strndup(begin, end-begin);

    		purple_str_strip_char(ret, '\r');
    		purple_str_strip_char(ret, '\t');
    		purple_str_strip_char(ret, '\n');
    	}
    }

    if (!ret)
		ret = g_strdup("");

    return ret;
}


/**
 * @brief parse the URL request from the client webbrowser and dispatch it to
 * @brief the appropriate action_xxx call
 */
static int client_parse_and_dispatch(webpidgin_client_t *httpd, char * buffer, const int len )
{
    char * purl = NULL;
	char * pxff = NULL;
	char buffer2[1024];

    purple_debug_info("WebPidgin 2","HTTP: [%s]\n", buffer);
    g_snprintf(buffer2, sizeof(buffer2), "[\n %s \n]", buffer);
    write_log_connection (httpd->fd, buffer2);

	//CHECK FOR X-Forwarded-For HEADER
	purple_debug_info("WebPidgin 2/SHAO","Checking for X-Forwarded-For\n");
	httpd->ip_client_xff=NULL;
	pxff = strstr(buffer,"X-Forwarded-For: ");
	if( pxff )
    {
		char *end = NULL;
		pxff += 17;

		end = strstr( pxff, "\n" );
        if( end )
		{
			char * cxff = NULL;
			cxff = g_malloc0 (end-pxff+1 * sizeof (char));
			memcpy (cxff, pxff, end-pxff);

			purple_debug_info("WebPidgin 2/SHAO","%s\n",cxff);

			httpd->ip_client_xff = cxff;
		}
    }

    //CHECK FOR User-Agent HEADER
    purple_debug_info("WebPidgin 2/SHAO","Checking for User-Agent\n");
    httpd->user_agent = NULL;
	pxff = strstr(buffer,"User-Agent: ");
	if( pxff )
    {
		char *end = NULL;
		pxff += 12;

		end = strstr( pxff, "\n" );
        if( end )
		{
			char * cxff = NULL;
			cxff = g_malloc0 (end-pxff+1 * sizeof (char));
			memcpy (cxff, pxff, end-pxff);

			purple_debug_info("WebPidgin 2/SHAO","%s\n",cxff);

			httpd->user_agent = cxff;
		}
    }

	add_session (httpd);
	

    /// Id auth is enabled check the response
    if( httpd->webpidgin->auth )
    {
        char *temp = strdup(buffer);

        if (!gOptionUseDigestAuth)
        {
        	///BASIC AUTH
        	char * end;
	        char *auth = strstr(temp,"Authorization: Basic ");

	        purple_debug_info("WebPidgin 2/SHAO","Auth time: Basic\n");

	        if( !auth )
	        {
				write_log_connection (httpd->fd, "'Authorization: Basic' not found");

	            free( temp );
	            return client_auth_reject(httpd);
	        }

	        auth+=strlen("Authorization: Basic ");
	        end = strstr(auth,"\n");
	        if( end )
	            *end = '\0';

	        end = strstr(auth,"\r");
	        if( end )
	            *end = '\0';


	        end = strstr(auth," ");
	        if( end )
				*end = '\0';

	        if( strcmp(httpd->webpidgin->auth,auth) != 0 )
	        {
	        	char * userpass;

	        	userpass = (char *) purple_base64_decode (auth, NULL);
	        	g_snprintf(buffer2, sizeof(buffer2), "Wrong user/pass: (%s)", userpass);
	        	g_free(userpass);
	        	write_log_connection (httpd->fd, buffer2);

	            free( temp );
	            /// wrong auth; deny
	            return client_auth_reject(httpd);
	        }
		}
		else ///DIGEST AUTH
		{
			char * username=NULL;
			char * nonce=NULL;
			char * uri=NULL;
			char * response=NULL;
			char * opaque=NULL;
			char * nc=NULL;
			char * cnonce=NULL;
			char * key;
			char * response_mine;
			gboolean reject_auth = FALSE;
			gboolean reject_auth_stale = FALSE;
            char *auth;


			purple_debug_info("WebPidgin 2/SHAO","Auth time: Digest\n");

            auth = strstr(temp,"Authorization: Digest ");
	        if( !auth )
	        {
				write_log_connection (httpd->fd, "'Authorization: Digest' not found");

	            free( temp );
	            return client_auth_reject_digest(httpd, reject_auth_stale);
	        }

	        username = parse_auth_digest(temp, "username");
	        nonce = parse_auth_digest(temp, "nonce");
	        uri = parse_auth_digest(temp, "uri");
	        response = parse_auth_digest(temp, "response");
	        opaque = parse_auth_digest(temp, "opaque");
	        nc = parse_auth_digest(temp, "nc");
	        cnonce = parse_auth_digest(temp, "cnonce");

	        key=purple_cipher_http_digest_calculate_session_key("MD5", gOptionUsername, "WebPidginZLoginDigest", gOptionPassword, nonce, cnonce);
	        response_mine =purple_cipher_http_digest_calculate_response("MD5", "GET", uri, "auth", NULL, nonce, nc, cnonce, key);

	        purple_debug_info("WebPidgin 2/SHAOC","user: [ %s ] \n", username);
	        purple_debug_info("WebPidgin 2/SHAOC","nonce: %s uri:[ %s ] opaque:[ %s ] nc:[ %s ] cnonce:[ %s ]\n", nonce, uri, opaque, nc, cnonce);
	        purple_debug_info("WebPidgin 2/SHAOC","key: ( %s ) response mine:[ %s ] response from client:[ %s ]\n", key, response_mine, response);


            ///AUTH WRONG
            if( ! response || strcmp(response_mine,response) )
            {
                char buffertmp[1024];
                g_snprintf(buffertmp, sizeof(buffertmp), "Wrong response from client: client response (%s) != server response (%s)", response, response_mine);
	        	write_log_connection (httpd->fd, buffertmp);

	        	purple_debug_info("WebPidgin 2/SHAO","%s\n", buffertmp);

                reject_auth=TRUE;
            }else if (response && !strcmp(response_mine,response))
            {/// RESPONSE OK, CHECKING nonce AND NC
                gpointer p;
                unsigned nc_actual= strtoul(nc, NULL, 16);

                if(g_hash_table_lookup_extended (nonce_list, nonce, NULL, &p) && GPOINTER_TO_UINT(p)< nc_actual && nc_actual<999)
                {
                    g_hash_table_insert(nonce_list, g_strdup(nonce), GUINT_TO_POINTER(nc_actual));
                    purple_debug_info("WebPidgin 2/SHAO","nc: %u %u\n", nc_actual, GPOINTER_TO_UINT(p));
                }else
                {
                    purple_debug_info("WebPidgin 2/SHAO","Invalid nonce or nc, updating nonce\n");
                    reject_auth=TRUE;
                    reject_auth_stale=TRUE;
                }
            }

            g_free(username);
            g_free(nonce);
            g_free(uri);
            g_free(response);
            g_free(opaque);
            g_free(nc);
            g_free(cnonce);
            g_free(key);
            g_free(response_mine);

            if (reject_auth)
                return client_auth_reject_digest(httpd, reject_auth_stale);
		}

        free( temp );
    }

	httpd->auth_ok = 1;
	add_session (httpd);

    /// Find the GET rquest line (we don't support anything else)
    purl = strstr(buffer,"GET ");
    if( purl )
    {
         int action = 0;
        char *end = NULL;
        char *extra = NULL;
        const char * data;
        purl+=4;
        // found our URL location
        end = strstr( purl, " " );
        if( end )
        {
            char *temp;
            *end = '\0';
            extra = strstr(purl,"?");
            if( extra )
            {
                *extra = '\0';
                extra++;
            }

            /// See if there is a timestamp
            if( extra )
            {
                temp = strstr(extra,"time=");
                if( temp )
                {
                    /// since we found the timestamp, trim it out
                    extra = strstr(temp,"&");
                    if( extra )
                    {
                        *extra = '\0';
                        extra++;
                    }
                }
            }

            if( extra )
            {
                data = extra;
            }
            else
            {
                data = empty_string;
            }

            /// Now we can dispatch
            for(action=0; action< ( sizeof(webpidgin_actions)/sizeof(webpidgin_parse_t) ); action++ )
            {
            	purple_debug_info("WebPidgin 2","Action Compare [%s] <--> [%s]\n",purl,webpidgin_actions[action].action);
                if( strcmp(webpidgin_actions[action].action,purl) == 0 )
                {
                    return webpidgin_actions[action].callback( httpd, data );
                }
            }
            
            /// SEARCHING FOR INCLUDED BINARY FILES
            for(action=0; action< ( sizeof(binary_data_array)/sizeof(webpidgin_parse_bin) ); action++ )
            {
            	purple_debug_info("WebPidgin 2-SHAO","File Compare [%s] <--> [%s] %lu %lu\n",purl,binary_data_array[action].filename, sizeof(binary_data_array[action].binary_data), binary_data_array[action].binary_data_size);
            	
            	if( strcmp(binary_data_array[action].filename, purl) == 0 )
                {
                	char vars[1024];                    
                    
                    g_snprintf(vars, sizeof(vars),"file=%d", action);
                    return action_image(httpd, vars);
                }
            }
            /// nothing on the first pass check to see if we have extra info

            purple_debug_info("WebPidgin 2","No action for [%s]\n",purl);
        }
    }

    return 0;
}


static void client_end_connection (webpidgin_client_t *httpd)
{
	char buffer[1024];

	g_snprintf(buffer, sizeof(buffer), "Closing connection from: %s\n------------------------------------------------------------------------", httpd->ip_client);
	write_log_connection (httpd->fd, buffer);

	g_hash_table_remove (active_httpds, httpd);

	close( httpd->fd );

	httpd->watcher=0;
	if (httpd->ip_client_xff)
		g_free(httpd->ip_client_xff);
    g_free(httpd);
}



static void add_session (webpidgin_client_t *httpd)
{
	GSList * gpointer;
	webpidgin_session * session;


	purple_debug_info("WebPidgin 2/SHAO","Adding session %s %s\n", httpd->ip_client_xff?httpd->ip_client_xff:"", httpd->user_agent);
	
	gpointer = last_sessions;
	while (gpointer)
	{
		// SI SON IGUALES LAS 3 LA BORRO
		session = (webpidgin_session *) gpointer->data;

		if ( !g_ascii_strcasecmp (session->ip_client, httpd->ip_client) &&
				( session->ip_client_xff == httpd->ip_client_xff  || !g_ascii_strcasecmp (session->ip_client_xff, httpd->ip_client_xff) ) &&
				( session->user_agent == httpd->user_agent || !g_ascii_strcasecmp (session->user_agent, httpd->user_agent) ) )
		{

			purple_debug_info("WebPidgin 2/SHAO","Session found, updating\n");
			g_free (session->user_agent);
			g_free (session->ip_client_xff);
			g_free (session);

			last_sessions = g_slist_delete_link (last_sessions, gpointer);
			gpointer = last_sessions;

		}else
			gpointer = g_slist_next(gpointer);
	}
	//INTRODUZCO LA SESION
	session = g_malloc(sizeof(webpidgin_session));

	session->last_connection = time(NULL);
	g_strlcpy (session->ip_client, httpd->ip_client, INET_ADDRSTRLEN);
	session->ip_client_xff = g_strdup (httpd->ip_client_xff);
	session->user_agent = g_strdup (httpd->user_agent);
	session->auth_ok = httpd->auth_ok;

	last_sessions= g_slist_prepend (last_sessions, session);
}


/**
 * @brief callback from Pidgin if the client browser wrote to or closed our socket
 */
static void client_request_cb(gpointer data, gint sock, PurpleInputCondition cond)
{
    webpidgin_client_t *httpd = ( webpidgin_client_t * ) data;
    int len, vret;
    char buffer[4096];

    ///FIXME: normally we would read the data on this socket, but for now we just close it and
    /// release our watcher.
    purple_debug_info("WebPidgin 2","http_request_cb ok\n");
    if( sock != httpd->fd )
    {
        purple_debug_info("WebPidgin 2","http_request_cb yuck bad corruption\n");
        return;
    }

    /// Read the request here, always read less than our buffer
    /// The client side should never send us more then 4K of data, that would be silly
    len = read( httpd->fd, buffer,4095 );
    if( len < 0 )
        return;

    if( len == 4095 )
    {
        /// if we got too much data, abort (that should not happen)
        purple_debug_info("WebPidgin 2","http_request_cb yuck bad corruption\n");

		client_end_connection(httpd);
        return;
    }

    if( len == 0 )
    {
        /// client browser closed the socket
        /// cleanup
        purple_debug_info("WebPidgin 2","http_request_cb endpoint closed connection\n");
        purple_input_remove(httpd->watcher );

		client_end_connection(httpd);
        return;
    }

	//COMPRUEBO LA IP DE ORIGEN        
    if( gOptionRestrictAccessByIP && !purple_strequal (httpd->ip_client, gAllowedIP ) )
    {
        /// client connecting from blocked ip
        /// cleanup
        purple_debug_info("WebPidgin 2","http_request_cb denying access from IP[ %s ]\n", httpd->ip_client);
        purple_input_remove(httpd->watcher );

		client_end_connection(httpd);
        return;
    }
    
    buffer[len] = '\0';

    /// Parse our request
    vret = client_parse_and_dispatch( httpd, buffer,len );
    if( ! vret )
    {
        purple_debug_info("WebPidgin 2","unknown request\n");
        client_write_header( httpd,"/" );
        client_write(httpd,"Borked: WebPidgin was handed a URL it does not know how to handle. ( Closing Session )");
        client_write_tail( httpd );
        purple_input_remove(httpd->watcher );

		client_end_connection(httpd);
        return;
    }

	if (vret<10)
	{
    	/// Always close the endpoint for now
	    /// we may want to leave the socket open for more stuff later
	    purple_input_remove(httpd->watcher );

		client_end_connection(httpd);
	}
    return;
}


/**
 * @brief Pidgin indicated we got a hit to our listen socket ( server socket )
 * @brief we need to accept the client socket and start up a httpd client listener
 */
static void webpidgin_connect_request_cb(gpointer data, gint sock, PurpleInputCondition cond)
{
    webpidgin_data_t * webpidgin = ( webpidgin_data_t * ) data;
    webpidgin_client_t *httpd;
    int acceptfd;
	char ip_client[INET_ADDRSTRLEN];
	char buffer [1024];

	struct sockaddr_in client;
	int sin_size;
	sin_size=sizeof(struct sockaddr_in);

    // FIXME: we will croak here right now
    if( sock == -1 )
        return;

	//acceptfd = accept(sock,NULL,0 );
	acceptfd = accept(sock, (struct sockaddr *)&client, (socklen_t *) &sin_size);

    if( acceptfd == -1 )
        return;

	inet_ntop(AF_INET, &client.sin_addr, ip_client, INET_ADDRSTRLEN);

	purple_debug_info("WebPidgin 2/SHAO", "%s \n", ip_client);
	g_snprintf(buffer, sizeof(buffer), "Opening connection from: %s", ip_client);
	write_log_connection (acceptfd, buffer);

    httpd = g_malloc(sizeof(webpidgin_client_t));
    if( !httpd )
    {
        close(acceptfd);
        return;
    }

    /// configure the client session object and tell Pidgin to listen for AVAILABLE READ data on it
    httpd->webpidgin = webpidgin;
    httpd->fd = acceptfd;
	g_strlcpy (httpd->ip_client, ip_client, INET_ADDRSTRLEN);
	httpd->ip_client_xff = NULL;
	httpd->auth_ok = 0;
	httpd->buffer = NULL;
	httpd->buffer_len = -1;
	g_hash_table_insert (active_httpds, httpd, NULL);
    httpd->watcher = purple_input_add( httpd->fd,PURPLE_INPUT_READ, client_request_cb, httpd );
}

/**
 * @brief callback for PURPLE 2.0+ when a listen socket is created
 */
static void webpidgin_listen_cb(int fd, gpointer data )
{
    webpidgin_data_t * webpidgin = ( webpidgin_data_t * ) data;

    if( fd >= 0 )
    {
        webpidgin->iListenWatcher = purple_input_add(fd, PURPLE_INPUT_READ, webpidgin_connect_request_cb, data );
        webpidgin->fdServerSocket = fd;
    }
    else
    {
        webpidgin->usListenPort = 0;
    }
}

/**
 * @brief callback when a Pidgin IM message was received
 */
static void received_im_msg_cb(PurpleAccount *account, char *sender, char *buffer,
                                   PurpleConversation *conv, int flags, void *data)
{

    purple_debug_info("WebPidgin 2-signals", "received-im-msg (%s, %s, %s, %s, %d)\n",
                                        purple_account_get_username(account), sender, buffer,
                                        (conv != NULL) ? purple_conversation_get_name(conv) : "(null)", flags);
    rss_add(conv, sender, buffer);
}

/**
 * @brief callback when a Pidgin CHAT message was received
 */
static void received_chat_msg_cb(PurpleAccount *account, char *sender, char *buffer,
                                   PurpleConversation *conv, int flags, void *data)
{

    purple_debug_info("WebPidgin 2-signals", "received-chat-msg (%s, %s, %s, %s, %d)\n",
                                        purple_account_get_username(account), sender, buffer,
                                        (conv != NULL) ? purple_conversation_get_name(conv) : "(null)", flags);

    rss_add(conv, sender, buffer);
}


/**
 * @brief callback when a Pidgin IM message was sent
 */
static void sent_im_msg_cb(PurpleAccount *account, const char *recipient, const char *buffer, void *data)
{
    purple_debug_info("WebPidgin 2-signals", "sent-im-msg (%s, %s, %s)\n",
                                       purple_account_get_username(account), recipient, buffer);
}

static void get_keys(gpointer key, gpointer value, gpointer user_data)
{
	GList** gl = user_data;
	*gl = g_list_prepend( *gl, key);
}

static void httpd_stop(webpidgin_data_t * webpidgin)
{		
	if (active_httpds)
	{	
		GList* gl = NULL;	
		/// DEBO HACER ESTO POR CULPA DE WIN32 Y SUS ANTICUADAS LIBRERIAS
		//gl = g_hash_table_get_keys(active_httpds);
		g_hash_table_foreach(active_httpds, get_keys, &gl);		
		
		purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2-SHAO", "Stopping http server, still %u clients active.\n", g_list_length (gl));
		
		while (gl)
		{					
			client_end_connection(gl->data);
			gl=gl->next;			
		}		
		
		g_list_free(gl);
	}

	/*int err;
	purple_input_get_error( webpidgin->iListenWatcher , &err);
	purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2-SHAO", "Swebpidgin->iListenWatcher: %d \n", err);
	*/
	if( webpidgin->fdServerSocket >= 0 )
    {
		purple_input_remove( webpidgin->iListenWatcher );
			
		close( webpidgin->fdServerSocket );
		webpidgin->fdServerSocket = -1;				
		
		/// Session connections will remain open until
		/// the client endpoint closes it ( this is ok )
		/// For paranoia reasons we may want to force drop them if we choose
	}
}



/**
 * @brief code to start/restart our httpd mini server
 */
static void httpd_restart(webpidgin_data_t * webpidgin)
{
    //unsigned usListenPort = 0;
    int usListenPort = 8888;

    free( webpidgin->auth );
    webpidgin->auth = NULL;

    webpidgin_load_pref();

    if( ( gOptionUsername != NULL) && ( gOptionPassword != NULL) )
    {
        if( strlen(gOptionUsername) > 0 )
        {
            char * temp = g_malloc( strlen(gOptionUsername) + strlen( gOptionPassword) + 2 );
            if( temp )
            {
                //unsigned len;
                strcpy( temp, gOptionUsername );
                strcat( temp,":");
                strcat( temp,gOptionPassword);
				webpidgin->auth = (char *)purple_base64_encode((const guchar *) temp,strlen(temp));
                purple_debug_info("WebPidgin 2","Load::Authorization Key set to [%s]\n",webpidgin->auth);
                free( temp );
            }
        }
    }

    if( gOptionPort > 1 || gOptionPort < 65536 )
    {
        usListenPort = gOptionPort;
    }
    
    purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2-SHAO", "Port: (%d).\n",usListenPort);
    
    if( usListenPort != webpidgin->usListenPort )
    {
        httpd_stop(webpidgin);
        webpidgin->usListenPort = usListenPort;
						
        if( ! purple_network_listen( usListenPort,SOCK_STREAM ,webpidgin_listen_cb, (gpointer)webpidgin ) )
        {
        	char buffer[1024];
        	g_snprintf(buffer,sizeof(buffer), "Loading::WebPidgin could not listen port: (%d).\n", usListenPort);
        	
            purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2", "%s", buffer);
            purple_notify_error(webpidgin_plugin, "Error", buffer, "Maybe another instance is still running. \nUnload/load webpidgin-z after a few seconds");
            webpidgin->usListenPort = 0;
            return;
        }
    }
    else
    {
        purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2", "Load::Port change not required ( using=%d )\n", usListenPort );
    }

}

/** The wegbaim global instance */
static webpidgin_data_t * webpidgin = NULL;


static gboolean plugin_load(PurplePlugin *plugin)
{
	webpidgin_plugin = plugin;

    webpidgin = g_malloc( sizeof( webpidgin_data_t ) );
    if( !webpidgin )
    {
        purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2", "WebPidgin could not allocate data\n");
        return FALSE;
    }
    memset( webpidgin,0, sizeof(webpidgin_data_t) );
    webpidgin->fdServerSocket = -1;    
    webpidgin->usListenPort = -1;    

    purple_network_init();

    httpd_restart(webpidgin);

    purple_signal_connect(purple_conversations_get_handle(), "received-im-msg", plugin,
                          PURPLE_CALLBACK(received_im_msg_cb), NULL);
    purple_signal_connect(purple_conversations_get_handle(), "sent-im-msg", plugin,
                          PURPLE_CALLBACK(sent_im_msg_cb), NULL);
    purple_signal_connect(purple_conversations_get_handle(), "received-chat-msg", plugin,
                          PURPLE_CALLBACK(received_chat_msg_cb), NULL);

	purple_signal_connect(purple_notify_get_handle(), "displaying-emails-notification",
						plugin, PURPLE_CALLBACK(notify_emails_cb), NULL);

	purple_signal_connect(purple_notify_get_handle(), "displaying-email-notification",
						plugin, PURPLE_CALLBACK(notify_email_cb), NULL);
						
	purple_signal_connect(purple_conversations_get_handle(), "chat-invited",
						plugin, PURPLE_CALLBACK(chat_invited_cb), NULL);
						
	purple_signal_connect(purple_xfers_get_handle(), "file-recv-complete", plugin,
								PURPLE_CALLBACK(auto_accept_complete_cb), NULL);

	active_httpds= g_hash_table_new(g_direct_hash, g_direct_equal);
	nonce_list = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2", "WebPidgin plugin loaded.\n");
        
    return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin)
{
	httpd_stop(webpidgin);        

    purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2", "WebPidgin plugin unloaded.\n");
    return TRUE;
}

static void apply_configuration()
{
    httpd_restart( webpidgin );
}

static void type_toggle_cb(GtkWidget *widget, gpointer data)
{
    if( strcmp(data,"use_color") == 0 )
    {
        gOptionUseColor = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionUseColor );
    }
    else if( strcmp(data,"use_bold_names") == 0  )
    {
        gOptionBoldNames = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionBoldNames );
    }
    else if( strcmp(data,"use_status_messages") == 0  )
    {
        gOptionStatusMessages = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionStatusMessages );
    }
    else if( strcmp(data,"use_www_frames") == 0  )
    {
        gOptionWWWFrames = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionWWWFrames );
    }
    else if( strcmp(data,"use_meta_refresh") == 0  )
    {
        gOptionMetaRefresh = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionMetaRefresh );
    }
    else if( strcmp(data,"use_meta_refresh_dynamic") == 0  )
    {
        gOptionMetaRefreshDynamic = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionMetaRefreshDynamic );
    }
    else if( strcmp(data,"use_rss_feed") == 0  )
    {
        gOptionRSSFeed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionRSSFeed );
    }
    else if( strcmp(data,"save_connection_log") == 0  )
    {
        gSaveConnectionLog = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gSaveConnectionLog );
    }
	else if( strcmp(data,"show_no_auth_sessions") == 0  )
    {
        gShowNoAuthSessions = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gShowNoAuthSessions );
    }
    else if( strcmp(data,"use_jscript") == 0  )
    {
        gUseJavascript = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gUseJavascript );
    }
    else if( strcmp(data,"use_jscript_only_refresh") == 0  )
    {
        gUseJSOnlyRef = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gUseJSOnlyRef );
    }
    else if( strcmp(data,"group_messages") == 0  )
    {
        gGroupMessages = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gGroupMessages );
    }
    else if( strcmp(data,"use_digest_auth") == 0  )
    {
        gOptionUseDigestAuth = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionUseDigestAuth );
    }
    else if( strcmp(data,"show_buddy_icon") == 0  )
    {
        gOptionShowBuddyIcon = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionShowBuddyIcon );
    }
    else if( strcmp(data,"restrict_access_by_ip") == 0  )
    {
        gOptionRestrictAccessByIP = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionRestrictAccessByIP );
    }
	else if( strcmp(data,"show_smileys") == 0  )
    {
        gOptionShowSmileys = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionShowSmileys );
    }
    else if( strcmp(data,"use_sounds") == 0  )
    {
        gUseSounds = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gUseSounds );
    }
    else if( strcmp(data,"use_custom_css") == 0  )
    {
        gUseCustomCSS = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gUseCustomCSS );
    }
}

static gboolean options_entry_cb(GtkWidget *widget, GdkEventFocus *evt, gpointer data)
{
    if (data == NULL)
        return FALSE;

    if (!strcmp(data, "server_port")) {
        //purple_prefs_set_string("/plugins/webpidgin/server_port", gtk_entry_get_text(GTK_ENTRY(widget)));        
        int size = 0;
        const char * str = gtk_entry_get_text(GTK_ENTRY(widget));
        sscanf( str,"%d",&size);
        webpidgin_save_pref_int( (char*)data, size );
    }
    else if (!strcmp(data, "server_user")) {
        purple_prefs_set_string("/plugins/webpidgin/server_user", gtk_entry_get_text(GTK_ENTRY(widget)));
    }
    else if (!strcmp(data, "server_password")) {
        purple_prefs_set_string("/plugins/webpidgin/server_password", gtk_entry_get_text(GTK_ENTRY(widget)));
    }
    else if (!strcmp(data, "allowed_ip")) {
        purple_prefs_set_string("/plugins/webpidgin/allowed_ip", gtk_entry_get_text(GTK_ENTRY(widget)));
    }
    else if (!strcmp(data, "meta_refresh_seconds")) {
        int sec = 0;
        const char * str = gtk_entry_get_text(GTK_ENTRY(widget));
        sscanf( str,"%d",&sec);
        webpidgin_save_pref_int( (char*)data, sec );
    }
    else if (!strcmp(data, "font_adjust")) {
        int size = 0;
        const char * str = gtk_entry_get_text(GTK_ENTRY(widget));
        sscanf( str,"%d",&size);
        webpidgin_save_pref_int( (char*)data, size );
    }
    else if (!strcmp(data, "show_n_last_sessions")) {
        int size = 0;
        const char * str = gtk_entry_get_text(GTK_ENTRY(widget));
        sscanf( str,"%d",&size);
        webpidgin_save_pref_int( (char*)data, size );
    }
    else if (!strcmp(data, "max_messages")) {
        int size = 0;
        const char * str = gtk_entry_get_text(GTK_ENTRY(widget));
        sscanf( str,"%d",&size);
        webpidgin_save_pref_int( (char*)data, size );
    }
    else
    {
        purple_debug_info("WebPidgin 2","UNHANDLED [%s]\n",(char*)data);
    }

    apply_configuration();
    return FALSE;
}


static GtkWidget * get_config_frame(PurplePlugin *plugin)
{
    GtkWidget *ret = NULL, *frame = NULL;
    GtkWidget *vbox = NULL;
    GtkWidget *vbox2 = NULL;
    GtkWidget *hbox = NULL;
    GtkWidget *entry = NULL;
    GtkWidget *toggle = NULL;
    GtkWidget *toggle2 = NULL;
    GtkWidget *spin_button = NULL;
	GtkSizeGroup *sg;
	GtkWidget *tab = NULL;

    ret=gtk_notebook_new();

    /// Web server segment 
    tab = gtk_vbox_new(FALSE, 5); 
    gtk_notebook_append_page((GtkNotebook *)ret, tab, NULL); 
    gtk_notebook_set_tab_label_text ((GtkNotebook *)ret, tab, _("Web Server"));
    
    frame = pidgin_make_frame(tab, _("Web Server Port:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);
             

  /*  entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 5);
    gtk_entry_set_text(GTK_ENTRY(entry), purple_prefs_get_string("/plugins/webpidgin/server_port"));
    g_signal_connect(G_OBJECT(entry), "focus-out-event",G_CALLBACK(options_entry_cb), "server_port");
*/

	sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
	gaim_gtk_prefs_labeled_spin_button(vbox, _("1-65535:"), "/plugins/webpidgin/server_port", 1, 65535, sg);


    frame = pidgin_make_frame(tab, _("Username:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 32);
    gtk_entry_set_text(GTK_ENTRY(entry), purple_prefs_get_string("/plugins/webpidgin/server_user"));
    g_signal_connect(G_OBJECT(entry), "focus-out-event",G_CALLBACK(options_entry_cb), "server_user");


    frame = pidgin_make_frame(tab, _("Password:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 32);
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
    gtk_entry_set_text(GTK_ENTRY(entry),purple_prefs_get_string("/plugins/webpidgin/server_password"));
    g_signal_connect(G_OBJECT(entry), "focus-out-event", G_CALLBACK(options_entry_cb), "server_password");


	frame = pidgin_make_frame(tab, _("More options:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

	toggle = gtk_check_button_new_with_mnemonic(_("Only allow access from the next IP/host"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/restrict_access_by_ip"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "restrict_access_by_ip");
	
	entry = gtk_entry_new();
	hbox = gtk_hbox_new(FALSE, 5);	
    gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 20);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 256);    
    gtk_entry_set_text(GTK_ENTRY(entry),purple_prefs_get_string("/plugins/webpidgin/allowed_ip"));
    g_signal_connect(G_OBJECT(entry), "focus-out-event", G_CALLBACK(options_entry_cb), "allowed_ip");

	if (!purple_prefs_get_bool("/plugins/webpidgin/restrict_access_by_ip"))
		gtk_widget_set_sensitive(GTK_WIDGET(entry), FALSE);
	g_signal_connect(G_OBJECT(toggle), "clicked",
					 G_CALLBACK(gaim_gtk_toggle_sensitive), entry);

    toggle = gtk_check_button_new_with_mnemonic(_("Use digest authentication instead of basic authentication"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_digest_auth"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_digest_auth");

	toggle = gtk_check_button_new_with_mnemonic(_("Enable RSS Feed"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_rss_feed"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_rss_feed");

	toggle = gtk_check_button_new_with_mnemonic(_("Save a connection log in .purple directory"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/save_connection_log"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "save_connection_log");

	///EXTRAS SECTION
	tab = gtk_vbox_new(FALSE, 5);
    gtk_notebook_append_page((GtkNotebook *)ret, tab, NULL);
    gtk_notebook_set_tab_label_text ((GtkNotebook *)ret, tab, _("Extra options"));
    
    frame = pidgin_make_frame(tab, _("Interface:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

    toggle = gtk_check_button_new_with_mnemonic(_("Color coded history"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_color"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_color");

    toggle = gtk_check_button_new_with_mnemonic(_("Bold Buddy Names"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_bold_names"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_bold_names");

    toggle = gtk_check_button_new_with_mnemonic(_("Buddy Status Messages"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_status_messages"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_status_messages");

	toggle = gtk_check_button_new_with_mnemonic(_("Show buddy icon"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/show_buddy_icon"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "show_buddy_icon");

	toggle = gtk_check_button_new_with_mnemonic(_("Show smileys"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/show_smileys"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "show_smileys");

    toggle = gtk_check_button_new_with_mnemonic(_("Enable Frames"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_www_frames"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_www_frames");



    toggle = gtk_check_button_new_with_mnemonic(_("Auto-refresh buddy list/status (unset this if javascript enabled)"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_meta_refresh"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_meta_refresh");
	
    vbox2 = gtk_vbox_new(FALSE, 5);
    hbox = gtk_hbox_new(FALSE, 5);	
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 20);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    
	sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
	spin_button = gaim_gtk_prefs_labeled_spin_button(vbox2, _("_Seconds:"),
			"/plugins/webpidgin/meta_refresh_seconds", 1, 3600, sg);
	if (!purple_prefs_get_bool("/plugins/webpidgin/use_meta_refresh"))
		gtk_widget_set_sensitive(GTK_WIDGET(spin_button), FALSE);
	g_signal_connect(G_OBJECT(toggle), "clicked",
					 G_CALLBACK(gaim_gtk_toggle_sensitive), spin_button);


    toggle2 = gtk_check_button_new_with_mnemonic(_("Dynamic Auto-Refresh"));
    gtk_box_pack_start(GTK_BOX(vbox2), toggle2, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle2),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_meta_refresh_dynamic"));
    g_signal_connect(G_OBJECT(toggle2), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_meta_refresh_dynamic");

	if (!purple_prefs_get_bool("/plugins/webpidgin/use_meta_refresh"))
		gtk_widget_set_sensitive ((GtkWidget *)toggle2, FALSE);
	g_signal_connect(G_OBJECT(toggle), "clicked",
					 G_CALLBACK(gaim_gtk_toggle_sensitive), toggle2);



    
    

    sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
    spin_button = gaim_gtk_prefs_labeled_spin_button(vbox, _("_Font Adjust:"),
            "/plugins/webpidgin/font_adjust", -10, 10, sg);

	sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
	gaim_gtk_prefs_labeled_spin_button(vbox, _("Last sessions to show:"),
			"/plugins/webpidgin/show_n_last_sessions", 1, 10, sg);

	
    hbox = gtk_hbox_new(FALSE, 5);	    
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);    
	toggle = gtk_check_button_new_with_mnemonic(_("Show non-authenticated sessions"));
    gtk_box_pack_start(GTK_BOX(hbox), toggle, TRUE, TRUE, 20);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/show_no_auth_sessions"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "show_no_auth_sessions");



	toggle = gtk_check_button_new_with_mnemonic(_("Use Javascript(Ajax) in conversations (increases the ammount of data to transfer)"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_jscript"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_jscript");


	vbox2 = gtk_vbox_new(FALSE, 5);
    hbox = gtk_hbox_new(FALSE, 5);	
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 20);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
	
    toggle2 = gtk_check_button_new_with_mnemonic(_("Don't use Ajax when sending messages \n(solves some issues with slow connections and mobile browsers) "));
    gtk_box_pack_start(GTK_BOX(vbox2), toggle2, TRUE, TRUE, 0);    
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle2),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_jscript_only_refresh"));
    g_signal_connect(G_OBJECT(toggle2), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_jscript_only_refresh");
	if (!purple_prefs_get_bool("/plugins/webpidgin/use_jscript"))
		gtk_widget_set_sensitive ((GtkWidget *)toggle2, FALSE);
	g_signal_connect(G_OBJECT(toggle), "clicked",
					 G_CALLBACK(gaim_gtk_toggle_sensitive), toggle2);
					 					 	
    toggle2 = gtk_check_button_new_with_mnemonic(_("Enable sounds (requires Flash 8.0 or later)"));
    gtk_box_pack_start(GTK_BOX(vbox2), toggle2, TRUE, TRUE, 0);    
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle2),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_sounds"));
    g_signal_connect(G_OBJECT(toggle2), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_sounds");
	if (!purple_prefs_get_bool("/plugins/webpidgin/use_jscript"))
		gtk_widget_set_sensitive ((GtkWidget *)toggle2, FALSE);
	g_signal_connect(G_OBJECT(toggle), "clicked",
					 G_CALLBACK(gaim_gtk_toggle_sensitive), toggle2);
					 

	sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
	spin_button = gaim_gtk_prefs_labeled_spin_button(vbox, _("Trim conversations after N messages:"),
			"/plugins/webpidgin/max_messages", 10, 20000, sg);

	toggle = gtk_check_button_new_with_mnemonic(_("Group messages in conversations"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/group_messages"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "group_messages");
                         
	
	toggle = gtk_check_button_new_with_mnemonic(_("Use custom.css file"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_custom_css"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_custom_css");

    gtk_widget_show_all(ret);
    
    return ret;
}

static PidginPluginUiInfo ui_info =
{
    get_config_frame,
    0, /* page_num (Reserved) */
    NULL,
    NULL,
    NULL,
    NULL
};


static PurplePluginInfo info =
{
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,                             /**< type           */
    PIDGIN_PLUGIN_TYPE,                             /**< ui_requirement */
    0,                                                /**< flags          */
    NULL,                                             /**< dependencies   */
    PURPLE_PRIORITY_DEFAULT,                            /**< priority       */

    "pidgin-webpidgin",                                   /**< id             */
    N_("WebPidginZ/Gaim"),                                    /**< name           */
    N_(WEBPIDGIN_VERSION),                              /**< version        */
                                                      /**  summary        */
    N_("Pidgin access over HTTP"),
                                                      /**  description    */
    N_("Allows access to all Pidgin features over a simple http interface such as those used in mobile phones."),
    "Sebastian Sobolewski <spsobole@yahoo.com>, Shaorang <shaorang@shaorang.com>",        /**< author         */
    N_("http://www.shaorang.com/webpidginz"), /**< homepage       */

    plugin_load,                                      /**< load           */
    plugin_unload,                                    /**< unload         */
    NULL,                                             /**< destroy        */

    &ui_info,                                         /**< ui_info        */
    NULL,                                             /**< extra_info     */
    NULL,
    NULL,
/* padding */
    NULL,
    NULL,
    NULL,
    NULL
};

static void init_plugin(PurplePlugin *plugin)
{
    purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2", "WebPidgin Init.\n");
    purple_prefs_add_none("/plugins/webpidgin");
    
    if (purple_prefs_exists ("/plugins/webpidgin/server_port") && purple_prefs_get_type("/plugins/webpidgin/server_port") != PURPLE_PREF_INT)
    {
    	purple_prefs_remove("/plugins/webpidgin/server_port");
    }
    
    purple_prefs_add_int("/plugins/webpidgin/server_port", 8888);
    purple_prefs_add_string("/plugins/webpidgin/server_user", "");
    purple_prefs_add_string("/plugins/webpidgin/server_password", "");
    purple_prefs_add_bool("/plugins/webpidgin/use_color", 1);
    purple_prefs_add_bool("/plugins/webpidgin/use_bold_names", 1);
    purple_prefs_add_bool("/plugins/webpidgin/use_status_messages", 1);
    purple_prefs_add_bool("/plugins/webpidgin/use_www_frames", 0);
    purple_prefs_add_bool("/plugins/webpidgin/use_meta_refresh", 0);
    purple_prefs_add_bool("/plugins/webpidgin/use_meta_refresh_dynamic", 0);
    purple_prefs_add_int("/plugins/webpidgin/meta_refresh_seconds", 180);
    purple_prefs_add_bool("/plugins/webpidgin/use_rss_feed", 1);
    purple_prefs_add_bool("/plugins/webpidgin/save_connection_log", 0);
    purple_prefs_add_int("/plugins/webpidgin/show_n_last_sessions", 3);
    purple_prefs_add_bool("/plugins/webpidgin/show_no_auth_sessions", 1);
    purple_prefs_add_bool("/plugins/webpidgin/use_jscript", 1);
    purple_prefs_add_bool("/plugins/webpidgin/use_jscript_only_refresh", 0);
    purple_prefs_add_int("/plugins/webpidgin/max_messages", 100);
    purple_prefs_add_bool("/plugins/webpidgin/group_messages", 1);
    purple_prefs_add_bool("/plugins/webpidgin/use_digest_auth", 1);
	purple_prefs_add_int("/plugins/webpidgin/font_adjust", 0);
	purple_prefs_add_bool("/plugins/webpidgin/show_buddy_icon", 0);
	purple_prefs_add_bool("/plugins/webpidgin/restrict_access_by_ip", 1);
	purple_prefs_add_string("/plugins/webpidgin/allowed_ip", "127.0.0.1");
	purple_prefs_add_bool("/plugins/webpidgin/show_smileys", 1);
	purple_prefs_add_bool("/plugins/webpidgin/use_sounds", 1);
	purple_prefs_add_bool("/plugins/webpidgin/use_custom_css", 0);
}

PURPLE_INIT_PLUGIN(WebPidgin, init_plugin, info)
