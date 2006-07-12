/** *******************************************************************
 *
 * @brief Runs an internal httpd (WWW server) and exposes a WWW/WAP
 * @brief interface to gaim converstaions.  This allows the use of
 * @brief gaim using a simple web browser such as those found in 
 * @brief mobile phones and other minimal WAP devices.  
 *
 * @copyright (C) 2006 Sebastian Sobolewski ( spsobole@thirdmartini.com )
 * @url http://www.thirdmartini.com/index.php/WebGaim_2.x
 *
 ********************************************************************** */
static char  *license = "\
     This program is free software; you can redistribute it and/or \n \
    modify it under the terms of the GNU General Public License as \n \
    published by the Free Software Foundation; either version 2 of \n \
    the License, or (at your option) any later version. \n \
 \n \
    This program is distributed in the hope that it will be useful, \n \
    but WITHOUT ANY WARRANTY; without even the implied warranty of \n \
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU \n \
    General Public License for more details. \n \
 \n \
    You should have received a copy of the GNU General Public \n \
    License along with this program; if not, write to the Free \n \
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, \n \
    MA 02111-1307, USA. "; 

 /** ********************************************************************* 
 * Compiling:
 *   - Follow the directions here: http://gaim.sourceforge.net/win32/build.php
 *   - Copy webgaim.c into the gaim/plugins source directory.
 *   - Inside that directory run "make -f Makefile.mingw webgaim.dll"
 *
 * Installing:
 *   - Follow the directions here: http://gaim.sourceforge.net/faq.php#q94
 *   - Start GAIM
 *   - By default the plugin launches the mini web server on port 8888
 *   - http://localhost:8888/ should display the Root/Home page
 */

#define WEBGAIM_VERSION "2.0-BXX"

/**
 * CHANGES:
 *     TODO:
 *       - Colorize History Display
 *       - Add History Searching using regex?
 *
 *     2.0-BXX
 *       - Buddies in the WebGaim Buddy List will now be laid out in the
 *          same manner as in Gaim, including Groups. ( Buddies->Sort Buddies )
 *       - Removed support for Gaim < 2.0; code was getting too messy
 *       - Added Auto-Refresh with meta refresh tag
 *         + preference to enable/disable, set # seconds in plugin details
 *       - Status stub
 *       - RSS Status feed available at http://<ip>:<port>/status.rss
 *         + Compatible with Akregator
 *         + Passes ( http://www.feedvalidator.org/ )
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
 *       - Buddies with spaces now work, for reall this time.
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
 *   - WAP 2.0 (Sanyo RL-4920, others)
 *   - Any others?
 *
 * Things That DO work:
 *   - display Buddies with active messages ontop of chat view
 *   - login and out of accounts
 *   - IM conversations
 *   - continuing a IM conversation started using gaim
 *   - starting a new IM conversation using webgaim
 *   - root page displays new messages: (3) BuddyName
 *   - timestamp messages
 *   - quick access keys for active conversations. ( IE the [0] | [1] keys indicate the numeric key to hit on a phone for a quick link )
 *   - simple color support
 *   - trim conversations at uOptionTrim messages in a conversation.  Dropping oldest messages
 *   - do our best to unmangle web form data
 *   - compiles for Gaim 2.0+
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
 *    B = Buddy List ( Unfiltered )
 *    O = Options ( takes you Home at the moment )
 *
 * Home Page:
 *   Active: Lists all active conversations ( IE ones that have new text in them )
 *   Online: Lists All online buddies
 *   Accounts: Any accounts you have set up 
 *             + Allows you to login and log out
 *
 * Chatting:
 *   If you click on a Online or Active buddy the chat page will show up 
 *   From here you can send a IM to the buddy you selected.  
 *   ( As a side effect it will also open a gaim window on the PC running gaim )
 *
 * Things I'm working on:
 *   - Status page
 *   - error handling 
 *   - Support for chat and chat room
 *   - closing/ending im/chats
 *   - Requests?
 */
#include "internal.h"
#include "gtkgaim.h"
#include "gtkprefs.h"

#include "conversation.h"
#include "network.h"
#include "eventloop.h"
#include "prefs.h"
#include "signals.h"
#include "version.h"
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
static int gOptionMetaRefreshSeconds = 180; // refresh after 3 minutes (if enabled)
static gboolean gOptionRSSFeed = 1;

typedef struct{
    const char * name;
    const char * shortOnline;
    const char * shortOffline;
}webgaim_icon_t;

typedef struct{
    int iListenWatcher;
    int fdServerSocket;
    unsigned usListenPort;
    char * auth;
}webgaim_data_t;

typedef struct{
    webgaim_data_t *webgaim;
    int fd;
    int watcher; /** The gaim_input_add handle on this socket **/
}webgaim_client_t;

typedef int (*WebgaimActionCallback) (webgaim_client_t *client, const char * extra);

typedef struct{
    const char * action;
    WebgaimActionCallback callback;
}webgaim_parse_t;

/*
static webgaim_icon_t icons[] = {
    { "Yahoo","Y!","y." },
    { "AIM/ICQ","AIM","aim" },
    { "MSN","MSN","msn" },
    { "IRC","IRC","msn" },
    { "Jabber","JAB","jab" }
};*/

#define CONV_SOURCE_SELF 0
#define CONV_SOURCE_BUDDY 1

/** 
 * @brief Message List.  This structure keeps a single message sent 
 * @brief to a buddy, or received from a buddy.  This list may be auto-prunned
 * @brief Latest messages are always at the head of the list
 */

typedef struct webgaim_chat_s webgaim_chat_t;
typedef struct rss_item_s rss_item_t;
typedef struct webgaim_conv_s webgaim_conv_t;

struct webgaim_conv_s{
    struct webgaim_conv_s *prev; /** The previous message received */
    char * message;                  /** Message contents */
    int whom;                     /** Who originated this message: CONV_SOURCE_SELF | CONV_SOURCE_BUDDY */
    time_t tm;                     /** Time stamp of message */
    unsigned id;
    webgaim_chat_t *chat;
    rss_item_t *rssItem;
};

struct rss_item_s{
    struct rss_item_s *next;
    webgaim_conv_t * conv;
};

/**
 * @brief a chat/im conversation list. Keeps tabs on active conversations
 */
struct webgaim_chat_s{
    char * buddy;                /** The buddy whose conversation we're tracking */
    webgaim_conv_t * head;        /** The head of the messages list */
    unsigned unseen;             /** Count of unseen messages */
    unsigned id;
    struct webgaim_chat_s *next;  /** link to the next chat/conv session */
};



static const char *empty_string ="";
static webgaim_chat_t head = { NULL,NULL,0,0,NULL };
static rss_item_t rssHead = { NULL, NULL };

/// Workaround for buggy gcc warning re: strftime (http://www.die.net/doc/linux/man/man3/strftime.3.html)
static size_t webgaim_strftime(char *s, size_t max, const char *fmt, const struct tm *tm) 
{ 
    return strftime(s, max, fmt, tm); 
}

