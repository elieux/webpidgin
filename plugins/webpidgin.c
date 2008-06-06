/** *******************************************************************
 *
 * @brief Runs an internal httpd (WWW server) and exposes a WWW/WAP
 * @brief interface to Pidgin converstaions.  This allows the use of
 * @brief pidgin (aka gaim ) using a simple web browser such as those 
 * @brief found in mobile phones and other minimal WAP devices.  
 *
 * @copyright (C) 2006,2007 Sebastian Sobolewski ( spsobole@thirdmartini.com )
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
 *   - Copy webpidgin.c into the gaim/plugins source directory.
 *   - Inside that directory run "make -f Makefile.mingw webpidgin.dll"
 *
 * Installing:
 *   - Follow the directions here: http://pidgin.sourceforge.net/faq.php#q94
 *   - Start Pidgin
 *   - By default the plugin launches the mini web server on port 8888
 *   - http://localhost:8888/ should display the Root/Home page
 */

#define WEBPIDGIN_VERSION "2.0-BXX"

/**
 * CHANGES:
 *     TODO:
 *       - Colorize History Display
 *       - Add History Searching using regex?
 *
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
 * Things I'm working on:
 *   - error handling 
 *   - Support for chat and chat room
 *   - closing/ending im/chats
 *   - Requests?
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

#include "gtkplugin.h"
#include "gtkutils.h"
#include "gtkblist.h"
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

/// Hardcode the port # for now
const char * gOptionUsername = NULL;
const char * gOptionPassword = NULL;
const char *  gOptionPort = NULL;
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

static gboolean gOptionRSSFeed = 1;

#define DREFRESH_MIN    4
#define DREFRESH_STEP   2 /// Cut in half at each hit



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
}webpidgin_client_t;

typedef int (*WebPidginActionCallback) (webpidgin_client_t *client, const char * extra);

typedef struct{
    const char * action;
    WebPidginActionCallback callback;
}webpidgin_parse_t;


static const char *empty_string ="";

/// Prototypes of functions, so they can be called before they're defined
static void webpidgin_save_pref_bool( const char * pref, unsigned enabled );
static void webpidgin_load_pref();
static int action_active_list( webpidgin_client_t * httpd, const char * notused );
static int action_options( webpidgin_client_t * httpd, const char * extra );
static int action_accounts( webpidgin_client_t * httpd, const char * notused );
static int action_root( webpidgin_client_t * httpd, const char * notused );
static int action_buddy_list( webpidgin_client_t * httpd, const char * notused );
static int action_login( webpidgin_client_t * httpd, const char * extra );
static int action_logout( webpidgin_client_t * httpd, const char * extra );
static int action_about( webpidgin_client_t * httpd, const char * extra );
static int action_history( webpidgin_client_t * httpd, const char * extra );

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
    gOptionPort = purple_prefs_get_string("/plugins/webpidgin/server_port");
    gOptionMetaRefreshSeconds = purple_prefs_get_int("/plugins/webpidgin/meta_refresh_seconds");
    gOptionRSSFeed = purple_prefs_get_bool("/plugins/webpidgin/use_rss_feed");
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

/**
 * @brief Find a PurpleBuddy object based on the buddy name
 */
static PurpleBuddy * find_buddy( char *name )
{
    PurpleBlistNode *gnode, *cnode, *bnode;
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

                if( strcmp(buddy->name,name)== 0 )
                    return buddy;
            }
        }
    }
    return NULL;
}

/**
 * @brief Find a conversation by name
 */
static PurpleConversation * find_conversation(char *name)
{
    GList *cnv;
    for (cnv = purple_get_conversations(); cnv != NULL; cnv = cnv->next) {
        PurpleConversation *conv = NULL;
        const char *convName = NULL;
        conv = (PurpleConversation *)cnv->data;
        convName = purple_conversation_get_name(conv);
        if ( (convName) && (strcmp(convName,name) == 0)) {
            return conv;
        }
    }
    return NULL;
}

static const char *self = "Self";
/**
 * @brief retreive the name that should appear on the UI based on the
 * @brief buddy.
 */
static const char * get_self_name( char * buddy )
{
    PurpleBuddy *b = find_buddy( buddy );
    /// If buddy is NULL, we return "Self" something is pretty wrong
    if( ! b )
        return self;

    /// If account is NULL, we return "Self" something is pretty wrong
    if( ! b->account )
        return self;

    /// If an alias is defined for this account, use it
    if( b->account->alias )
        return b->account->alias;

    /// Otherwise use the username
    return b->account->username;
}


