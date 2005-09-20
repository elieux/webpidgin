HTTP/1.1 200 OK
Date: Sun, 23 Nov 2014 20:54:06 GMT
Server: Apache/2.2.8 (Ubuntu) DAV/2 SVN/1.4.6 PHP/5.2.4-2ubuntu5.1 with Suhosin-Patch
Last-Modified: Tue, 20 Sep 2005 13:38:50 GMT
ETag: "4a17f-66f1-401342e168e80"
Accept-Ranges: bytes
Content-Length: 26353
Keep-Alive: timeout=15, max=100
Connection: Keep-Alive
Content-Type: text/x-perl

#############################################################3
#
# Expose A WEB Server for conversations So that a lightweight
#    WAP device can use the webserver as a proxy to conversations
#
#   Copyright (C) 2004 Sebastian Sobolewski ( spsobole@yahoo.com )
#
###############################################################
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 2 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#   General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with this program; if not, write to the Free
#   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
#   MA 02111-1307, USA.
#
#######################################################################
use Gaim;
use threads;
use threads::shared;

#use strict;
#use warnings;

####################################################
# Options Block:
#   These are some basic confiuration options available 
#   at this time.
#
my $opt_color_me     = "#DD5555";  # color of messages by you
my $opt_color_them   = "#5555DD";  # color of the other sides messages
my $opt_time_stamp   = "true";     # Time stamp messages
my $opt_chat_display = "top";      # display messages with newest on top ( bottom, displays newest on bottom )
my $opt_chat_prune   = "25";       # Number of lines of conversation to retain
my $opt_port         = "8111";     # The port number for the built in webserver 

#
# opt_auth is the http simple authentication string that a browser will send 
# you  This needs to be implemented as a  username/password decode in the future.
#

my $opt_auth         = "";

#
# Load all of the above $opt_xxxxx parameters from a config file instead
#

my $home = $ENV{"HOME"};
my $config =  "$home/.gaim/webgaim.cfg";

if( -e "$config" )
{
	require "$config";
}
else
{
	printf STDOUT "NO $config\n";
}


#
# END OPTIONS
####################################################
my $WEBGAIM = "webgaim";

my %g_conv = ();

my %buddies_all     : shared = ();

my @buddies_online : shared = ();
my @g_accounts     : shared = ();
my @g_chats         : shared = ();

my %g_chat_lines    : shared = ();
my %g_chat_contents : shared = ();

my $who_am_i : shared = "Me";    

my @todo : shared = ();          # List of work items to be done in the timer callback 

#
# Plugin info structure
#
#
%PLUGIN_INFO = (
    perl_api_version => 2,
    name             => "WebGaim",
    version          => "0.4",
    summary          => "Web intrerface to gaim.",
    description      => "Alows your active gaim session to be also visible over the internet. Thus accessible with WEB capable devices such as smart phones",
    author           => "Sebastian Sobolewski <spsobole\@yahoo.com>",
    url              => "N/A",
    load             => "plugin_load",
    unload           => "plugin_unload"
);

#
# Plugin Init code ( nothing special here )
#
sub plugin_init {
    return %PLUGIN_INFO;
}

sub plugin_unload {
    my $plugin = shift;
	close( S );
	sleep(1);
}

#
# Plugin load
#
sub plugin_load {
	$plugin = shift;
	#my $data   = "";
	
	Gaim::signal_connect(Gaim::Connections::handle, "signed-on",
                         $plugin, \&signed_on_cb, \%data);
	Gaim::signal_connect(Gaim::Connections::handle, "signed-off",
                         $plugin, \&signed_off_cb, \%data);
						 
	
	Gaim::signal_connect(Gaim::Conversations::handle, "received-im-msg",
                         $plugin, \&received_msg_cb, \%data);

	Gaim::signal_connect(Gaim::BuddyList::handle, "buddy-signed-on", $plugin, \&buddy_signed_on_cb, \%data);
	Gaim::signal_connect(Gaim::BuddyList::handle, "buddy-signed-off", $plugin, \&buddy_signed_off_cb, \%data);
	
	$thr = threads->new(\&gaim_webserver);
	Gaim::timeout_add($plugin, 1, \&timeout_cb, NULL );
	&update_account_status();
	&update_buddy_list();
}