/// Prototypes of functions, so they can be called before they're defined
static void webgaim_save_pref_bool( const char * pref, unsigned enabled );
static void webgaim_load_pref();
static int action_active_list( webgaim_client_t * httpd, const char * notused );
static int action_options( webgaim_client_t * httpd, const char * extra );
static int action_accounts( webgaim_client_t * httpd, const char * notused );
static int action_root( webgaim_client_t * httpd, const char * notused );
static int action_buddy_list( webgaim_client_t * httpd, const char * notused );
static int action_login( webgaim_client_t * httpd, const char * extra );
static int action_logout( webgaim_client_t * httpd, const char * extra );
static int action_chat( webgaim_client_t * httpd, const char * extra );
static int action_send( webgaim_client_t * httpd, const char * extra );
static int action_about( webgaim_client_t * httpd, const char * extra );
static int action_history( webgaim_client_t * httpd, const char * extra );

/**
 * @brief load all webgaim prefs
 */
static void webgaim_load_pref()
{
    gOptionUseColor = gaim_prefs_get_bool("/plugins/webgaim/use_color");
    gOptionBoldNames = gaim_prefs_get_bool("/plugins/webgaim/use_bold_names");
    gOptionWWWFrames = gaim_prefs_get_bool("/plugins/webgaim/use_www_frames");
    gOptionMetaRefresh = gaim_prefs_get_bool("/plugins/webgaim/use_meta_refresh");
    gOptionStatusMessages = gaim_prefs_get_bool("/plugins/webgaim/use_status_messages");
    gOptionUsername = gaim_prefs_get_string("/plugins/webgaim/server_user");
    gOptionPassword = gaim_prefs_get_string("/plugins/webgaim/server_password");
    gOptionPort = gaim_prefs_get_string("/plugins/webgaim/server_port");
    gOptionMetaRefreshSeconds = gaim_prefs_get_int("/plugins/webgaim/meta_refresh_seconds");
}

/**
 * @brief Common pref save code, since we now use it in multiple places
 */
static void webgaim_save_pref_bool( const char * pref, unsigned enabled )
{
    gchar gcPref[256];
    g_snprintf(gcPref, sizeof(gcPref), "/plugins/webgaim/%s",  pref );
    gaim_prefs_set_bool(gcPref, enabled);
    gaim_debug_info("WebGaim 2","Saved-Pref: %s = [%d]\n",pref, enabled);
}

static void webgaim_save_pref_int( const char * pref, int value )
{
    gchar gcPref[256];
    g_snprintf(gcPref, sizeof(gcPref), "/plugins/webgaim/%s",  pref );
    gaim_prefs_set_int(gcPref, value );
    gaim_debug_info("WebGaim 2","Saved-Pref: %s = [%d]\n",pref, value);
}

static void rss_item_add( webgaim_conv_t * conv )
{
    rss_item_t * rssItem = NULL;
    rssItem = g_malloc( sizeof(webgaim_conv_t) );
    if(! rssItem )
        return;

    rssItem->conv   = conv;
    rssItem->next = rssHead.next;
    rssHead.next = rssItem;
    conv->rssItem = rssItem;
}


/**
 * @brief find the chat instance for this buddy 
 */
static webgaim_chat_t * chat_find( const char * buddy )
{
    webgaim_chat_t *chat = NULL;
    for( chat = head.next; chat != NULL; chat = chat->next )
    {
        if( strcmp( chat->buddy, buddy ) == 0 )
            return chat;
    }

    return NULL;
}

/**
 * @brief Create a new chat instance for this buddy
 */
static webgaim_chat_t * chat_new(const char * buddy )
{
    webgaim_chat_t *chat = NULL;

    chat = g_malloc( sizeof( webgaim_chat_t ) );
    if( chat )
    {
        chat->buddy = strdup( buddy );
        chat->unseen = 0;
        chat->head = NULL;

        chat->next = head.next;
        head.next = chat;
    }
    return chat;
}

/**
 * @brief Add a new message line into a chat list, if  the chat list
 * @brief for this buddy does not exist yet, create one
 */
static webgaim_chat_t * chat_add( const char * buddy, int whom, const char * data )
{
    webgaim_chat_t *chat = NULL;
    webgaim_conv_t * conv = NULL;
    
    /// We can lookup the sender and see who it was
    chat = chat_find( buddy );
    if( ! chat )
    {
        chat = chat_new( buddy );
    }

    conv = g_malloc( sizeof(webgaim_conv_t) );
    if( !conv )
        return NULL;

    gaim_debug_info("WebGaim 2","chat_add[%s]\n",data);

    conv->message = strdup( data );
    conv->whom = whom;
    conv->prev = chat->head;
    conv->tm = time(NULL);
    conv->id = head.id++;
    conv->chat = chat;
    chat->head = conv;
    chat->unseen++;

    if( gOptionRSSFeed )
    {
        rss_item_add( conv );
    }

    return chat;
}

/**
 * @brief trim the number of history messages for this chat based on the
 * @brief uOptionTrim global
 */
static void chat_trim(webgaim_chat_t * chat )
{
    webgaim_conv_t *conv;
    unsigned count = 0;
    for(conv=chat->head;conv!=NULL;conv=conv->prev )
    {
        count++;
        if( count >= uOptionTrim )
            break;
    }

    if( conv )
    {
        webgaim_conv_t *next;
        do{
            next = conv->prev;
            free( conv->message );
            conv->rssItem->conv = NULL;
            g_free( conv );
        }while( next );
    }
}

/**
 * @brief push a chat message that you SENT to a buddy to the message list
 */
static webgaim_chat_t * chat_send( const char * buddy, const char * data )
{
    webgaim_chat_t *chat = chat_add( buddy, CONV_SOURCE_SELF, data );
    return chat;
}

/**
 * @brief push a chat message that you RECEIVED from a buddy to the message list
 */
static webgaim_chat_t * chat_receive( const char * buddy, const char * data )
{
    webgaim_chat_t *chat = chat_add( buddy, CONV_SOURCE_BUDDY, data );
    return chat;
}


/**
 * @brief Find a GaimBuddy object based on the buddy name
 */
static GaimBuddy * find_buddy( char *name )
{
    GaimBlistNode *gnode, *cnode, *bnode;
    for (gnode = gaim_get_blist()->root; gnode != NULL; gnode = gnode->next)
    {
        if (!GAIM_BLIST_NODE_IS_GROUP(gnode))
            continue;

        for (cnode = gnode->child; cnode != NULL; cnode = cnode->next)
        {
            if (!GAIM_BLIST_NODE_IS_CONTACT(cnode))
                continue;

            for (bnode = cnode->child; bnode != NULL; bnode = bnode->next)
            {
                GaimBuddy *buddy = (GaimBuddy *)bnode;

                if ( ! GAIM_BUDDY_IS_ONLINE(buddy))
                    continue;

                if( strcmp(buddy->name,name)== 0 )
                    return buddy;
            }
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
    GaimBuddy *b = find_buddy( buddy );
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
static char * webgaim_encode( const char * object )
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
                    gaim_debug_info("WebGaim 2.x","key [%c][%2x]\n",object[idx],object[idx]);
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
    gaim_debug_info("WebGaim 2.x","coded [%s]\n",coded);
    return coded;
}


/**
 * @brief normalize a post-method encoded data to normal "ASCII"
 * @note FIXME: this is not the correct normalize function, just a quick hack
 */
static char * webgaim_normalize( const char * message )
{
    char * normal = g_malloc( strlen(message)*2 );
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
                gaim_debug_info("WebGaim 2.x","Got [%d] [%c]\n",ch,(unsigned char)ch);
                normal[current]=(unsigned char)ch;
            }break;

            default:
                normal[current] = message[idx];
            break;
        }
        current++;
    }
    normal[current]='\0';
    gaim_debug_info("WebGaim 2.x","Normal [%s]\n",normal);
    return normal;
}