static const char * time_stamp()
{
    static char now[255];
    sprintf(now,"time=%u&",(unsigned)time(NULL) );
    return now;
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
                    snprintf(hex,8,"%%%2x",object[idx]);
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
static void client_write( webpidgin_client_t * httpd, const char *buffer )
{
    /// FIXME: We need to LOOP_EINTR here
    (void) write( httpd->fd,buffer,strlen(buffer));
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
    client_write( httpd,"Server: WebPidgin\n");
    client_write( httpd,"Connection: close\n");
    client_write( httpd,"Content-type: text/html\n\n");
    client_write( httpd,"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
}

static void client_write_header( webpidgin_client_t * httpd, const char *update )
{
    char buffer[4096]; 
    client_write_http_header( httpd );
    client_write( httpd,"<html>\n");
    client_write( httpd," <head>\n");
    client_write( httpd,"  <meta http-equiv=\"Pragma\" content=\"no-cache\">\n");
    if ( gOptionMetaRefresh )
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
            snprintf(buffer,sizeof(buffer),"  <meta http-equiv=\"refresh\" content=\"%d\">\n",gLastRefreshInterval);
            client_write( httpd,buffer);
        }
    }
    client_write( httpd,"  <title>WebPidgin</title>\n");
    client_write( httpd," </head>\n");

    //
    //  If we Are In Frames We Behave Differently!!
    //
    if( gOptionWWWFrames )
    {
        if( ( strcmp(update,"/") == 0 ) || ( strcmp(update,"/ActiveList") == 0 ) )
        {
            client_write( httpd,"<body bgcolor=#EEEEEE>\n");
            snprintf(buffer,sizeof(buffer),"CMD: <A HREF=%s target=\"list\">U</A> | <A HREF=/Accounts target=\"conv\">A</A> | <A HREF=/Options target=\"conv\">O</A> | <A HREF=/Help target=\"conv\">?</A><HR>\n",update);
        }
        else
        {
            client_write( httpd,"<body>\n");
            snprintf(buffer,sizeof(buffer),"CMD: <A HREF=%s target=\"conv\">Update</A><HR> \n",update);
        }
    }
    else
    {
        //
        // Not framed; use the "Normal Way"
        //
        client_write( httpd,"<body>\n");
        snprintf(buffer,sizeof(buffer),"CMD: <A HREF=%s>U</A> | <A HREF=/>H</A> | <A HREF=/Accounts>A</A> | <A HREF=/Options>O</A> | <A HREF=/Help>?</A><HR>\n",update);
    }

    client_write( httpd,buffer);
}

/**
 * @brief write end of the httpd stream 
 */
static void client_write_tail( webpidgin_client_t * httpd )
{
    client_write( httpd,"</body>\n");
    client_write( httpd,"</html>\n");
    client_write( httpd,"\n\n");
}


static void webpidgin_show_buddy(webpidgin_client_t * httpd,const char * extra_html,PurpleBuddy *buddy )
{
    char buffer[4096];
    char * name = webpidgin_encode( buddy->name );

    if( buddy->alias )
    {
        snprintf(buffer,sizeof(buffer),"&nbsp;&nbsp; <A HREF=conversation?%s%s %s>%s</A>",time_stamp(),name,extra_html,buddy->alias);
    }
    else if( buddy->server_alias )
    {
        snprintf(buffer,sizeof(buffer),"&nbsp;&nbsp; <A HREF=conversation?%s%s %s>%s</A>",time_stamp(),name,extra_html,buddy->server_alias);
    }
    else
    {
        snprintf(buffer,sizeof(buffer),"&nbsp;&nbsp; <A HREF=conversation?%s%s %s>%s</A>",time_stamp(),name,extra_html,buddy->name);
    }
    g_free(name);
    client_write(httpd,buffer);


    /* Retrieve and display status message, if one exists */
    if (gOptionStatusMessages)
    {
        PurpleStatus *status;
        const char *status_msg = NULL;
        status = purple_presence_get_active_status(buddy->presence);
        status_msg = purple_value_get_string(purple_status_get_attr_value(status, "message"));
        if ((status_msg != NULL) && (*status_msg != '\0')) {
            char *stripped_status_msg = purple_markup_strip_html(status_msg);
            client_write(httpd," ");
            client_write(httpd,stripped_status_msg);
            g_free(stripped_status_msg);
        }
    }

    /* Idle - lifted largely from gaim/src/gtkblist.c */
    if (purple_presence_is_idle(buddy->presence))
    {
        time_t idle_secs = purple_presence_get_idle_time(buddy->presence);
        if (idle_secs > 0) {
            int ihrs, imin;
            time_t t;

            time(&t);
            purple_debug_info("WebPidgin 2","Buddy %s has been idle for %d sec\n", buddy->name, t - idle_secs);
            ihrs = (t - idle_secs) / 3600;
            imin = ((t - idle_secs) / 60) % 60;

            if (ihrs)
                snprintf(buffer,sizeof(buffer)," (Idle %dh %02dm)", ihrs, imin);
            else
                snprintf(buffer,sizeof(buffer)," (Idle %dm)", imin);
        }
        else
            snprintf(buffer,sizeof(buffer)," (Idle)");

        client_write(httpd,buffer);
    } else if (!purple_presence_is_available(buddy->presence)) {
        client_write(httpd," (Offline)");
    }
    client_write(httpd,"<BR>\n");
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
                snprintf(buffer,sizeof(buffer),"<B>%s</B><BR>\n",group->name);
                client_write(httpd,buffer);

                if( gtk_tree_model_iter_children(model,&child,parent) )
                    webpidgin_buddy_list_walk( httpd,extra_html,model,&child);
            }break;


            case PURPLE_BLIST_CONTACT_NODE:{
                PurpleContact *contact = (PurpleContact *)node;
                if ( contact->alias )
                {
                    /// not sure yet
                    //snprintf(buffer,sizeof(buffer),"<B>type=%d ( not sure )</B><BR>\n",node->type);
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

            default:
                snprintf(buffer,sizeof(buffer),"<B>type=%ds</B><BR>\n",node->type);
                client_write(httpd,buffer);
            break;
        }
    }while( gtk_tree_model_iter_next(model,parent) );
}


/**
 * @brief list all buddies that have active chat sessions and how many unread mesages we have
 * @brief (x) BuddyName [n] :: Where x is #unread messages, and n is a quick access key for mobile phones
 */