#
# When a buddy signs on we need to add them to the active buddy list 
#
sub buddy_signed_on_cb{
	my ($buddy,$data) = @_;
	my $found = "0";
	my $name = Gaim::BuddyList::Buddy::get_name($buddy);
	
	Gaim::debug_info($WEBGAIM,"buddy_signed_on_cb [$name]<==>[@buddies_online]\n");
	
	lock( @buddies_online );
	@buddies_prev = @buddies_online;
	while( @buddies_prev )
	{
		$name_prev = shift( @buddies_prev );
		Gaim::debug_info($WEBGAIM,"buddy_signed_on_cb [::$name_prev]\n");		
		if( $name_prev eq "$name" )
		{
			$found = "1";
		}
	}
	
	if( $found eq "0" )
	{
		push(@buddies_online,$name);
	}
	
	Gaim::debug_info($WEBGAIM,"buddy_signed_on_cb [$found]\n");
	return undef;
}

#
# If he signed off remove them from list
#
sub buddy_signed_off_cb{
	my ($buddy,$data) = @_;
	$name = Gaim::BuddyList::Buddy::get_name($buddy);
	
	lock( @buddies_online );
	
	@buddies_prev = @buddies_online;
	@buddies_online=();
	while( @buddies_prev )
	{
		$name_prev = shift( @buddies_prev );
		if( $name_prev ne "$name" )
		{
			push(@buddies_online,$name_prev);
		}
	}
	Gaim::debug_info($WEBGAIM,"buddy_signed_OFF_cb");
	return undef;
}

#
# WHen we receive a message we need to add it into the users conversation 
#  list
#
sub received_msg_cb{
	my ($gc, $sender,$msg,$flags) = @_;

	Gaim::debug_info($WEBGAIM,"received_msg_cb:: $sender:$msg:$flags\n");
	
	add_chat_msg( $sender, $sender,$msg );
	
	Gaim::debug_info($WEBGAIM,"received_msg_cb:: DONE\n");
	return undef;
}

#
# All messaging and updates to and from the main gaim thread are done via
#  locked lists.  
#    The todo list tells us what we need to do in the gaim context
#    We do this this way fopr now as there are some nasty rentrancy problems
#    in calling gaim functions from the web thread.
#
sub timeout_cb {
	update_conversations();

	{				 
		lock(@todo);
		while( @todo )
		{
			$work = shift(@todo);
			Gaim::debug_info("webgaim","work = [$work]\n");			
			($action,$rest)=split(/:/,$work,2);
			
			if( $action eq "logout" )
			{
				&logout( $rest );
			}
			elsif( $action eq "login" )
			{
				&login( $rest );
			}
			elsif( $action eq "send" )
			{
				&send( $rest );
			}
			elsif( $action eq "start-chat" )
			{
				&start_chat_simple( $rest );
			}
			
		}
	}
	# Add a new timeout
	Gaim::timeout_add($plugin, 1, \&timeout_cb, "todo");
}

#
# The login function gets called when a user clicked on the 
#  icon to login into a specific account, we need to take the action of 
#  completing the login here
sub login{
	my ($my_protocol,$crap) = @_;
	
	Gaim::debug_info($WEBGAIM,"login [$my_protocol,$crap]\n");
	
	@accounts = Gaim::accounts();
	while( @accounts )
	{
		$acc = shift( @accounts );
		$protocol = Gaim::Account::get_protocol_id( $acc );
		if( $my_protocol eq $protocol )
		{
			$ret = Gaim::Account::connect( $acc );
		}
	}
}

#
# see !login
#
#
sub logout{
	my $my_protocol = shift;
	Gaim::debug_info($WEBGAIM,"logout [$my_protocol]\n");
	@accounts = Gaim::accounts();	
	while( @accounts )
	{
		$acc = shift( @accounts );
		$protocol = Gaim::Account::get_protocol_id( $acc );
		if( $my_protocol eq $protocol )
		{
			Gaim::Account::disconnect( $acc );
		}
	}
}