/**
 * @brief send a string ( const char* ) message to the http client socket
 * @brief for this session.  the message must be NULL ('\0' ) terminated as
 * @brief we use strlen to trunc the size.  We do not send the '\0'
 */
static void client_write( webgaim_client_t * httpd, const char *buffer )
{
    write( httpd->fd,buffer,strlen(buffer));
}

/**
 * @brief Get the current time and insert it into the client stream 
 * @note FIXME
 */
static void client_write_date(webgaim_client_t * httpd)
{
    client_write(httpd,"Date: Sun, 21 Nov 2004 20:56:06 GMT\n"); 
}

/**
 * @brief Write a HTTPD header and additional WebGaim "toolbar" to the client stream
 */
static void client_write_http_header( webgaim_client_t * httpd )
{
    client_write( httpd,"");
    client_write( httpd,"HTTP/1.1 200 OK\n");
    client_write_date(httpd);
    client_write( httpd,"Server: WebGaim\n");
    client_write( httpd,"Connection: close\n");
    client_write( httpd,"Content-type: text/html\n\n");
    client_write( httpd,"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
}

static void client_write_header( webgaim_client_t * httpd, const char *update )
{
    char buffer[4096]; 
    client_write_http_header( httpd );
    client_write( httpd,"<html>\n");
    client_write( httpd," <head>\n");
    client_write( httpd,"  <meta http-equiv=\"Pragma\" content=\"no-cache\">\n");
    if ( gOptionMetaRefresh )
    {
        // Only refresh on root, active, chat, and status pages
        if( ( strcmp(update,"/") == 0 ) ||
            ( strcmp(update,"/ActiveList") == 0 ) ||
            ( strstr(update,"/chat") != NULL ) ||
            ( strstr(update,"/Status") != NULL ) )
        {
            snprintf(buffer,4096,"  <meta http-equiv=\"refresh\" content=\"%d\">\n",gOptionMetaRefreshSeconds);
            client_write( httpd,buffer);
        }
    }
    client_write( httpd,"  <title>WebGaim</title>\n");
    client_write( httpd," </head>\n");
    client_write( httpd,"<body>\n");

    //
    //  If we Are In Frames We Behave Differently!!
    //
    if( gOptionWWWFrames )
    {
        if( ( strcmp(update,"/") == 0 ) || ( strcmp(update,"/ActiveList") == 0 ) )
        {
            snprintf(buffer,4096,"CMD: <A HREF=%s target=\"list\">U</A> | <A HREF=/Accounts target=\"conv\">A</A> | <A HREF=/Options target=\"conv\">O</A> | <A HREF=/About target=\"conv\">?</A><HR>\n",update);
        }
        else
        {
            snprintf(buffer,4096,"CMD: <A HREF=%s target=\"conv\">Update</A><HR> \n",update);
        }
    }
    else
    {
        //
        // Not framed; use the "Normal Way"
        //
        snprintf(buffer,4096,"CMD: <A HREF=%s>U</A> | <A HREF=/>H</A> | <A HREF=/Accounts>A</A> | <A HREF=/Options>O</A> | <A HREF=/About>?</A><HR>\n",update);
    }

    client_write( httpd,buffer);
}

/**
 * @brief write end of the httpd stream 
 */
static void client_write_tail( webgaim_client_t * httpd )
{
    client_write( httpd,"</body>\n");
    client_write( httpd,"</html>\n");
    client_write( httpd,"\n\n");
}


static void webgaim_show_buddy(webgaim_client_t * httpd,const char * extra_html,GaimBuddy *buddy )
{
    char buffer[4096];
    char * name = webgaim_encode( buddy->name );

    if( buddy->alias )
    {
        snprintf(buffer,4096,"&nbsp;&nbsp; <A HREF=chat?%s%s %s>%s</A>",time_stamp(),name,extra_html,buddy->alias);
    }
    else if( buddy->server_alias )
    {
        snprintf(buffer,4096,"&nbsp;&nbsp; <A HREF=chat?%s%s %s>%s</A>",time_stamp(),name,extra_html,buddy->server_alias);
    }
    else
    {
        snprintf(buffer,4096,"&nbsp;&nbsp;<A HREF=chat?%s%s %s>%s</A>",time_stamp(),name,extra_html,buddy->name);
    }
    g_free(name);
    client_write(httpd,buffer);


    /* Retrieve and display status message, if one exists */
    if (gOptionStatusMessages)
    {
        GaimStatus *status;
        const char *status_msg = NULL;
        status = gaim_presence_get_active_status(buddy->presence);
        status_msg = gaim_value_get_string(gaim_status_get_attr_value(status, "message"));
        if ((status_msg != NULL) && (*status_msg != '\0')) {
            char *stripped_status_msg = gaim_markup_strip_html(status_msg);
            client_write(httpd," ");
            client_write(httpd,stripped_status_msg);
            g_free(stripped_status_msg);
        }
    }

    /* Idle - lifted largely from gaim/src/gtkblist.c */
    if (gaim_presence_is_idle(buddy->presence))
    {
        time_t idle_secs = gaim_presence_get_idle_time(buddy->presence);
        if (idle_secs > 0) {
            int ihrs, imin;
            time_t t;

            time(&t);
            gaim_debug_info("Webgaim 2","Buddy %s has been idle for %d sec\n", buddy->name, t - idle_secs);
            ihrs = (t - idle_secs) / 3600;
            imin = ((t - idle_secs) / 60) % 60;

            if (ihrs)
                snprintf(buffer,4096," (Idle %dh %02dm)", ihrs, imin);
            else
                snprintf(buffer,4096," (Idle %dm)", imin);
        }
        else
            snprintf(buffer,4096," (Idle)");

        client_write(httpd,buffer);
    } else if (!gaim_presence_is_available(buddy->presence)) {
        client_write(httpd," (Away)");
    }
    client_write(httpd,"<BR>\n");
}


static void webgaim_buddy_list_walk( webgaim_client_t * httpd,const char * extra_html,GtkTreeModel *model,  GtkTreeIter * parent)
{
    char buffer[4096];
    GValue val;
    GaimBlistNode *node;
    do{
        val.g_type = 0;
        gtk_tree_model_get_value (model, parent, NODE_COLUMN, &val);
        node = g_value_get_pointer(&val);
        switch (node->type)
        {
            case GAIM_BLIST_GROUP_NODE:{
                GtkTreeIter child;
                GaimGroup *group = (GaimGroup*) node;
                snprintf(buffer,4096,"<B>%s</B><BR>\n",group->name);
                client_write(httpd,buffer);

                if( gtk_tree_model_iter_children(model,&child,parent) )
                    webgaim_buddy_list_walk( httpd,extra_html,model,&child);
            }break;


            case GAIM_BLIST_CONTACT_NODE:{
                GaimContact *contact = (GaimContact *)node;
                if ( contact->alias )
                {
                    /// not sure yet
                    //snprintf(buffer,4096,"<B>type=%d ( not sure )</B><BR>\n",node->type);
                    //client_write(httpd,buffer);
                }
                else
                {
                    GaimBuddy *buddy = gaim_contact_get_priority_buddy(contact);
                    webgaim_show_buddy( httpd, extra_html, buddy );
                }
            }break;

            case GAIM_BLIST_BUDDY_NODE:{
                GaimBuddy *buddy = (GaimBuddy*) node;
                webgaim_show_buddy( httpd, extra_html, buddy );
            }break;

            default:
                snprintf(buffer,4096,"<B>type=%ds</B><BR>\n",node->type);
                client_write(httpd,buffer);
            break;
        }
    }while( gtk_tree_model_iter_next(model,parent) );
}


/**
 * @brief list all buddies that have active chat sessions and how many unread mesages we have
 * @brief (x) BuddyName [n] :: Where x is #unread messages, and n is a quick access key for mobile phones
 */
static void show_active_chats( webgaim_client_t * httpd, const char * except )
{
    char buffer[4096];
    char extra_html[512];
    unsigned count = 0;
    GaimBlistNode *gnode, *cnode, *bnode;

    strcpy(extra_html, "");
    if( gOptionWWWFrames )
    {
        strncat(extra_html," target=\"conv\" ",511-strlen(extra_html));
    }


    for (gnode = gaim_get_blist()->root; gnode != NULL; gnode = gnode->next)
    {
        if (!GAIM_BLIST_NODE_IS_GROUP(gnode))
            continue;

        for (cnode = gnode->child; cnode != NULL; cnode = cnode->next)
        {
            if (!GAIM_BLIST_NODE_IS_CONTACT(cnode))
                continue;

            for (bnode = cnode->child; bnode != NULL; bnode = bnode->next)
            {
                char * name;
                unsigned unread = 0;
                GaimBuddy *buddy = (GaimBuddy *)bnode;
                webgaim_chat_t * chat = NULL;

                if ( ! GAIM_BUDDY_IS_ONLINE(buddy))
                    continue;

                if( except && ( strcmp( except,buddy->name) == 0 ) )
                    continue;

                chat = chat_find( buddy->name );
                if( chat )
                {
                    unread = chat->unseen;
                }
                if( unread == 0 )
                    continue;

                if( count == 0 ){
                    client_write( httpd,"Active:<BR>");        
                }

                
                name = webgaim_encode( buddy->name );
                if( buddy->alias )
                {
                    
                    snprintf(buffer,4096,"&nbsp;(%d)&nbsp; <A HREF=chat?%s%s accesskey=\"%d\" %s>%s</A> [%d]<BR>\n",unread,time_stamp(),name,count,extra_html,buddy->alias,count);
                }
                else if( buddy->server_alias )
                {
                    snprintf(buffer,4096,"&nbsp;(%d)&nbsp; <A HREF=chat?%s%s accesskey=\"%d\" %s>%s</A> [%d]<BR>\n", unread,time_stamp(),name,count,extra_html,buddy->server_alias,count);
                }
                else
                {
                    snprintf(buffer,4096,"&nbsp;(%d)&nbsp;<A HREF=chat?%s%s accesskey=\"%d\" %s>%s</A> [%d]<BR>\n",unread,time_stamp(),name,count,extra_html,buddy->name,count);
                }
                g_free( name );
                client_write(httpd,buffer);
                count++;
            }
        }
    }
}


static int action_active_list( webgaim_client_t * httpd, const char * notused )
{
    GaimGtkBuddyList *gtkblist = gaim_gtk_blist_get_default_gtk_blist();
    GtkTreeIter iter;
    char extra_html[512];
    gaim_debug_info("WebGaim 2","%s\n",__FUNCTION__);

    client_write_header( httpd,"/ActiveList" );

    show_active_chats( httpd , NULL);

    strcpy(extra_html,"");
    if( gOptionWWWFrames )
    {
        strncat(extra_html," target=\"conv\" ", 511-strlen(extra_html));
    }


    /// Walk through all the buddy lists and display them
    /// Our buddy display should now match whatever gaim chooses :)
    if( gtk_tree_model_get_iter_first(GTK_TREE_MODEL(gtkblist->treemodel),&iter) )
    {
        webgaim_buddy_list_walk(httpd,extra_html,GTK_TREE_MODEL(gtkblist->treemodel),&iter);
    }
    client_write_tail( httpd );
    return 1;
}

/**
 * @brief Given a WWW encoded post string, see if a "checkbox" is set with the given name
 */
static unsigned webgaim_get_param_bool( const char *data, const char * param )
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

static int webgaim_get_param_int( const char *data, const char * param )
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

    gaim_debug_info("WebGaim 2","search=[%s] in=[%s] value=[%d]\n",search,key,value);

    return value;
}