static void show_active_chats( webpidgin_client_t * httpd, const char * except )
{
    char buffer[4096];
    char extra_html[512];
//    unsigned count = 0;
//    PurpleBlistNode *gnode, *cnode, *bnode;
    GList *cnv;

    strcpy(extra_html, "");
    if( gOptionWWWFrames )
    {
        strncat(extra_html," target=\"conv\" ",511-strlen(extra_html));
    }

    /** Now see if what active conversations we have */
    client_write( httpd,"<B>Active:</B><BR>");        

    for (cnv = purple_get_conversations(); cnv != NULL; cnv = cnv->next) {
        PurpleConversation *conv;
        char * encoded_name = NULL;
        const char *name = NULL;
        unsigned unread = 0;

        conv = (PurpleConversation *)cnv->data;
        name =  purple_conversation_get_name(conv);
        encoded_name = webpidgin_encode( name );
        if (!encoded_name)
            continue;

        //unread = get_unread_count(conv);

        snprintf(buffer,sizeof(buffer),"&nbsp;&nbsp; <A HREF=conversation?%s%s %s>%s</A>(%d)<BR>\n", time_stamp(), encoded_name, extra_html, name, unread);
        client_write(httpd, buffer);
        g_free(encoded_name);
    }
    client_write( httpd,"<HR>");
}


static void client_write_last_update( webpidgin_client_t * httpd )
{
    if( gOptionMetaRefresh )
    {
        char buffer[128];
        time_t tme = time(NULL);
        struct tm *tm = localtime( &tme );
        snprintf(buffer,128,"%d:%02d:%02d&nbsp;(%d)",tm->tm_hour,tm->tm_min,tm->tm_sec,gLastRefreshInterval);
        client_write(httpd,"<HR>");
        client_write(httpd,buffer);
    }
}

static int action_active_list( webpidgin_client_t * httpd, const char * notused )
{
    GaimGtkBuddyList *gtkblist = gaim_gtk_blist_get_default_gtk_blist();
    GtkTreeIter iter;
    char extra_html[512];
    purple_debug_info("WebPidgin 2","%s\n",__FUNCTION__);

    client_write_header( httpd,"/ActiveList" );

    show_active_chats( httpd , NULL);

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
    snprintf(search,255,"%s=",param); /// in forms, the = always follows making this key unique

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
    snprintf(search,255,"%s=",param); /// in forms, the = always follows making this key unique

    /// If a form object has the named key then we are indeed selected
    key = strstr(data, search );
    if( ! key )
        return 0;

    snprintf(search,255,"%s=%%d&",param); /// in forms, the = always follows making this key unique
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

        value = webpidgin_get_param_int( extra,"meta_refresh_seconds");
        if( value >=1 && value <= 3600 )
        {
            webpidgin_save_pref_int( "meta_refresh_seconds", value );
        }

        webpidgin_load_pref();
        return action_root(httpd,NULL); // return to root page after preferences saved & reloaded
    }

    client_write_header( httpd,"/Options" );
    client_write(httpd,"<B>WebPidgin Options:</B><BR>\n");
    client_write(httpd,"<form method=\"get\" target=\"_top\" action=\"/Options?\">\n");
    client_write(httpd,"<div>\n");

    ///
    /// Don't let people change their username and password
    /// over the web
    ///
    client_write(httpd,"&nbsp;&nbsp;Username: N/A<BR>\n");
    client_write(httpd,"&nbsp;&nbsp;Password: N/A<BR>\n");
    client_write(httpd,"<BR>\n");

    snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_color %s>Color Coded History<BR>\n",gOptionUseColor ? "checked" : "");
    client_write(httpd,buffer);

    snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_bold_names %s>Bold Buddy Names<BR>\n",gOptionBoldNames ? "checked" : "");
    client_write(httpd,buffer);

    snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_status_messages %s>Buddy Status Messages<BR>\n",gOptionStatusMessages ? "checked" : "" );
    client_write(httpd,buffer);

    snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_www_frames %s>Enable Frames<BR>\n",gOptionWWWFrames ? "checked" : "");
    client_write(httpd,buffer);

    snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_meta_refresh %s>Auto-refresh  list/status<BR>\n",gOptionMetaRefresh ? "checked" : "");
    client_write(httpd,buffer);

    snprintf(buffer,1024,"&nbsp;&nbsp;&nbsp;&nbsp;Seconds: <input type=text value=\"%d\" size=4 maxlength=4 name=meta_refresh_seconds><BR>\n",gOptionMetaRefreshSeconds);
    client_write(httpd,buffer);

    snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_meta_refresh_dynamic %s>Dynamic Refresh<BR>\n",gOptionMetaRefreshDynamic ? "checked" : "");
    client_write(httpd,buffer);


    snprintf(buffer,1024,"&nbsp;&nbsp;<input type=checkbox name=use_rss_feed %s>Enable RSS Feed<BR>\n",gOptionRSSFeed ? "checked" : "");
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
    char buffer[4096]; // yuck buffer overflow waiting to happen!!
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
        snprintf(display,1024,"%s (%s)",user,proto);

        encoded_user = webpidgin_encode( user );
        if( encoded_user == NULL )
            return 0;

        if( purple_account_is_connected( (PurpleAccount*) account_iter->data ) )
        {
            purple_debug_info("WebPidgin 2","[%s] connected\n",display);
            snprintf(buffer,sizeof(buffer),"&nbsp;<A HREF=logout?%s&%s target=\"_top\">(logout)</A> %s<BR>\n",
                                 encoded_user,proto,display);

            client_write(httpd,buffer);
        }
        else
        {
            purple_debug_info("WebPidgin 2","[%s] disconnected\n",display);
            snprintf(buffer,sizeof(buffer),"&nbsp;<A HREF=login?%s&%s target=\"_top\">(login)</A> %s<BR>\n",
                            encoded_user,proto,display);
            client_write( httpd,buffer);
        }
        g_free(encoded_user);
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
        client_write( httpd,"<html>\n");
        client_write( httpd," <head>\n");
        client_write( httpd,"  <title>WebPidgin</title>\n");
        client_write( httpd,"  <noframes><body>Your browser can not handle frames.<BR>\n");
        client_write( httpd,"  Please <A HREF=/Options>click here</A> and uncheck Enable Frames</body></noframes>\n");
        client_write( httpd,"   <FRAMESET COLS=\"160, *\" BORDER=1 ID=fs1>");
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
 * @brief Display the entire known buddy list
 */
