
#include "global.h"


CConfig	gCfg;
CLog	gLog;


static std::map<std::string, irc_dcc_t>	gConnectionMap;



static void dcc_chat_callback (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
	CIrcMud * pMud = (CIrcMud *) ctx;

	switch (status)
	{
	case LIBIRC_ERR_CLOSED:
		gLog.Add (CLog::ERROR, "DCC %d: chat closed", id);
		pMud->Terminate();
		break;

	case 0:
		if ( !data )
			pMud->Start(id);
		else
			pMud->NewLineFromIRC (data);
		break;

	default:
		gLog.Add (CLog::ERROR, "DCC %d: error", id);
		pMud->Terminate();
		break;
	}
}


static void event_connect (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	gLog.Add (CLog::INFO, "Successfully connected to IRC server %s", gCfg.irc_server.c_str());
	irc_cmd_join (session, "#sweethome", 0);
}


static void event_privmsg (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	if ( !origin || count < 2 )
		return;

	std::string phrase = params[1];

	gLog.Add (CLog::DEBUG, "[%s]: %s", origin, params[1]);

	if ( gConnectionMap.find(origin) != gConnectionMap.end() )
	{
		if ( phrase == "close" )
		{
			irc_dcc_destroy (session, gConnectionMap[origin]);
//			gConnectionMap.erase 
		}
		else
		{
			irc_cmd_msg (session, origin, "You're already playing MUD!");
		}
	}
	else
	{
		if ( phrase == "shutdown" )
			irc_cmd_quit (session, 0);

		if ( phrase.find ("mud") == 0 )
		{
			irc_dcc_t dccid;
			try
			{
				CIrcMud * pMud = new CIrcMud (session, origin, CIrcMud::INVERSED);

				if ( irc_dcc_chat (session, pMud, origin, dcc_chat_callback, &dccid) == 0 )
				{
					irc_cmd_msg (session, origin, "Started... please accept the DCC CHAT request.");
					gConnectionMap[origin] = dccid;
				}
				else
					irc_cmd_msg (session, origin, "Not possible now");
			}
			catch (...)
			{
				irc_cmd_msg (session, origin, "<MUD> Could not start new connection.");
				gLog.Add (CLog::ERROR, "Failed to create a new connection");
			}
		}
	}

	// Cleanup
	std::map<std::string, CIrcMud*>::iterator conn, conn_next;

    for ( conn = gConnectionMap.begin(); conn != gConnectionMap.end(); conn++ )
	{
		if ( !conn->second->IsAlive() )
		{
			delete conn->second;
			conn = gConnectionMap.erase(conn);
		}
	}
}

/*
static void event_join (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	dump_event (session, event, origin, params, count);

	irc_cmd_msg (session, params[0], "Hi all");
	irc_cmd_msg (session, "tim", "Hi tim");
}


static void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
	char buf[24];
	sprintf (buf, "%d", event);

	dump_event (session, buf, origin, params, count);
}
*/

static void event_notice (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	if ( !origin )
		return;

	gLog.Add (CLog::DEBUG, "NOTICE from '%s': %s", origin, params[1] );

	if ( strcasecmp (origin, "nickserv") )
		return;

	if ( strstr (params[1], "This nick is not registered") == params[1] )
	{
		std::string regcmd = "REGISTER " + gCfg.irc_nickserv_pass + " NOMAIL";
		gLog.Add (CLog::INFO, "Registering our nick with NICKSERV");
		irc_cmd_msg (session, "nickserv", regcmd.c_str());
	}
	else if ( strstr (params[1], "This nickname is registered and protected") == params[1] )
	{
		std::string identcmd = "IDENTIFY " + gCfg.irc_nickserv_pass;
		gLog.Add (CLog::INFO, "Identifying our nick with NICKSERV");
		irc_cmd_msg (session, "nickserv", identcmd.c_str());
	}
	else if ( strstr (params[1], "Password accepted - you are now recognized") == params[1] )
		gLog.Add (CLog::INFO, "Nickserv authentication succeed.");
}


void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
	if ( event > 400 )
	{
		gLog.Add (CLog::ERROR, "ERROR: %s: %s%s\n", 
				origin ? origin : "unknown",
				params[0],
				params[1] ? params[1] : "");
	}
}


int main (int argc, char **argv)
{
	irc_callbacks_t	callbacks;
	memset (&callbacks, 0, sizeof(callbacks));

	callbacks.event_connect = event_connect;
//	callbacks.event_join = event_join;
//	callbacks.event_nick = event_nick;
	callbacks.event_privmsg = event_privmsg;
	callbacks.event_notice = event_notice;
	callbacks.event_numeric = event_numeric;

	callbacks.event_ctcp_req = irc_event_ctcp_internal;
//	callbacks.event_dcc_chat_req = irc_event_dcc_chat;

	irc_session_t * s = irc_create_session (&callbacks);

	if ( !s )
	{
		printf ("Could not create session: %s\n", strerror(errno));
		return 1;
	}

	try
	{
		gCfg.Load (0);
		gLog.Init (gCfg.log_level, gCfg.log_path);
	}
	catch (std::string errmsg)
	{
		std::cerr << "Error: " << errmsg;
		exit (1);
	}

	irc_option_set (s, LIBIRC_OPTION_STRIPNICKS);

	if ( irc_connect (s, gCfg.irc_server.c_str(), 6667, 0, gCfg.irc_nick.c_str(), 0, "Tell me 'mud' to play mud") )
	{
		gLog.Add (CLog::FATAL, "Could not connect to IRC server %s: %d", gCfg.irc_server.c_str(), irc_strerror (irc_errno(s)));
		return 1;
	}

	irc_run (s);
	return 0;
}