static int action_options( webgaim_client_t * httpd, const char * extra )
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
        webgaim_save_pref_bool( "use_color",          webgaim_get_param_bool( extra,"use_color") );
        webgaim_save_pref_bool( "use_bold_names",     webgaim_get_param_bool( extra,"use_bold_names") );
        webgaim_save_pref_bool( "use_status_messages",webgaim_get_param_bool( extra,"use_status_messages") );
        webgaim_save_pref_bool( "use_www_frames",     webgaim_get_param_bool( extra,"use_www_frames") );
        webgaim_save_pref_bool( "use_meta_refresh",   webgaim_get_param_bool( extra,"use_meta_refresh") );

        value = webgaim_get_param_int( extra,"meta_refresh_seconds");
        if( value >=1 && value <= 3600 )
        {
            webgaim_save_pref_int( "meta_refresh_seconds", value );
        }
        webgaim_load_pref();
        return action_root(httpd,NULL); // return to root page after preferences saved & reloaded
    }

    client_write_header( httpd,"/Options" );
    client_write(httpd,"<B>Webgaim Options:</B><BR>\n");
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


    client_write(httpd,"<BR>\n");
    client_write(httpd,"&nbsp;&nbsp;<input type=submit name=submit value=Save>\n");
    client_write(httpd,"</div>\n");
    client_write(httpd,"</form>\n");

    client_write_tail( httpd );
    return 1;
}



static int action_accounts( webgaim_client_t * httpd, const char * notused )
{
    int accountCount = 0;
    char buffer[4096]; // yuck buffer overflow waiting to happen!!
    GList *account_iter = NULL;
    client_write_header( httpd,"/Accounts" );
    client_write(httpd,"Accounts:<BR>\n");

    for (account_iter = gaim_accounts_get_all() ; account_iter != NULL ; account_iter = account_iter->next) 
    {
        char *encoded_user = NULL;
        const char * user  = gaim_account_get_username( ( (GaimAccount*) account_iter->data ) );
        const char * proto = gaim_account_get_protocol_name( ( (GaimAccount*) account_iter->data ) );
        char display[1024];

        accountCount++;
        snprintf(display,1024,"%s (%s)",user,proto);

        encoded_user = webgaim_encode( user );
        if( encoded_user == NULL )
            return 0;

        if( gaim_account_is_connected( (GaimAccount*) account_iter->data ) )
        {
            gaim_debug_info("WebGaim 2","[%s] connected\n",display);
            snprintf(buffer,4096,"&nbsp;<A HREF=logout?%s&%s target=\"_top\">(logout)</A> %s<BR>\n",
                                 encoded_user,proto,display);

            client_write(httpd,buffer);
        }
        else
        {
            gaim_debug_info("WebGaim 2","[%s] disconnected\n",display);
            snprintf(buffer,4096,"&nbsp;<A HREF=login?%s&%s target=\"_top\">(login)</A> %s<BR>\n",
                            encoded_user,proto,display);
            client_write( httpd,buffer);
        }
        g_free(encoded_user);
    }

    client_write_tail( httpd );
    return 1;
}