static int action_buddy_list( webpidgin_client_t * httpd, const char * notused )
{
    GaimGtkBuddyList *gtkblist = gaim_gtk_blist_get_default_gtk_blist();
    GtkTreeIter iter;
    char extra_html[512];

    strcpy(extra_html,"");
    if( gOptionWWWFrames )
    {
        strncat(extra_html," target=\"conv\" ", 511-strlen(extra_html));
    }

    client_write_header(httpd,"/BuddyList");
    if( gtk_tree_model_get_iter_first(GTK_TREE_MODEL(gtkblist->treemodel),&iter) )
    {
        webpidgin_buddy_list_walk(httpd,extra_html,GTK_TREE_MODEL(gtkblist->treemodel),&iter);
    }
    client_write_tail(httpd);
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
    return action_root(httpd,NULL); // return to root page
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
    return action_root(httpd,NULL); // return to root page
}


/**
 * @brief handle a click on a buddy, which opens the "chat" view
 * @brief then handle any new message updates, etc.
 */
static int action_conversation( webpidgin_client_t * httpd, const char * extra )
{
    char buffer[1024];
    char * name;
    const char * self;
    PurpleConversation *conv;
    unsigned msgCount=0;

    purple_debug_info("WebPidgin 2","conversation::Extra[%s]\n",extra);


    name = webpidgin_normalize( extra );

    self = get_self_name( name ); /// Get the name we will use for ourselves

    snprintf(buffer,1024,"/conversation?%s%s",time_stamp(),extra);
    client_write_header( httpd,buffer);

    /// Now our web form for the chat
    client_write(httpd,"<form method=\"get\" action=\"/sendMessage?\">\n");
    client_write(httpd,"<div>\n");
    snprintf(buffer,1024,"<input style=\"-wap-input-format: *m\" name=\"%s\" tabindex=1 size=48 maxlength=128/>\n",name);
    client_write(httpd,buffer);
    client_write(httpd,"<input type=\"submit\" value=\"Send\" tabindex=2/>\n");
    client_write(httpd,"</div>\n");
    client_write(httpd,"</form>\n");

    conv = find_conversation(name);
    if (conv) {
        GList *histIter;
        msgCount = 0;
        for (histIter=purple_conversation_get_message_history(conv);histIter!=NULL;histIter=histIter->next) {
            PurpleConvMessage *msg = histIter->data;
            time_t when; 
            struct tm *tm; 

            if (msgCount>=uOptionTrim) {
                /** Don't show more then uOptionTrim Messages */
                break;
            }

            when = msg->when;
            tm = localtime( &when );


            snprintf(buffer,1024,"(%d:%02d:%02d)&nbsp;",tm->tm_hour,tm->tm_min,tm->tm_sec);
            client_write(httpd,buffer);

            if( gOptionBoldNames )
                client_write(httpd,"<B>");

            client_write(httpd, msg->who);

            if( gOptionBoldNames )
                client_write(httpd,"</B>");

            client_write(httpd,":&nbsp;"); /// Add a space, otherwise things blend in to much
            {
                char * tmpstr = purple_markup_strip_html(msg->what);
                client_write(httpd,tmpstr);
                g_free(tmpstr);
            }
            client_write(httpd,"<BR>\n");
            msgCount++;
        }
    } else {
        purple_debug_info("WebPidgin 2","conversation::Could not locate conversation for %s\n", name);
    }
    /// Show any active chats from other users. IE we're talking to bob, but in the meantime sally IM-ed us
    /// show sally has an unread message for us. ( Just like the Active: buddies section on the root page )
    show_active_chats( httpd, name );
    g_free( name );

    client_write_tail( httpd );
    return 1;
}

/**
 * @brief Execute the http-post click
 */
static int action_sendMessage( webpidgin_client_t * httpd, const char * extra )
{
    char * message = NULL;
    purple_debug_info("WebPidgin 2","sendMessage::Extra[%s]\n",extra);

    /// FIRST we need to see if we can find a conversation window with this chat already in there
    gMissedRefreshes = 0;
    message = strstr(extra,"=");
    if( message )
    {
        char * name;
        char * normal;
        char * encoded_name= NULL;
        PurpleConversation *c;
        PurpleConversationType type;

        *message='\0';
        message++;

        name = webpidgin_normalize(extra);

        c = find_conversation(name);
        if (!c) {
            PurpleBuddy *b = NULL;
            // Create the conversation ( assume extra is a buddy )
            b=find_buddy(name);
            if (!b) {
                g_free(name);
                return 0;
            }

            c = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, b->name, b->account);
            if (!c) {
                c = purple_conversation_new(PURPLE_CONV_TYPE_IM, b->account, b->name);
            }

            if (!c) {
                g_free(name);
                return 0;
            }
        }

        normal = webpidgin_normalize( message );
        type = purple_conversation_get_type(c);

        switch (type) {
            case PURPLE_CONV_TYPE_IM:
                purple_conv_im_send( PURPLE_CONV_IM(c), normal);
            break;

            case PURPLE_CONV_TYPE_CHAT:
                purple_conv_chat_send( PURPLE_CONV_CHAT(c), normal);
            break;

            default:
                purple_debug_info("WebPidgin 2","sendMessage::UnhandledType[%d]\n",type);
            break;
        }

        g_free(name);
        g_free(normal);

        encoded_name = webpidgin_encode(purple_conversation_get_name(c));
        action_conversation(httpd, encoded_name);
        g_free(encoded_name);
    }
    else
    {
        return action_root(httpd,NULL);
    }
    return 1;
}