#
# Post a message into a conversation
#
sub send_borked{
	my $line = shift;
	($to,$msg) = split(/=/,$line,2);
	Gaim::debug_info($WEBGAIM,"ENTER send [$to]:[$msg]\n");
	$to = buddy_real( $to );
		
	my @gaim_conversations = Gaim::conversations();
	while( @gaim_conversations )
	{
		my $gaim_conv = shift( @gaim_conversations );
		my $name = Gaim::Conversation::get_name( $gaim_conv );
		
		if( $name eq "$to" )
		{
			if( $gaim_conv->is_im() )
			{
				Gaim::Conversation::IM::send( $gaim_conv, $msg  );
			}
			elsif( $gaim_conv->is_chat() )
			{
				Gaim::Conversation::Chat::send( $gaim_conv, $msg  );
			}
			#Gaim::debug_info($WEBGAIM,"sent [$to]:[$msg]\n");
		}
		Gaim::debug_info($WEBGAIM,"sent [$to]:[$name]\n");
	}
	Gaim::debug_info($WEBGAIM,"EXIT: sent\n");
	return undef;
}


sub send{
	my $line = shift;
	($to,$msg) = split(/=/,$line,2);
	$to = buddy_real( $to );
	Gaim::debug_info($WEBGAIM,"ENTER send [$to]:[$msg]\n");
	
	my @gaim_ims = Gaim::ims();
	while( @gaim_ims )
	{
		my $gaim_im = shift( @gaim_ims );
		my $name = Gaim::Conversation::get_name( Gaim::Conversation::IM::get_conversation( $gaim_im ) );
		
		if( $name eq "$to" )
		{
			Gaim::Conversation::IM::send( $gaim_im, $msg  );
			#Gaim::debug_info($WEBGAIM,"sent [$to]:[$msg]\n");
		}
		Gaim::debug_info($WEBGAIM,"sent [$to]:[$name]\n");
	}
	Gaim::debug_info($WEBGAIM,"EXIT: sent\n");
	return undef;
}