/**
 * @brief Display the / ( Home ) page of WebGaim.
 * @brief this mainly displays account status, online and active buddies
 */
static int action_root( webgaim_client_t * httpd, const char * notused )
{
    if( gOptionWWWFrames )
    {
        client_write_http_header( httpd );
        client_write( httpd,"<html>\n");
        client_write( httpd," <head>\n");
        client_write( httpd,"  <title>WebGaim</title>\n");
        client_write( httpd,"  <noframes><body>Your browser can not handle frames.<BR>\n");
        client_write( httpd,"  Please <A HREF=/Options>click here</A> and uncheck Enable Frames</body></noframes>\n");
        client_write( httpd,"   <FRAMESET COLS=\"150, *\" BORDER=0 ID=fs1>");
        client_write( httpd,"    <FRAME SRC=\"/ActiveList\" NORESIZE NAME=\"list\">");
        client_write( httpd,"    <FRAME SRC=\"/Status\" NORESIZE NAME=\"conv\">");
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
static int action_buddy_list( webgaim_client_t * httpd, const char * notused )
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
        webgaim_buddy_list_walk(httpd,extra_html,GTK_TREE_MODEL(gtkblist->treemodel),&iter);
    }
    client_write_tail(httpd);
    return 1;
}

/**
 * @brief Find a GaimAccount object based on a buddy name and account type
 */
static GaimAccount* find_account( const char * from )
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

    for (account_iter = gaim_accounts_get_all() ; account_iter != NULL ; account_iter = account_iter->next) 
    {
        const char * user  = gaim_account_get_username( ( (GaimAccount*) account_iter->data ) );
        const char * proto = gaim_account_get_protocol_name( ( (GaimAccount*) account_iter->data ) );

        if( ( strcmp(user,findName) == 0 ) && ( strcmp(proto,findProto) == 0 ) )
            return ((GaimAccount*) account_iter->data );
    }
    return NULL;
}

/**
 * @brief handle a click on the (Login) account link to login to an account
 */