/**
 * @brief Print the GPL license and a web link to the WebPidgin website
 */
static int action_about( webpidgin_client_t * httpd, const char * extra )
{
    purple_debug_info("WebPidgin 2","%s\n",__FUNCTION__);
    client_write_header( httpd,"/About" );
    client_write(httpd,"<FONT size=+1>CopyRight (C) 2006,2007 Sebastian Sobolewski<BR>\n");
    client_write(httpd,"Additional Contributors:<BR>\n");
    client_write(httpd,"David Morse<BR>\n");
    client_write(httpd,"Version ");
    client_write(httpd,WEBPIDGIN_VERSION);
    client_write(httpd,"</FONT><BR>\n");
    client_write(httpd,"<A HREF=http://www.thirdmartini.com/index.php/WebPidgin_2.x>WebPidgin Home Page</A><HR>\n");
    client_write(httpd,"<PRE>");
    client_write(httpd,license);
    client_write(httpd,"</PRE>");
    client_write_tail( httpd );
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


    snprintf(buffer,1024,"/history?%s%s",time_stamp(),extra);
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
    snprintf(buffer,1024,"/Status?%s",time_stamp());
    client_write_header( httpd,buffer);

    client_write(httpd,"WebPidgin Version: ");
    client_write(httpd,WEBPIDGIN_VERSION);
    client_write(httpd,"<BR><BR>\n");

//    my_addr = purple_network_get_local_system_ip(httpd->fd);
//    snprintf(buffer,1024,"System IP: %s<BR>\n", my_addr? my_addr : "N/A" );
//    client_write(httpd,buffer);

    my_addr = purple_network_get_my_ip(-1);
    snprintf(buffer,1024,"Public IP: %s<BR>\n", my_addr? my_addr : "N/A" );
    client_write(httpd,buffer);

    snprintf(buffer,1024,"RSS Feed: %s<BR>\n",gOptionRSSFeed ? "Enabled" : "Disabled");
    client_write(httpd,buffer);

    snprintf(buffer,1024,"Auto Refresh: %s",gOptionMetaRefresh ? "Enabled" : "Disabled");
    client_write(httpd,buffer);

    if( gOptionMetaRefresh )
    {
        snprintf(buffer,1024," (%d sec)",gOptionMetaRefreshSeconds);
        client_write(httpd,buffer);
    }

    if( gOptionMetaRefreshDynamic )
    {
        snprintf(buffer,1024," *Dynamic*");
        client_write(httpd,buffer);
    }
    client_write(httpd,"<BR>\n");


    client_write(httpd,"<HR>\n");
    //More stuff here like, e-mail notices etc
    //client_write(httpd,"<B>New E-Mail:</B>");
    //client_write(httpd,"Updates");
    strcpy(buffer,"");
    show_active_chats( httpd, buffer );

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

static int action_rss( webpidgin_client_t * httpd, const char * unused )
{
    return 0;
#if 0
	const char *my_addr = NULL;
	const char *rfc822_time = NULL;
    char buffer[1024];


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
        snprintf(buffer,sizeof(buffer),"  <link>http://%s:%s/</link>\n",my_addr,gOptionPort);
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
            rfc822_time = purple_utf8_strftime("%a, %d %b %Y %H:%M:%S %z", localtime(&item->conv->tm));
            if (rfc822_time != NULL)
            {
                snprintf(buffer,sizeof(buffer),"  <pubDate>%s</pubDate>\n", rfc822_time);
                client_write( httpd,buffer);
            }

            // Write a <lastBuildDate> = now, since we generate the feed on-the-fly
            rfc822_time = purple_utf8_strftime("%a, %d %b %Y %H:%M:%S %z", NULL);
            if (rfc822_time != NULL)
            {
                snprintf(buffer,sizeof(buffer),"  <lastBuildDate>%s</lastBuildDate>\n", rfc822_time);
                client_write( httpd,buffer);
            }
        }
        if( ( item->conv != NULL ) && ( item->conv->whom != CONV_SOURCE_SELF ))
        {
            client_write(httpd,"  <item>\n");
            snprintf(buffer,sizeof(buffer),"   <guid isPermaLink=\"false\">%d-%d</guid>\n",(int)item->conv->tm,item->conv->id);
            client_write(httpd,buffer);
            client_write(httpd,"   <title>");
            snprintf(buffer,sizeof(buffer),"New Message From %s",item->conv->chat->buddy);
            client_write(httpd,buffer);
            client_write(httpd,"</title>\n");
        	if (my_addr != NULL)
        	{
                char * name = webpidgin_encode(item->conv->chat->buddy);
                client_write(httpd,"   <link>");
                snprintf(buffer,sizeof(buffer),"http://%s:%s/chat?%s",my_addr,gOptionPort,name);
                g_free(name);
                client_write(httpd,buffer);
                client_write(httpd,"</link>\n");
            }
            client_write(httpd,"   <description>");

            {
                char * tmpstr = webpidgin_rss_strip_html(item->conv->message);
                snprintf(buffer,sizeof(buffer),"%s",tmpstr);
                g_free( tmpstr );
            }

            client_write(httpd,buffer);
            client_write(httpd,"</description>\n");
            rfc822_time = purple_utf8_strftime("%a, %d %b %Y %H:%M:%S %z", localtime(&item->conv->tm));
            if (rfc822_time != NULL)
            {
                snprintf(buffer,sizeof(buffer),"   <pubDate>%s</pubDate>\n", rfc822_time);
                client_write( httpd,buffer);
            }
            client_write(httpd,"  </item>\n");
        }
    }
    client_write(httpd," </channel>\n</rss>\n");
    client_write(httpd,"\n\n");
    return 1;