#
# Add a chat/im message from another user into our list of messages
#
sub add_chat_msg
{
	my $sender = shift;
	my $as = shift;
	my $msg = shift;
	
	lock( %g_chat_contents );
	lock( %g_chat_lines );
	if( exists($g_chat_contents{$sender} ) )
	{
		$g_chat_lines{$sender}++;
	}
	else
	{
		$g_chat_lines{$sender} = "1";
	}
	
	$linesofar = $g_chat_contents{$sender};
	
	if( $opt_time_stamp eq "true" )
	{
		($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime(time);
		#Gaim::debug_info($WEBGAIM,"sent [$sec]:[$min]:[$hour]:[$mday]:[$mon]:[$year]\n");
		
		#Gaim::debug_info($WEBGAIM,"sent [$sec]:[$min]:[$hour]:[$mday]:[$mon]:[$year]\n");
		$timestamp = "($hour:$min:$sec)";
		#Gaim::debug_info($WEBGAIM,"sent [$timestamp]\n");
		$linesofar = "$linesofar<:::>$timestamp $as<::>$msg";
	}
	else
	{
		$linesofar = "$linesofar<:::>$as<::>$msg";	
	}
	$g_chat_contents{$sender} = $linesofar;
}

#
# If we are initiating the conversation we need to tell gaim to 
# create a new conversation window or instance  
#
sub start_chat_simple{
	my $to = shift;
	my $found = 0;
	$to = buddy_real( $to );
	
		
	my @groups = Gaim::BuddyList::groups();
	while( @groups )
	{
		$group = shift( @groups );
		$group_name = Gaim::BuddyList::Group::get_name($group);
		
		Gaim::debug_info($WEBGAIM,"search:[$to]::group:$group_name\n");
			
		@buddies = Gaim::BuddyList::Group::buddies( $group );
		while( @buddies )
		{
			my $buddy = shift( @buddies );
			my $buddy_name = Gaim::BuddyList::Buddy::get_name($buddy);
			
			Gaim::debug_info($WEBGAIM,"search:[$to]::group:$group_name::$buddy_name\n");
			if( $buddy_name eq $to )
			{
				$found_buddy = $buddy;
			}
		}
	}
		
	if( $found_buddy )
	{
		$buddy_name = Gaim::BuddyList::Buddy::get_name($found_buddy);
		$account = Gaim::BuddyList::Buddy::get_account($found_buddy);
		$protocol = Gaim::Account::get_protocol_id( $account );
		
		Gaim::debug_info($WEBGAIM,"[$account][$protocol]\n");
		
		@accounts = Gaim::accounts();
		while( @accounts )
		{
			$acc = shift( @accounts );
			$proto = Gaim::Account::get_protocol_id( $acc );
			if( $protocol eq $proto )
			{
			
				$new_account = $acc;
				#break;
			}
		}

		$conv = Gaim::Conversations::find_with_account( $to,$new_account);
		Gaim::debug_info($WEBGAIM,"====================> [$conv][$new_account]\n");
		if( $conv )
		{
			Gaim::debug_info($WEBGAIM,"already have conversation:[$buddy_name][$account][$conv]\n");
		}
		else
		{
			Gaim::debug_info($WEBGAIM,"new conversation:[$buddy_name][$new_account]\n");

			#
			# HACK: This is my newbism to perl, but somehow if we do not use a has to store
			#			the conversation object everytime we open a new conversation we kill the old one
			#           If anyone knows a better way to do this please let me know :)
			#$g_conv{$to} = Gaim::Conversation::IM::new( $new_account , $to );
			$g_conv{$to} = Gaim::Conversation::IM::new( $new_account , $to );
			
		}
	}
}

#
#  Update the statuses of accounts, Ie if we got logged out somehow or something like that
#
sub update_account_status
{
	lock( @g_accounts );
	@g_accounts = ();
	@accounts = Gaim::accounts();
	while( @accounts )
	{
		$acc = shift( @accounts );
		$status = Gaim::Account::is_connected( $acc );
		$protocol = Gaim::Account::get_protocol_id( $acc );
		$acc_stat = "$protocol:$status";
		Gaim::debug_info($WEBGAIM,"account [$acc_stat]\n");
		push(@g_accounts,$acc_stat);
	}
}

#
# Get the latest buddy list from configs/accounts
#
sub update_buddy_list
{
	lock( %buddies_all );
	%buddies_all = ();
	
	my @groups = Gaim::BuddyList::groups();
	while( @groups )
	{
		my $group = shift( @groups );
		my $group_name = Gaim::BuddyList::Group::get_name($group);
		
		my @buddies = Gaim::BuddyList::Group::buddies( $group );
		while( @buddies )
		{
			my $buddy = shift( @buddies );
			my $buddy_name = Gaim::BuddyList::Buddy::get_name($buddy);
			my $buddy_alias = Gaim::BuddyList::Buddy::get_alias($buddy);
			my $buddy_account = Gaim::BuddyList::Buddy::get_account($buddy);
			my $buddy_protocol = Gaim::Account::get_protocol_id($buddy_account);
			
			$buddies_all{$buddy_name} = "$group_name:$buddy_alias:$buddy_protocol";
		}
	}
}

#
# See if any one else initiated a new conversation
#
sub update_conversations
{
	lock( @g_chats );
	@g_chats = ();
	
	my @gaim_chats = Gaim::conversations();
	while( @gaim_chats )
	{
		my $gaim_chat = shift( @gaim_chats );
		my $name = Gaim::Conversation::get_name( $gaim_chat );
		push(@g_chats,$name );
	}

}

#
# Callback for when our account has signed on 
#
sub signed_on_cb 
{
    my ($gc, $data) = @_;
    my $account = $gc->get_account();
    Gaim::debug_info($WEBGAIM,"Account " . $account->get_username() . " ONNLINE.\n");
	&update_account_status();
	&update_buddy_list();
}

#
# Callback for a sign in account
#
sub signed_off_cb 
{
    my ($gc, $data) = @_;
    my $account = $gc->get_account();
    Gaim::debug_info($WEBGAIM,"Account " . $account->get_username() . " OFFLINE.\n");
	&update_account_status();
	&update_buddy_list();
	&clear_active_buddies( $gc );
}

#
# Clear out any buuddies that were in an account we just logged out off
#  from the online buddies list
#
sub clear_active_buddies
{
	my $gc = shift @_;
    my $account = $gc->get_account();
	
	$logged_out_proto = Gaim::Account::get_protocol_id( $account );
	
	lock( @buddies_online );
	my @buddies_prev = @buddies_online;
	@buddies_online=();
	
	Gaim::debug_info($WEBGAIM,"++++++++++++++++++++++++++++++++++++++++\n");		
	lock( %buddies_all );
	
	while( @buddies_prev )
	{
		$name_prev = shift( @buddies_prev );
		if( exists( $buddies_all{$name_prev}) )
		{
			($group_name,$alias,$proto) = split(/:/,$buddies_all{$name_prev});
			
			if( $proto eq $logged_out_proto )
			{
				Gaim::debug_info($WEBGAIM,"buddy removed to to SIGNOFF");		
			}
			else
			{
				Gaim::debug_info($WEBGAIM,"buddy retained SIGNOFF");		
				push(@buddies_online,$name_prev);
			}
		}
		else
		{
			Gaim::debug_info($WEBGAIM,"buddy ERROR");		
			push(@buddies_online,$name_prev);
		}
		
	}
	Gaim::debug_info($WEBGAIM,"-----------------------------------------\n");
}


####################################################################################################33
# 
#  GAIM Webserver thread: This needs major work
#    the main loop of gaim_webserver was "borrowed" from QuickiWicki
#
sub gaim_webserver { 
	$port = 8111;

	do{
		$| = 1;
		
		$WNOHANG = 1;  
		$AF_INET = 2;
		$SOCK_STREAM = 1;
		$sockaddr = 'S n a4 x8';
		
		($name, $aliases , $proto ) = getprotobyname ('tcp');
		$this = pack($sockaddr, $AF_INET, $port, "\0\0\0\0");
		select(NS); $| = 1;
		socket(S, $AF_INET, $SOCK_STREAM, $proto) || die "socket: $!";
		$bindresult = bind(S,$this);
		if( $bindresult ne "1" )
		{
			Gaim::debug_info("webgaim","port $port is in use retrying in 1 sec ( $bindresult )\n");
			sleep(2);
		}
	}while( $bindresult ne "1" );
	Gaim::debug_info("webgaim","######################################\n");
	Gaim::debug_info("webgaim","running on port $port ( $bindresult )\n");
	Gaim::debug_info("webgaim","######################################\n");
	
	listen(S,5) || die "connect: $!";
	select(S); $| = 1; select(NS);

	Gaim::debug_info("webgaim","######################################################################### \n");
		
	for($con = 1; ; $con++) {
		($addr = accept(NS,S)) || die "accept: $!";
	#	if (($child = fork()) == O) {
			&service();
	#		exit;
	#	}
	#	close(NS);
	#	waitpid(-1, $WNOHANG);
	#	waitpid(-1, $WNOHANG);
	}
}

#
# Service qa httpd request
#
sub service {
	($af,$port,$inetaddr) = unpack($sockaddr,$addr);
	@inetaddr = unpack('C4',$inetaddr);
	$inetaddr = join('.', @inetaddr);

	Gaim::debug_info("webgaim::HTTP::","Request:\n");
	
	local (%head, %body, $query);
	$request = <NS>;
	($method, $file, $protocol) = $request =~ /^(\S*) \/(\S*) (\S*)/;
	%head = ();
	while(<NS>) {
		s/\r|\n//g;
		last unless /\S/;
		$head{"\L$1"}=$2 if /^(\S*): (.*)/;
		#print STDOUT "[$1][$2]\n";
		
		#
		# HACK: a fugly way at doing simple http authentication
		#
		if( $1 eq "Authorization" )
		{
			$idx = index($2,$opt_auth);
			print STDOUT "[$1][$2][$idx]\n";
			if( index($2,$opt_auth) ne -1 )
			{
				$has_auth = 1;
			}
		}
	}
	
	$file =~ s/\%(..)/pack(C, hex($1))/geo;
	print STDOUT "$con: $inetaddr $method $file $head\n";
	($file, $query) = ($`, $') if $file =~ /\?/;
  	
	Gaim::debug_info("webgaim::HTTP::","Request:$file,$query\n");

	#
	# If we have authorization try to do the work
	#
	if( $has_auth eq "1" )
	{
		#print NS "HTTP/1.1 401 Authorization Required\n";
		print NS "HTTP/1.1 200 OK\n";
		print NS "Date: Sun, 21 Nov 2004 20:56:06 GMT\n";
		print NS "Server: WebGaim\n";
		print NS "Connection: close\n";
		print NS "Content-type: text/html\n\n";
		print NS "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n";
		print NS "<html>\n";
		print NS " <head>\n";
		print NS "  <meta http-equiv=\"Pragma\" content=\"no-cache\">\n";
		print NS " </head>\n";
		print NS "<body>\n";
	
							
		if( $file eq "" )
		{
			&home();
		}
		elsif( $file eq "BuddyList" )
		{
			&BuddyList();
		}
		elsif( $file eq "login" )
		{
			&submit_work( "login:$query");
			print NS "<BR><BR>Logged In<br><BR>\n";
			
		}
		elsif( $file eq "logout" )
		{
			&submit_work( "logout:$query" );
			print NS "<BR><BR>Logged Out<br><BR>\n";
		}
		elsif( $file eq "chats" )
		{
			&active_chats();
		}	
		elsif( $file eq "start-chat" )
		{
			&submit_work( "start-chat:$query" );
			&chat( $query );
		}	
		elsif( $file eq "chat" )
		{
			&chat( $query );
		}	
		elsif( $file eq "log" )
		{
			&log( $query );
		}	
		elsif( $file eq "send" )
		{
			$query=~ s/\+/ /g;
			($name,$rest) = split(/=/,$query,2);
			$name=~ s/\&//s;
			$query="$name=$rest";
			&submit_work("send:$query");
			my $real_name = buddy_real( $name );
			add_chat_msg($real_name,$who_am_i,$rest);
			&chat( $name );
		}	
		else
		{
			&home();
		}
		&webgaim_footer();
	}
	else
	{
		#
		# Otherwise authorization was denied
		#
		print NS "HTTP/1.1 401 Authorization Required\n";
		print NS "Date: Sun, 21 Nov 2004 20:56:06 GMT\n";
		print NS "Server: webgaim 1.00\n";
		print NS "WWW-Authenticate: Basic realm=\"Login\"\n";
		print NS "Connection: close\n";
		print NS "Content-type: text/html\n\n";
	}
	close(NS);
}

#
# Print a simple footer that has backlinks to the main view
#
sub webgaim_footer()
{
	print NS "<HR>\n";
	print NS "<A HREF=BuddyList>Buddies<A>,<A HREF=home?>home</A>\n";
	print NS "</body></html>\n";	
}

#
# Print a header with links to account login/logout updates/etc
#
sub webgaim_header()
{
	my ($refresh) = @_;
	$logedin = 0;

	print NS "Acc: ";

	{
		lock(@g_accounts);
		@accounts_tmp = @g_accounts;
	}	

	while( @accounts_tmp )
	{
		$acc = shift( @accounts_tmp );
		($protocol,$status) = split(/:/,$acc,2);
		
		if( $protocol eq "prpl-yahoo" )
		{
			if( $status eq "" )
			{
				print NS "<A HREF=login?$protocol>y.</A>";
			}
			else
			{
				print NS "<A HREF=logout?$protocol>Y!</A>";
				$logedin++;
			}
		}
		elsif( $protocol eq "prpl-msn" )
		{
			if( $status eq "" )
			{
				print NS "<A HREF=login?$protocol>msn</A>";
			}
			else
			{
				print NS "<A HREF=logout?$protocol>MSN</A>";
				$logedin++;
			}
		}
		elsif( $protocol eq "prpl-oscar" )
		{
			if( $status eq "" )
			{
				print NS "<A HREF=login?$protocol>aol</A>";
			}
			else
			{
				print NS "<A HREF=logout?$protocol>AOL</A>";
				$logedin++;
			}
		}
		if( @accounts_tmp )
		{
			print NS " : ";
		}
	}
	print "  | CMD: <A HREF=$refresh>U</A> : <A HREF=options?>O</A>\n";
	print "<HR>\n";

	$logedin;
}

#
# Display online buddies and their IM statuses ( IE is there an active conversation, 
#   are there new messages, etc )
#
sub home()
{
	&webgaim_header("home?");
	
	my %g_chat_hash = ();
	{
		lock( @g_chats);
		my @tmp_chats = @g_chats;
		while( @tmp_chats )
		{
			my $chat = shift(@tmp_chats);
			$g_chat_hash{$chat} = "1";
		}
	}
	
	if( $logedin )
	{
		print NS "Buddies:<BR>\n";
		
		{
			lock( @buddies_online );
			@buddies_tmp = @buddies_online;
			#push(@buddies_tmp,"Buddy with spaces");
		}
		while( @buddies_tmp )
		{
			my $name = shift( @buddies_tmp );
			my $new_name = buddy_clean($name);
			lock(%g_chat_contents);
			
			if( exists( $g_chat_hash{$name} ) )
			{
				$new = 0;
				
				if( exists( $g_chat_contents{$name} ) )
				{
					lock(%g_chat_lines);
					$new = $g_chat_lines{$name};
				}
			
				print NS " + <A HREF=chat?$new_name>$name ( $new New )</A><BR>\n";
			}
			else
			{
				print NS " + <A HREF=start-chat?$new_name>$name<A>";
				if( exists( $g_chat_contents{$name} ) )
				{
					$lines = $g_chat_contents{$name} ;
					if( $lines ne "" )
					{
						print NS " ( <A HREF=log?$new_name>Log</A> )";
					}
				}
				print NS "<BR>\n";
			}
		}
	}
	else
	{
		print NS "Not loged into any accounts<BR>\n";
		#print NS "<A HREF=login?>Login All</A><BR>\n";
	}
}

#
# Print out all active chats
#
sub active_chats()
{
	my @chats_tmp = ();
	{
		lock( @g_chats );
		@chats_tmp = @g_chats;
	}

	while( @chats_tmp )
	{
		my $chat = shift( @chats_tmp );
		print NS "$chat<BR>\n";
	}
}

#
# In the chat screen if another user has sent you a message , show who and 
#  how many
sub see_also()
{

	$lines = "<HR>Activity:<BR>\n";
	my %g_chat_hash = ();
	{
		lock( @g_chats);
		my @tmp_chats = @g_chats;
		while( @tmp_chats )
		{
			my $chat = shift(@tmp_chats);
			$g_chat_hash{$chat} = "1";
		}
	}
	
	if( $logedin )
	{
		{
			lock( @buddies_online );
			@buddies_tmp = @buddies_online;
		}
		
		while( @buddies_tmp )
		{
			my $name = shift( @buddies_tmp );
			
			if( exists( $g_chat_hash{$name} ) )
			{
				$new = 0;
	
				if( exists( $g_chat_contents{$name} ) )
				{
					lock(%g_chat_lines);
					$new = $g_chat_lines{$name};
					Gaim::debug_info($WEBGAIM,"see_also:: $name ::$new\n");
				}
				else
				{
					Gaim::debug_info($WEBGAIM,"see_also:: $name ::!exists\n");
				}
				
				
				if( $new > 0 )
				{
					my $new_name = buddy_clean( $name );
					$lines = "$lines + <A HREF=chat?$new_name>$name ( $new New )</A><BR>\n";
				}
			}
			else
			{
				Gaim::debug_info($WEBGAIM,"see_also:: $name ::!nochat\n");			
			}
		}

				
		if( $lines ne "<HR>Activity:<BR>\n" )
		{
			print NS $lines;
		}
	}

}

#
# Display an entire buddy list 
#
sub BuddyList{
	&webgaim_header("BuddyList?");
	{
		lock( %buddies_all );
		@buddylist_tmp = keys( %buddies_all );
	}
	
	print NS "Buddies:<br>\n";	
	while(@buddylist_tmp )
	{
		$record = shift( @buddylist_tmp );
		($name,$group,$alias,$proto) = split( /:/,$record);
		if( $alias ne "" )
		{
			print NS " + $alias ( $name )<BR>\n";
		}
		else
		{
			print NS " + $name<BR>\n";
		}
	}
}

#
# If a chat is conversation ended show any messages we saw in the mean time
#
sub log
{
	my ($sender) = @_;
	$sender = buddy_real( $sender );
	&webgaim_header("log?$sender");
	lock(%g_chat_contents);
	$lines = $g_chat_contents{$sender};
	
	do{
		($line,$rest) =  split( /<:::>/,$lines,2);
		$lines = $rest;
		($who,$msg) = split(/<::>/,$line,2);
			
		if( $who eq $sender )
		{
			print NS "<B>$who:</B>$msg<BR>\n";
		}
		else
		{
			print NS "<B>$who:</B>$msg<BR>\n";
		}
	}while( $lines ne "" );
}

#
# Display the chat screen and submit chat/im messages for send
#
sub chat
{
	my ($sender) = @_;
	my @lines = ();
	my $count = 0;
	my $new_lines = "";
	my $real_sender = buddy_real( $sender );
	&webgaim_header("chat?$sender");
	lock(%g_chat_contents);
	$lines = $g_chat_contents{$real_sender};
	
	
	#
	# Trim the number of text lines to something resonable
	#
	do{
		($line,$rest) =  split( /<:::>/,$lines,2);
		push(@lines,$line);
		$count++;
		if( $count > $opt_chat_prune )
		{
			my $remove = shift( @lines );
		}
		else
		{
			$new_lines = "$new_lines<:::>$line";
		}
		$lines = $rest;
	}while( $lines ne "" );

	if( $opt_chat_display eq "top"  )
	{
		print NS "<form method=\"get\" action=\"/send?\">\n";
		print NS "<div>";
		print NS "<input style=\"-wap-input-format: *m\" name=\"$sender\" size=64/>\n";
		print NS "<input type=\"submit\" value=\"send\"/>\n";
		print NS "</div>";
		print NS "</form>";
	}
		
	while( @lines )
	{
	
		if( $opt_chat_display eq "top" )
		{
			#
			# Newest text is ontop
			#
			$line = pop(@lines);
		}
		else
		{
			$line = shift(@lines);
		}
		
		if( $line ne "" )
		{
			($who,$msg) = split(/<::>/,$line,2);
			if(  $opt_time_stamp eq "true" )
			{
				($ts,$real_who) = split(' ',$who,2);
			}
			else
			{
				$real_who = $who;
			}	
			
			if( $real_who eq $real_sender )
			{
			
				print NS "<FONT COLOR=$opt_color_me><B>$who:</B>$msg</FONT><BR>\n";
			}
			else
			{
				print NS "<FONT COLOR=$opt_color_them><B>$who:</B>$msg</FONT><BR>\n";
			}
		}
	}
	
	$g_chat_contents{$real_sender} = $new_lines;
	$g_chat_lines{$real_sender}    = 0;
	#
	# Now Add A message box:
	#
	if( $opt_chat_display eq "bottom"  )
	{
		print NS "<form method=\"get\" action=\"/send?\">\n";
		print NS "<div>";
		print NS "<input style=\"-wap-input-format: *m\" name=\"$sender\" size=64/>\n";
		print NS "<input type=\"submit\" value=\"send\"/>\n";
		print NS "</div>";
		print NS "</form>";
	}

	#
	# Show any other active conversation links
	#
	&see_also;
}


#########################################################
# Parts of GAIM are not thread safe.  So queue up a timeout
#   and have the timeout handler do the work for us
#
sub submit_work{
	my ($work) = @_;
	{
		lock( @todo );
		push(@todo,"$work");
	}
}

#############################
# Sanitize buddy names
#
sub buddy_clean
{
	my ($new_name) = @_;
	
	$new_name =~ s/\\/\\\\/g;
	$new_name =~ s/ /\\sp/g;
	return $new_name;
}

sub buddy_real
{
	my ($new_name) = @_;
	
	$new_name =~ s/\\\\/\\/g;
	$new_name =~ s/\\sp/ /g;
	
	return $new_name;
}