static int action_login( webgaim_client_t * httpd, const char * extra )
{
    char * decoded = NULL;
    GaimAccount * account;
    gaim_debug_info("WebGaim 2","%s (%s)\n",__FUNCTION__,extra);

    decoded = webgaim_normalize( extra );
    account=find_account(decoded);
    g_free(decoded);
    if( account )
    {    
        gaim_account_set_enabled(account, GAIM_GTK_UI, TRUE);
        gaim_account_connect( account );
    }
/*
    /// FIXME: Gaim needs (a long) time to login...
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
static int action_logout( webgaim_client_t * httpd, const char * extra )
{
    char * decoded = NULL;
    GaimAccount * account;
    gaim_debug_info("WebGaim 2","%s (%s)\n",__FUNCTION__,extra);
    decoded = webgaim_normalize( extra );
    account=find_account(decoded);
    g_free(decoded);
    if( account )
    {
        gaim_account_disconnect( account );
        gaim_account_set_enabled(account, GAIM_GTK_UI, FALSE);
    }
    return action_root(httpd,NULL); // return to root page
}

/**
 * @brief handle a click on a buddy, which opens the "chat" view
 * @brief then handle any new message updates, etc.
 */
static int action_chat( webgaim_client_t * httpd, const char * extra )
{
    char buffer[1024];
    char * name;
    const char * self;
    webgaim_chat_t * chat;

    name = webgaim_normalize( extra );
    chat = chat_find( name );
    self = get_self_name( name ); /// Get the name we will use for ourselves


    snprintf(buffer,1024,"/chat?%s%s",time_stamp(),extra);
    client_write_header( httpd,buffer);

    /// Now our web form for the chat
    client_write(httpd,"<form method=\"get\" action=\"/send?\">\n");
    client_write(httpd,"<div>\n");
    snprintf(buffer,1024,"<input style=\"-wap-input-format: *m\" name=\"%s\" size=64/>\n",name);
    client_write(httpd,buffer);
    client_write(httpd,"<input type=\"submit\" value=\"send\"/>\n");
    client_write(httpd,"</div>\n");
    client_write(httpd,"</form>\n");

    /// Show any active chats from other users. IE we're talking to bob, but in the meantime sally IM-ed us
    /// show sally has an unread message for us. ( Just like the Active: buddies section on the root page )
    show_active_chats( httpd, name );
    g_free( name );

    client_write(httpd,"<HR>");
    if( chat )
    {
        webgaim_conv_t * conv = NULL;
        for(conv=chat->head;conv!=NULL;conv=conv->prev )
        {
            struct tm *tm = localtime( &conv->tm );
            snprintf(buffer,1024,"(%d:%02d:%02d)&nbsp;",tm->tm_hour,tm->tm_min,tm->tm_sec);
            client_write(httpd,buffer);

            /// LATEST on top
            if( conv->whom == CONV_SOURCE_SELF )
            {
                if( gOptionUseColor )
                    client_write(httpd,"<FONT COLOR=#DD5555>");

                if( gOptionBoldNames )
                    client_write(httpd,"<B>");

                client_write(httpd,self);

                if( gOptionBoldNames )
                    client_write(httpd,"</B>");

                if( gOptionUseColor )
                    client_write(httpd,"</FONT>");
            }
            else
            {
                if( gOptionUseColor )
                    client_write(httpd,"<FONT COLOR=#5555DD>");

                if( gOptionBoldNames )
                    client_write(httpd,"<B>");

                client_write(httpd,chat->buddy);

                if( gOptionBoldNames )
                    client_write(httpd,"</B>");

                if( gOptionUseColor )
                    client_write(httpd,"</FONT>");

            }
            client_write(httpd,":&nbsp;"); /// Add a space, otherwise things blend in to much
            client_write(httpd,gaim_markup_strip_html(conv->message));
            client_write(httpd,"<BR>\n");
        }
        chat->unseen=0;

        chat_trim( chat );
    }

    snprintf(buffer,1024,"(<A HREF=/history?%s%s>history</A>)",time_stamp(),extra);
    client_write( httpd,buffer);
    client_write_tail( httpd );
    return 1;
}

/**
 * @brief Execute the http-post click
 */
static int action_send( webgaim_client_t * httpd, const char * extra )
{
    const char * encoded_buddy = NULL;
    char * message = NULL;
    gaim_debug_info("Webgaim 2","Extra[%s]\n",extra);

    /// FIRST we need to see if we can find a conversation window with this chat already in there
    encoded_buddy = extra;
    message= strstr(encoded_buddy,"=");
    if( message )
    {
        char * buddy;
        char * normal;
        GaimConversation *c;
        GaimBuddy *b;
        *message='\0';
        message++;
        
        buddy=webgaim_normalize(encoded_buddy);

        b=find_buddy(buddy);
        if( !b )
        {
            g_free(buddy);
            return 1;
        }

        normal = webgaim_normalize( message );

        
        c = gaim_find_conversation_with_account(GAIM_CONV_TYPE_IM, b->name, b->account);
        if( !c )
        {
             c = gaim_conversation_new(GAIM_CONV_TYPE_IM, b->account, b->name);
        }

        if( c )
        {
            gaim_conv_im_send( GAIM_CONV_IM(c),normal);
        //    chat_send(b->name,normal);    /// Not needed as we get a send bounce
        }

        g_free(buddy);
        g_free(normal);

        action_chat(httpd,encoded_buddy);
    }
    else
    {
        return action_root(httpd,NULL);
    }
    
    
    return 1;
}

/**
 * @brief Print the GPL license and a web link to the Webgaim website
 */
static int action_about( webgaim_client_t * httpd, const char * extra )
{
    gaim_debug_info("WebGaim 2","%s\n",__FUNCTION__);
    client_write_header( httpd,"/About" );
    client_write(httpd,"<FONT size=+1>Copyright (C) 2006 Sebastian Sobolewski<BR>\n");
    client_write(httpd,"Additional Contributors:<BR>\n");
    client_write(httpd,"David Morse<BR>\n");
    client_write(httpd,"Version ");
    client_write(httpd,WEBGAIM_VERSION);
    client_write(httpd,"</FONT><BR>\n");
    client_write(httpd,"<A HREF=http://www.thirdmartini.com/index.php/WebGaim_2.x>WebGaim Home Page</A><HR>\n");
    client_write(httpd,"<PRE>");
    client_write(httpd,license);
    client_write(httpd,"</PRE>");
    client_write_tail( httpd );
    return 1;
}

static char * webgaim_log_read_reverse(GaimLog *log, GaimLogReadFlags *flags)
{
    char * data = gaim_log_read( log, flags );
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

static int action_history( webgaim_client_t * httpd, const char * extra )
{
    char buffer[1024];
    GaimBuddy *buddy = NULL;
    char * name;

    name = webgaim_normalize( extra );
    buddy = find_buddy( name );


    snprintf(buffer,1024,"/history?%s%s",time_stamp(),extra);
    client_write_header( httpd,buffer);


    if( buddy && buddy->account )
    {
        GList *logs = gaim_log_get_logs(GAIM_LOG_IM, name, buddy->account );

        if( logs ) 
        {
            logs = g_list_concat( logs,  gaim_log_get_logs(GAIM_LOG_CHAT, name, buddy->account ) );
        }
        else
        {
            logs = gaim_log_get_logs(GAIM_LOG_CHAT, name, buddy->account );
        }
        
        if( logs )
        {
            GList *log;

            for (log = logs; log != NULL; log = log->next) 
            {
                GaimLog * gaimLog = log->data;
                char *data = webgaim_log_read_reverse(gaimLog, NULL);

                if( data )
                {
                    webgaim_strftime(buffer, 1024,"%c" , localtime(&gaimLog->time) );
                    client_write(httpd, buffer);
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
 * @brief This is the Initial Page when opening WebGaim
 */
static int action_status( webgaim_client_t * httpd, const char * unused )
{
    char buffer[1024];

    snprintf(buffer,1024,"/Status?%s",time_stamp());
    client_write_header( httpd,buffer);

    client_write(httpd,"WebGaim Version: ");
    client_write(httpd,WEBGAIM_VERSION);
    client_write(httpd,"<BR>\n");
    
    //More stuff here like, e-mail notices etc
    //client_write(httpd,"<B>New E-Mail:</B>");
    //client_write(httpd,"Updates");
    strcpy(buffer,"");
    show_active_chats( httpd, buffer );

    client_write(httpd,"<BR> Status Information Goes Here<BR>");
    client_write_tail( httpd );
    return 1;
}

static int action_rss( webgaim_client_t * httpd, const char * unused )
{
    rss_item_t *item = NULL;
    char buffer[1024];

    client_write( httpd,"");
    client_write( httpd,"HTTP/1.1 200 OK\n");
    client_write_date(httpd);
    client_write( httpd,"Server: WebGaim\n");
    client_write( httpd,"Connection: close\n");
    client_write( httpd,"Content-type: application/rss+xml\n\n");
//    client_write( httpd,"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");

    client_write(httpd,"<?xml version=\"1.0\"?>\n");
    client_write(httpd,"<rss version=\"2.0\">\n");
    client_write(httpd," <channel>\n");
    client_write(httpd,"  <title>WebGaim Status</title>\n");
    client_write(httpd,"  <link>http://sourceforge.net/projects/webgaim/</link>\n");
    client_write(httpd,"  <description>Webgaim Updates</description>\n");

    for( item = rssHead.next; item != NULL; item = item->next )
    {
        if( ( item->conv != NULL ) && ( item->conv->whom != CONV_SOURCE_SELF ))
        {
            client_write(httpd,"  <item>\n");
            snprintf(buffer,sizeof(buffer),"   <guid isPermaLink=\"false\">%d-%d</guid>\n",(int)item->conv->tm,item->conv->id);
            client_write(httpd,buffer);
            client_write(httpd,"   <title>");
            snprintf(buffer,sizeof(buffer),"New Message From %s",item->conv->chat->buddy);
            client_write(httpd,buffer);
            client_write(httpd,"</title>\n");
            //client_write(httpd,"   <link>http://localhost:5888/</link>\n");
            //client_write(httpd,"<guid isPermaLink="true">http://www.engadget.com/2006/07/11/sling-opens-up-mac-slingplayer-beta/</guid>
            client_write(httpd,"   <description>");
            snprintf(buffer,sizeof(buffer),"%s",gaim_markup_strip_html(item->conv->message));
            client_write(httpd,buffer);
            client_write(httpd,"</description>\n");
            client_write(httpd,"  </item>\n");
        }
    }
    client_write(httpd," </channel>\n</rss>\n");
    client_write( httpd,"\n\n");
    return 1;
}



/**
 * @brief Action dispatcher  structure, matches the http-post request to
 * @brief the appropriate action_xxx call for handling
 */
static webgaim_parse_t webgaim_actions[] = {
    { "/", action_root },
    { "/ActiveList", action_active_list },
    { "/BuddyList", action_buddy_list },
    { "/login",action_login },
    { "/logout",action_logout },
    { "/chat",action_chat },
    { "/send",action_send },
    { "/Accounts",action_accounts },
    { "/Options",action_options },
    { "/About",action_about },
    { "/history",action_history },
    { "/Status",action_status },
    { "/status.rss",action_rss }
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
static int client_auth_reject( webgaim_client_t * client )
{
    client_write(client,"HTTP/1.1 401 Authorization Required\n");
    /// FIXME: stamp the time and server name correctly
    client_write_date(client);
    client_write(client,"Server: webgaim 2.0.0\n");;
    client_write(client,"WWW-Authenticate: Basic realm=\"Login\"\n");
    client_write(client,"Connection: close\n");
    client_write(client,"Content-type: text/html\n\n\n\n");
    return 1;
}

/**
 * @brief parse the URL request from the client webbrowser and dispatch it to
 * @brief the appropriate action_xxx call
 */
static int client_parse_and_dispatch(webgaim_client_t *httpd, char * buffer, const int len )
{
    char * purl = NULL;
    

    /// Id auth is enabled check the response
    if( httpd->webgaim->auth )
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


        if( strcmp(httpd->webgaim->auth,auth) != 0 )
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
            for(action=0; action< ( sizeof(webgaim_actions)/sizeof(webgaim_parse_t) ); action++ )
            {
            gaim_debug_info("WebGaim 2","Actioon Compare [%s] <--> [%s]\n",purl,webgaim_actions[action].action);
                if( strcmp(webgaim_actions[action].action,purl) == 0 )
                {
                    return webgaim_actions[action].callback( httpd, data );
                }
            }
            /// nothing on the first pass check to see if we have extra info

            gaim_debug_info("WebGaim 2","No action for [%s]\n",purl);
        }
    }
    return 0;
}

/**
 * @brief callback from gaim if the client browser wrote to or closed our socket
 */
static void client_request_cb(gpointer data, gint sock, GaimInputCondition cond)
{
    webgaim_client_t *httpd = ( webgaim_client_t * ) data;
    int len;
    char buffer[4096]; 

    ///FIXME: normally we would read the data on this socket, but for now we just close it and 
    /// release our watcher.
    gaim_debug_info("WebGaim 2","http_request_cb ok\n");
    if( sock != httpd->fd )
    {
        gaim_debug_info("WebGaim 2","http_request_cb yuck bad corruption\n");
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
        gaim_debug_info("WebGaim 2","http_request_cb yuck bad corruption\n");
        close( httpd->fd );
        g_free(httpd);
        return;
    }

    if( len == 0 )
    {
        /// client browser closed the socket
        /// cleanup
        gaim_debug_info("WebGaim 2","http_request_cb endpoint closed connection\n");
        gaim_input_remove(httpd->watcher );
        close( httpd->fd );
        g_free(httpd);
        return;
    }
    buffer[len] = '\0';

    /// Parse our request
    if( ! client_parse_and_dispatch( httpd, buffer,len ) )
    {
        gaim_debug_info("WebGaim 2","unknow request\n");
        client_write_header( httpd,"/" );
        client_write(httpd,"Borked: WebGaim was handed a URL it does not know how to handle. ( Closing Session )");
        client_write_tail( httpd );
        gaim_input_remove(httpd->watcher );
        close( httpd->fd );
        g_free(httpd);
        return;
    }

    /// Always close the endpoint for now
    /// we may want to leave the socket open for more stuff later
    gaim_input_remove(httpd->watcher );
    close( httpd->fd );
    g_free(httpd);
    return;
}

/**
 * @brief gaim indicated we got a hit to our listen socket ( server socket )
 * @brief we need to accept the client socket and start up a httpd client listener
 */
static void webgaim_connect_request_cb(gpointer data, gint sock, GaimInputCondition cond)
{
    webgaim_data_t * webgaim = ( webgaim_data_t * ) data;
    webgaim_client_t *httpd;
    int acceptfd;
    // FIXME: we will croak here right now
    if( sock == -1 )
        return;

    acceptfd = accept(sock,NULL,0 );
    if( acceptfd == -1 )
        return;

    httpd = g_malloc(sizeof(webgaim_client_t));
    if( !httpd )
    {
        close(acceptfd);
        return;
    }

    /// configure the client session object and tell gaim to listen for AVAILABLE READ data on it
    httpd->webgaim = webgaim;
    httpd->fd = acceptfd;
    httpd->watcher = gaim_input_add( httpd->fd,GAIM_INPUT_READ, client_request_cb, httpd );
}

/**
 * @brief callback for GAIM 2.0+ when a listen socket is created
 */
static void webgaim_listen_cb(int fd, gpointer data )
{
    webgaim_data_t * webgaim = ( webgaim_data_t * ) data;

    if( fd >= 0 )
    {
        webgaim->iListenWatcher = gaim_input_add(fd, GAIM_INPUT_READ, webgaim_connect_request_cb, data );
        webgaim->fdServerSocket = fd;
    }    
    else
    {
        webgaim->usListenPort = 0;
    }
}

/**
 * @brief callback when a gaim IM message was received
 */
static void received_im_msg_cb(GaimAccount *account, char *sender, char *buffer,
                                   GaimConversation *conv, int flags, void *data)
{

    gaim_debug_info("signals test", "received-im-msg (%s, %s, %s, %s, %d)\n",
                                        gaim_account_get_username(account), sender, buffer,
                                        (conv != NULL) ? gaim_conversation_get_name(conv) : "(null)", flags);

    /// Add the message to the chat
    chat_receive( sender, buffer );
}

/**
 * @brief callback when a gaim IM message was sent
 */
static void sent_im_msg_cb(GaimAccount *account, const char *recipient, const char *buffer, void *data)
{
    gaim_debug_info("signals test", "sent-im-msg (%s, %s, %s)\n",
                                       gaim_account_get_username(account), recipient, buffer);

    chat_send( recipient, buffer );
}



/**
 * @brief code to start/restart our httpd mini server
 */
static void httpd_restart(webgaim_data_t * webgaim)
{
    unsigned usListenPort = 0;

    free( webgaim->auth );
    webgaim->auth = NULL;

    webgaim_load_pref();

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
                webgaim->auth = (char *)base64_encode((const unsigned char*)temp,strlen(temp),&len);
                gaim_debug_info("WebGaim 2","Load::Authorization Key set to [%s]\n",webgaim->auth);
                free( temp );
            }
        }
    }

    if( gOptionPort )
    {
        if( sscanf(gOptionPort,"%d",&usListenPort) != 1 )
        {
            usListenPort = webgaim->usListenPort;
        }
    }

    if( usListenPort != webgaim->usListenPort )
    {
        if( webgaim->fdServerSocket >= 0 )
        {
            gaim_input_remove( webgaim->iListenWatcher );
            close( webgaim->fdServerSocket );
            webgaim->fdServerSocket = -1;
            /// Session connections will remain open until
            /// the client endpoint closes it ( this is ok )
            /// For paranoia reasons we may want to force drop them if we choose
        }
        webgaim->usListenPort = usListenPort;


        if( !gaim_network_listen( usListenPort,SOCK_STREAM ,webgaim_listen_cb, (gpointer)webgaim ) )
        {
            gaim_debug(GAIM_DEBUG_INFO, "WebGaim 2", "Load::WebGaim could not listen port (%d).\n",usListenPort);
            webgaim->usListenPort = 0;
            return;
        }
    }
    else
    {
        gaim_debug(GAIM_DEBUG_INFO, "WebGaim 2", "Load::Port change not required ( using=%d )\n", usListenPort );
    }

}

/** The wegbaim global instance */
static webgaim_data_t * webgaim = NULL; 


static gboolean plugin_load(GaimPlugin *plugin)
{

    webgaim = g_malloc( sizeof( webgaim_data_t ) );
    if( !webgaim )
    {
        gaim_debug(GAIM_DEBUG_INFO, "WebGaim 2", "WebGaim could not allocate data\n");
        return FALSE;
    }
    memset( webgaim,0, sizeof(webgaim_data_t) );
    webgaim->fdServerSocket = -1;

    gaim_network_init();
    
    httpd_restart(webgaim);

    gaim_signal_connect(gaim_conversations_get_handle(), "received-im-msg",plugin, GAIM_CALLBACK(received_im_msg_cb), NULL);
    gaim_signal_connect(gaim_conversations_get_handle(), "sent-im-msg",plugin, GAIM_CALLBACK(sent_im_msg_cb), NULL);

    gaim_debug(GAIM_DEBUG_INFO, "WebGaim 2", "WebGaim plugin loaded.\n");
    return TRUE;
}

static gboolean plugin_unload(GaimPlugin *plugin)
{
    gaim_debug(GAIM_DEBUG_INFO, "WebGaim 2", "WebGaim plugin unloaded.\n");
    return TRUE;
}

static void apply_configuration()
{
    httpd_restart( webgaim );
}

static void type_toggle_cb(GtkWidget *widget, gpointer data)
{
    if( strcmp(data,"use_color") == 0 )
    {
        gOptionUseColor = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webgaim_save_pref_bool( (char*)data, gOptionUseColor );
    }
    else if( strcmp(data,"use_bold_names") == 0  )
    {
        gOptionBoldNames = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webgaim_save_pref_bool( (char*)data, gOptionBoldNames );
    }
    else if( strcmp(data,"use_status_messages") == 0  )
    {
        gOptionStatusMessages = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webgaim_save_pref_bool( (char*)data, gOptionStatusMessages );
    }
    else if( strcmp(data,"use_www_frames") == 0  )
    {
        gOptionWWWFrames = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webgaim_save_pref_bool( (char*)data, gOptionWWWFrames );
    }
    else if( strcmp(data,"use_meta_refresh") == 0  )
    {
        gOptionMetaRefresh = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
        webgaim_save_pref_bool( (char*)data, gOptionMetaRefresh );
    }
}

static gboolean options_entry_cb(GtkWidget *widget, GdkEventFocus *evt, gpointer data)
{
    if (data == NULL)
        return FALSE;

    if (!strcmp(data, "server_port")) {
        gaim_prefs_set_string("/plugins/webgaim/server_port", gtk_entry_get_text(GTK_ENTRY(widget)));
    }
    else if (!strcmp(data, "server_user")) {
        gaim_prefs_set_string("/plugins/webgaim/server_user", gtk_entry_get_text(GTK_ENTRY(widget)));
    }
    else if (!strcmp(data, "server_password")) {
        gaim_prefs_set_string("/plugins/webgaim/server_password", gtk_entry_get_text(GTK_ENTRY(widget)));
    }
    else if (!strcmp(data, "meta_refresh_seconds")) {
        int sec = 0;
        const char * str = gtk_entry_get_text(GTK_ENTRY(widget));
        sscanf( str,"%d",&sec);
        webgaim_save_pref_int( (char*)data, sec );
    }
    else
    {
        gaim_debug_info("WebGaim 2","UNHANDLED [%s]\n",(char*)data);
    }

    apply_configuration();
    return FALSE;
}


static GtkWidget * get_config_frame(GaimPlugin *plugin)
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
    gtk_entry_set_text(GTK_ENTRY(entry), gaim_prefs_get_string("/plugins/webgaim/server_port"));
    g_signal_connect(G_OBJECT(entry), "focus-out-event",G_CALLBACK(options_entry_cb), "server_port");

    frame = gaim_gtk_make_frame(ret, _("Username:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 32);
    gtk_entry_set_text(GTK_ENTRY(entry), gaim_prefs_get_string("/plugins/webgaim/server_user"));
    g_signal_connect(G_OBJECT(entry), "focus-out-event",G_CALLBACK(options_entry_cb), "server_user");


    frame = gaim_gtk_make_frame(ret, _("Password:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 32);
    gtk_entry_set_text(GTK_ENTRY(entry),gaim_prefs_get_string("/plugins/webgaim/server_password"));
    g_signal_connect(G_OBJECT(entry), "focus-out-event", G_CALLBACK(options_entry_cb), "server_password");

    frame = gaim_gtk_make_frame(ret, _("Extras:"));
    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

    toggle = gtk_check_button_new_with_mnemonic(_("Color coded history"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     gaim_prefs_get_bool("/plugins/webgaim/use_color"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_color");

    toggle = gtk_check_button_new_with_mnemonic(_("Bold Buddy Names"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     gaim_prefs_get_bool("/plugins/webgaim/use_bold_names"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_bold_names");

    toggle = gtk_check_button_new_with_mnemonic(_("Buddy Status Messages"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     gaim_prefs_get_bool("/plugins/webgaim/use_status_messages"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_status_messages");

    toggle = gtk_check_button_new_with_mnemonic(_("Enable Frames"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     gaim_prefs_get_bool("/plugins/webgaim/use_www_frames"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_www_frames");

    toggle = gtk_check_button_new_with_mnemonic(_("Auto-refresh buddy list/status"));
    gtk_box_pack_start(GTK_BOX(vbox), toggle, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),
                                     gaim_prefs_get_bool("/plugins/webgaim/use_meta_refresh"));
    g_signal_connect(G_OBJECT(toggle), "toggled",
                         G_CALLBACK(type_toggle_cb), "use_meta_refresh");

	sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
	spin_button = gaim_gtk_prefs_labeled_spin_button(vbox, _("_Seconds:"),
			"/plugins/webgaim/meta_refresh_seconds", 1, 3600, sg);
	if (!gaim_prefs_get_bool("/plugins/webgaim/use_meta_refresh"))
		gtk_widget_set_sensitive(GTK_WIDGET(spin_button), FALSE);
	g_signal_connect(G_OBJECT(toggle), "clicked",
					 G_CALLBACK(gaim_gtk_toggle_sensitive), spin_button);


    gtk_widget_show_all(ret);
    return ret;
}

static GaimGtkPluginUiInfo ui_info =
{
    get_config_frame,
    0 /* page_num (Reserved) */
};


static GaimPluginInfo info =
{
    GAIM_PLUGIN_MAGIC,
    GAIM_MAJOR_VERSION,
    GAIM_MINOR_VERSION,
    GAIM_PLUGIN_STANDARD,                             /**< type           */
    GAIM_GTK_PLUGIN_TYPE,                             /**< ui_requirement */
    0,                                                /**< flags          */
    NULL,                                             /**< dependencies   */
    GAIM_PRIORITY_DEFAULT,                            /**< priority       */

    "gaim-webgaim",                                   /**< id             */
    N_("WebGaim"),                                    /**< name           */
    N_(WEBGAIM_VERSION),                              /**< version        */
                                                      /**  summary        */
    N_("Gaim access over HTTP"),
                                                      /**  description    */
    N_("Allows access to all Gaim features over a simple http interface such as those used in mobile phones."),
    "Sebastian Sobolewski <spsobole@yahoo.com>",        /**< author         */
    N_("http://www.thirdmartini.com/index.php/WebGaim_2.x"), /**< homepage       */

    plugin_load,                                      /**< load           */
    plugin_unload,                                    /**< unload         */
    NULL,                                             /**< destroy        */

    &ui_info,                                         /**< ui_info        */
    NULL,                                             /**< extra_info     */
    NULL,
    NULL
};

static void init_plugin(GaimPlugin *plugin)
{
    gaim_debug(GAIM_DEBUG_INFO, "WebGaim 2", "WebGaim Init.\n");
    gaim_prefs_add_none("/plugins/webgaim");
    gaim_prefs_add_string("/plugins/webgaim/server_port", "8888");
    gaim_prefs_add_string("/plugins/webgaim/server_user", "");
    gaim_prefs_add_string("/plugins/webgaim/server_password", "");
    gaim_prefs_add_bool("/plugins/webgaim/use_color", 1);
    gaim_prefs_add_bool("/plugins/webgaim/use_bold_names", 1);
    gaim_prefs_add_bool("/plugins/webgaim/use_status_messages", 1);
    gaim_prefs_add_bool("/plugins/webgaim/use_www_frames", 0);
    gaim_prefs_add_bool("/plugins/webgaim/use_meta_refresh", 0);
    gaim_prefs_add_int("/plugins/webgaim/meta_refresh_seconds", 180);
}

GAIM_INIT_PLUGIN(WebGaim, init_plugin, info)