#endif
}

static int action_help( webpidgin_client_t * httpd, const char * unused )
{
    const char * my_addr = NULL;
    char buffer[1024];
    snprintf(buffer,1024,"/Help?%s",time_stamp());
    client_write_header( httpd,buffer);
    client_write(httpd,"<B>Help:</B><BR><BR>\n");
    
    client_write(httpd,"1. <B>RSS Feed</B>\n");
    client_write(httpd,"<div style=\"margin-left: 20px;\">\n");
    my_addr = purple_network_get_my_ip(-1);

    snprintf(buffer,1024,"When Enabled the RSS feed is available at http://%s:%s/status.rss<BR>\n", my_addr ? my_addr : "localhost", gOptionPort);
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
    client_write(httpd,"<div style=\"margin-left: 20px;\">\n");
    client_write(httpd,"Copyright (C) 2006,2007 Sebastian Sobolewski<BR>\n");
    client_write(httpd,"Additional Contributors:<BR>\n");
    client_write(httpd,"&nbsp;&nbsp;David Morse<BR>\n");
    client_write(httpd,"<BR>\n");
    client_write(httpd,"WebPidgin Version: ");
    client_write(httpd,WEBPIDGIN_VERSION);
    client_write(httpd,"<BR>\n");
    client_write(httpd,"<A HREF=http://www.thirdmartini.com/index.php/WebPidgin_2.x>WebPidgin Home Page</A><BR>\n");
    client_write(httpd,"</div><BR>\n");

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
    { "/", action_root },
    { "/ActiveList", action_active_list },
    { "/BuddyList", action_buddy_list },
    { "/login",action_login },
    { "/logout",action_logout },
    { "/conversation",action_conversation },
    { "/sendMessage",action_sendMessage },
    { "/Accounts",action_accounts },
    { "/Options",action_options },
    { "/About",action_about },
    { "/history",action_history },
    { "/Status",action_status },
    { "/status.rss",action_rss },
    { "/Help", action_help }
};
///****************************************************************************************************///
///  WEBSERVER CODE:  The code below deals with the webserver micro engine
///****************************************************************************************************///
/// Base64 code borrowed from http://hostap.epitest.fi/wpa_supplicant/devel/base64_8c-source.html
///
static const unsigned char base64_table[64] =
         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static unsigned char * base64_encode(const unsigned char *src, size_t len,
                               size_t *out_len)
{
    unsigned char *out, *pos;
    const unsigned char *end, *in;
    size_t olen;
    int line_len;
    
    olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
    olen += olen / 72; /* line feeds */
    olen++; /* nul termination */
    out = malloc(olen);
    if (out == NULL)
        return NULL;

    end = src + len;
    in = src;
    pos = out;
    line_len = 0;
    while (end - in >= 3) 
    {
        *pos++ = base64_table[in[0] >> 2];
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = base64_table[in[2] & 0x3f];
        in += 3;
        line_len += 4;
    }

    if (end - in) 
    {
        *pos++ = base64_table[in[0] >> 2];
        if (end - in == 1) 
        {
            *pos++ = base64_table[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
        else 
        {
            *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
            *pos++ = base64_table[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
        line_len += 4;
    }

    *pos = '\0';
    if (out_len)
        *out_len = pos - out;
    return out;
}

#if 0 /// Not using decode at this time.  save ourselves the compile warning
static unsigned char * base64_decode(const unsigned char *src, size_t len,size_t *out_len)
{
    unsigned char dtable[256], *out, *pos, in[4], block[4], tmp;
    size_t i, count, olen;

    memset(dtable, 0x80, 256);
    for (i = 0; i < sizeof(base64_table); i++)
    {
        dtable[base64_table[i]] = i;
    }

    dtable['='] = 0;
 
    count = 0;
    for (i = 0; i < len; i++) 
    {
        if (dtable[src[i]] != 0x80)
        {
            count++;
        }
    }
 
    if (count % 4)
    {
        return NULL;
    }
 
    olen = count / 4 * 3;
    pos = out = malloc(count);
    if (out == NULL)
    {
        return NULL;
    }
 
    count = 0;
    for (i = 0; i < len; i++) 
    {
        tmp = dtable[src[i]];
        if (tmp == 0x80)
                continue;
 
        in[count] = src[i];
        block[count] = tmp;
        count++;
        if (count == 4) 
        {
            *pos++ = (block[0] << 2) | (block[1] >> 4);
            *pos++ = (block[1] << 4) | (block[2] >> 2);
            *pos++ = (block[2] << 6) | block[3];
            count = 0;
        }
    }
 
    if (pos > out) {
        if (in[2] == '=')
            pos -= 2;
        else if (in[3] == '=')
            pos--;
    }

    *out_len = pos - out;
    return out;
}
#endif

/**
 * @brief send a AUTH-REJECT message to the client webbrowser if the user/pass
 * @brief was incorrect
 */
static int client_auth_reject( webpidgin_client_t * client )
{
    client_write(client,"HTTP/1.1 401 Authorization Required\n");
    /// FIXME: stamp the time and server name correctly
    client_write_date(client);
    client_write(client,"Server: webpidgin 2.0.0\n");;
    client_write(client,"WWW-Authenticate: Basic realm=\"Login\"\n");
    client_write(client,"Connection: close\n");
    client_write(client,"Content-type: text/html\n\n\n\n");
    return 1;
}

/**
 * @brief parse the URL request from the client webbrowser and dispatch it to
 * @brief the appropriate action_xxx call
 */
static int client_parse_and_dispatch(webpidgin_client_t *httpd, char * buffer, const int len )
{
    char * purl = NULL;
    
    purple_debug_info("WebPidgin 2","HTTP: [%s]\n", buffer);

    /// Id auth is enabled check the response
    if( httpd->webpidgin->auth )
    {
        char *temp = strdup(buffer);
        char *end = NULL;
        char *auth = strstr(temp,"Authorization: Basic ");

        if( !auth )
        {
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
            free( temp );
            /// wrong auth; deny
            return client_auth_reject(httpd);
        }
        free( temp );
    }
    
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
            /// nothing on the first pass check to see if we have extra info

            purple_debug_info("WebPidgin 2","No action for [%s]\n",purl);
        }
    }
    return 0;
}

/**
 * @brief callback from Pidgin if the client browser wrote to or closed our socket
 */
static void client_request_cb(gpointer data, gint sock, PurpleInputCondition cond)
{
    webpidgin_client_t *httpd = ( webpidgin_client_t * ) data;
    int len;
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
        close( httpd->fd );
        g_free(httpd);
        return;
    }

    if( len == 0 )
    {
        /// client browser closed the socket
        /// cleanup
        purple_debug_info("WebPidgin 2","http_request_cb endpoint closed connection\n");
        purple_input_remove(httpd->watcher );
        close( httpd->fd );
        g_free(httpd);
        return;
    }
    buffer[len] = '\0';

    /// Parse our request
    if( ! client_parse_and_dispatch( httpd, buffer,len ) )
    {
        purple_debug_info("WebPidgin 2","unknown request\n");
        client_write_header( httpd,"/" );
        client_write(httpd,"Borked: WebPidgin was handed a URL it does not know how to handle. ( Closing Session )");
        client_write_tail( httpd );
        purple_input_remove(httpd->watcher );
        close( httpd->fd );
        g_free(httpd);
        return;
    }

    /// Always close the endpoint for now
    /// we may want to leave the socket open for more stuff later
    purple_input_remove(httpd->watcher );
    close( httpd->fd );
    g_free(httpd);
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
    // FIXME: we will croak here right now
    if( sock == -1 )
        return;

    acceptfd = accept(sock,NULL,0 );
    if( acceptfd == -1 )
        return;

    httpd = g_malloc(sizeof(webpidgin_client_t));
    if( !httpd )
    {
        close(acceptfd);
        return;
    }

    /// configure the client session object and tell Pidgin to listen for AVAILABLE READ data on it
    httpd->webpidgin = webpidgin;
    httpd->fd = acceptfd;
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

    purple_debug_info("signals test", "received-im-msg (%s, %s, %s, %s, %d)\n",
                                        purple_account_get_username(account), sender, buffer,
                                        (conv != NULL) ? purple_conversation_get_name(conv) : "(null)", flags);

    // Add an RSS entry for this message
}

/**
 * @brief callback when a Pidgin IM message was sent
 */
static void sent_im_msg_cb(PurpleAccount *account, const char *recipient, const char *buffer, void *data)
{
    purple_debug_info("signals test", "sent-im-msg (%s, %s, %s)\n",
                                       purple_account_get_username(account), recipient, buffer);
    // Add an RSS entry for this message
}



/**
 * @brief code to start/restart our httpd mini server
 */
static void httpd_restart(webpidgin_data_t * webpidgin)
{
    unsigned usListenPort = 0;

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
                unsigned len;
                strcpy( temp, gOptionUsername );
                strcat( temp,":");
                strcat( temp,gOptionPassword);
                webpidgin->auth = (char *)base64_encode((const unsigned char*)temp,strlen(temp),&len);
                purple_debug_info("WebPidgin 2","Load::Authorization Key set to [%s]\n",webpidgin->auth);
                free( temp );
            }
        }
    }

    if( gOptionPort )
    {
        if( sscanf(gOptionPort,"%d",&usListenPort) != 1 )
        {
            usListenPort = webpidgin->usListenPort;
        }
    }

    if( usListenPort != webpidgin->usListenPort )
    {
        if( webpidgin->fdServerSocket >= 0 )
        {
            purple_input_remove( webpidgin->iListenWatcher );
            close( webpidgin->fdServerSocket );
            webpidgin->fdServerSocket = -1;
            /// Session connections will remain open until
            /// the client endpoint closes it ( this is ok )
            /// For paranoia reasons we may want to force drop them if we choose
        }
        webpidgin->usListenPort = usListenPort;


        if( !purple_network_listen( usListenPort,SOCK_STREAM ,webpidgin_listen_cb, (gpointer)webpidgin ) )
        {
            purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2", "Load::WebPidgin could not listen port (%d).\n",usListenPort);
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

    webpidgin = g_malloc( sizeof( webpidgin_data_t ) );
    if( !webpidgin )
    {
        purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2", "WebPidgin could not allocate data\n");
        return FALSE;
    }
    memset( webpidgin,0, sizeof(webpidgin_data_t) );
    webpidgin->fdServerSocket = -1;

    purple_network_init();
    
    httpd_restart(webpidgin);

    purple_signal_connect(purple_conversations_get_handle(), "received-im-msg",plugin, PURPLE_CALLBACK(received_im_msg_cb), NULL);
    purple_signal_connect(purple_conversations_get_handle(), "sent-im-msg",plugin, PURPLE_CALLBACK(sent_im_msg_cb), NULL);

    purple_debug(PURPLE_DEBUG_INFO, "WebPidgin 2", "WebPidgin plugin loaded.\n");
    return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin)
{
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
        gOptionMetaRefresh = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionMetaRefresh );
    }
    else if( strcmp(data,"use_rss_feed") == 0  )
    {
        gOptionRSSFeed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webpidgin_save_pref_bool( (char*)data, gOptionMetaRefresh );
    }

}

static gboolean options_entry_cb(GtkWidget *widget, GdkEventFocus *evt, gpointer data)
{
    if (data == NULL)
        return FALSE;

    if (!strcmp(data, "server_port")) {
        purple_prefs_set_string("/plugins/webpidgin/server_port", gtk_entry_get_text(GTK_ENTRY(widget)));
    }
    else if (!strcmp(data, "server_user")) {
        purple_prefs_set_string("/plugins/webpidgin/server_user", gtk_entry_get_text(GTK_ENTRY(widget)));
    }
    else if (!strcmp(data, "server_password")) {
        purple_prefs_set_string("/plugins/webpidgin/server_password", gtk_entry_get_text(GTK_ENTRY(widget)));
    }
    else if (!strcmp(data, "meta_refresh_seconds")) {
        int sec = 0;
        const char * str = gtk_entry_get_text(GTK_ENTRY(widget));
        sscanf( str,"%d",&sec);
        webpidgin_save_pref_int( (char*)data, sec );
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
    GtkWidget *entry = NULL;
    GtkWidget *toggle = NULL;
    GtkWidget *spin_button = NULL;
	GtkSizeGroup *sg;

    ret = gtk_vbox_new(FALSE, 18);
    gtk_container_set_border_width(GTK_CONTAINER (ret), 12);

    /// Web server segment
    frame = gaim_gtk_make_frame(ret, _("Web Server Port:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);
    
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 5);
    gtk_entry_set_text(GTK_ENTRY(entry), purple_prefs_get_string("/plugins/webpidgin/server_port"));
    g_signal_connect(G_OBJECT(entry), "focus-out-event",G_CALLBACK(options_entry_cb), "server_port");

    frame = gaim_gtk_make_frame(ret, _("Username:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 32);
    gtk_entry_set_text(GTK_ENTRY(entry), purple_prefs_get_string("/plugins/webpidgin/server_user"));
    g_signal_connect(G_OBJECT(entry), "focus-out-event",G_CALLBACK(options_entry_cb), "server_user");


    frame = gaim_gtk_make_frame(ret, _("Password:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 32);
    gtk_entry_set_text(GTK_ENTRY(entry),purple_prefs_get_string("/plugins/webpidgin/server_password"));
    g_signal_connect(G_OBJECT(entry), "focus-out-event", G_CALLBACK(options_entry_cb), "server_password");

    frame = gaim_gtk_make_frame(ret, _("Extras:"));
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

    toggle = gtk_check_button_new_with_mnemonic(_("Enable Frames"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_www_frames"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_www_frames");

    toggle = gtk_check_button_new_with_mnemonic(_("Auto-refresh buddy list/status"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_meta_refresh"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_meta_refresh");

	sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
	spin_button = gaim_gtk_prefs_labeled_spin_button(vbox, _("_Seconds:"),
			"/plugins/webpidgin/meta_refresh_seconds", 1, 3600, sg);
	if (!purple_prefs_get_bool("/plugins/webpidgin/use_meta_refresh"))
		gtk_widget_set_sensitive(GTK_WIDGET(spin_button), FALSE);
	g_signal_connect(G_OBJECT(toggle), "clicked",
					 G_CALLBACK(gaim_gtk_toggle_sensitive), spin_button);


    toggle = gtk_check_button_new_with_mnemonic(_("Dynamic Auto-Refresh"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_meta_refresh_dynamic"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_meta_refresh_dynamic");

    toggle = gtk_check_button_new_with_mnemonic(_("Enable RSS Feed"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     purple_prefs_get_bool("/plugins/webpidgin/use_rss_feed"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_rss_feed");

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
    N_("WebPidgin/Gaim"),                                    /**< name           */
    N_(WEBPIDGIN_VERSION),                              /**< version        */
                                                      /**  summary        */
    N_("Pidgin access over HTTP"),
                                                      /**  description    */
    N_("Allows access to all Pidgin features over a simple http interface such as those used in mobile phones."),
    "Sebastian Sobolewski <spsobole@yahoo.com>",        /**< author         */
    N_("http://www.thirdmartini.com/index.php/WebPidgin_2.x"), /**< homepage       */

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
    purple_prefs_add_string("/plugins/webpidgin/server_port", "8888");
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
}

PURPLE_INIT_PLUGIN(WebPidgin, init_plugin, info)
