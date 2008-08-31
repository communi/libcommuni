/*
 * $Id$
 */

#include "global.h"


static void * process_connection (void * arg)
{
	CIrcMud * pIrcMud = (CIrcMud *) arg;
	pIrcMud->Run();

	pthread_exit(0);
	return 0;
}


CIrcMud::CIrcMud (irc_session_t * session, const std::string& nick, color_mode_t cmode)
{
	pthread_mutexattr_t	attr;

	if ( pthread_mutexattr_init (&attr)
	|| pthread_mutexattr_settype (&attr, PTHREAD_MUTEX_RECURSIVE)
	|| pthread_mutex_init (&mutex_outgoing_buf, &attr) )
		throw (std::string("Could not init pthread mutex: ") + strerror(errno));

	if ( (mudsock = socket (AF_INET, SOCK_STREAM, 0)) < 0 )
		throw (std::string("Could not create socket: ") + strerror(errno));

	this->session = session;
	this->nick = nick;
	state = CONNECTING;

	incoming_offset = 0;
	outgoing_offset = 0;
	color_mode = cmode;
	time(&last_read_time);
}


CIrcMud::~CIrcMud()
{
	if ( mudsock >= 0 )
		close (mudsock);

	pthread_mutex_init (&mutex_outgoing_buf, 0);
}


bool CIrcMud::IsAlive()
{
	return state != DISCONNECTED;
}

// New line received from IRC, and should be sent to MUD
void CIrcMud::NewLineFromIRC (const char * data)
{
	if ( state != FORWARDING )
		return;

	pthread_mutex_lock (&mutex_outgoing_buf);
	unsigned int length = strlen (data);

	if ( length + 1 + outgoing_offset < sizeof(outgoing_buf) )
	{
		memcpy (outgoing_buf + outgoing_offset, data, length);
		outgoing_offset += length;
		outgoing_buf[outgoing_offset++] = '\n';
		outgoing_buf[outgoing_offset] = '\0';
	}
	else
		irc_cmd_msg (session, nick.c_str(), "<MUD>: Message is not sent: buffer owerflowed.");

	pthread_mutex_unlock (&mutex_outgoing_buf);
}


bool CIrcMud::Start (irc_dcc_t id)
{
	pthread_t tid;

	dccid = id;

	if ( pthread_create (&tid, 0, process_connection, (void *) this) )
	{
		gLog.Add (CLog::ERROR, "pthread_create() failed: %s", strerror(errno));
		irc_cmd_msg (session, nick.c_str(), "<MUD>: Could not connect to MUD.");
		state = DISCONNECTED;
		return false;
	}

	gLog.Add (CLog::DEBUG, "new thread started, id is %08X", tid);
	return true;
}


void CIrcMud::Terminate()
{
	state = DISCONNECTED;
}

void CIrcMud::Run()
{
	struct sockaddr_in saddr;
	memcpy (&saddr, &gCfg.mud_serv_in, sizeof(saddr));

    // Connect to the IRC server
	gLog.Add (CLog::DEBUG, "Connecting to the server %s", gCfg.mud_server.c_str());

    if ( connect (mudsock, (struct sockaddr *) &saddr, sizeof(saddr)) < 0 )
    {
		gLog.Add (CLog::ERROR, "Connect to the server %s failed: ", gCfg.mud_server.c_str(), strerror(errno));
		irc_cmd_msg (session, nick.c_str(), "<MUD>: Could not connect to MUD.");
		state = DISCONNECTED;
		return;
	}

	gLog.Add (CLog::DEBUG, "Connect to the server %s succeed", gCfg.mud_server.c_str());
	state = FORWARDING;

	while ( 1 )
	{
		fd_set in_set, out_set;
 		struct timeval tv;

		FD_ZERO (&in_set);
		FD_ZERO (&out_set);

		tv.tv_usec = 250000;
		tv.tv_sec = 0;

		// Add input descriptor if there is space in input buffer
		if ( incoming_offset < (sizeof (incoming_buf) - 1) )
			FD_SET (mudsock, &in_set);

		// Add output descriptor if there is something in output buffer
		pthread_mutex_lock (&mutex_outgoing_buf);

		if ( outgoing_offset > 0 )
			FD_SET (mudsock, &out_set);

		pthread_mutex_unlock (&mutex_outgoing_buf);

		if ( select (mudsock + 1, &in_set, &out_set, 0, &tv) < 0 )
		{
			if ( errno == EINTR )
				continue;

			gLog.Add (CLog::ERROR, "select() failed: %s", strerror(errno));
			irc_cmd_msg (session, nick.c_str(), "<MUD>: Connection terminated.");
			state = DISCONNECTED;
			return;
		}

		// Hey, we've got something to read!
		if ( FD_ISSET (mudsock, &in_set) )
		{
			unsigned int amount = (sizeof (incoming_buf) - 1) - incoming_offset;
			int length = read (mudsock, incoming_buf + incoming_offset, amount);

			if ( length <= 0 )
			{
				gLog.Add (CLog::ERROR, "read() failed: %s", strerror(errno));
				irc_cmd_msg (session, nick.c_str(), "<MUD>: Connection terminated.");
				state = DISCONNECTED;
				return;
			}

			incoming_offset += length;
			time(&last_read_time);
		}

        // We can write a stored buffer
		if ( FD_ISSET (mudsock, &out_set) )
		{
			int length;

			// Because outgoing_buf could be changed asynchronously, we should 
			// lock any change
			pthread_mutex_lock (&mutex_outgoing_buf);

			if ( (length = write (mudsock, outgoing_buf, outgoing_offset)) <= 0 )
			{
				pthread_mutex_unlock (&mutex_outgoing_buf);
				gLog.Add (CLog::ERROR, "write() failed: %s", strerror(errno));
				irc_cmd_msg (session, nick.c_str(), "<MUD>: Connection terminated.");
				state = DISCONNECTED;
				return;
			}

			if ( outgoing_offset - length > 0 )
				memmove (outgoing_buf, outgoing_buf + length, outgoing_offset - length);

			outgoing_offset -= length;
			pthread_mutex_unlock (&mutex_outgoing_buf);
		}

		ParseInputBuffer();

		if ( !mud2irc.empty() )
		{
			for ( unsigned int i = 0; i < mud2irc.size(); i++ )
			{
				if ( irc_dcc_msg (session, dccid, mud2irc[i].c_str()) )
					break;

				mud2irc.erase (mud2irc.begin() + i);
				i--;
			}
		}
	}
}


static void std_replace (std::string& str, const char * what, const char * to)
{
	std::string::size_type s;
	unsigned int length = strlen (what);

	while ( (s = str.find (what)) != std::string::npos )
	{
		if ( to )
			str.replace (s, length, to);
		else
			str.erase (s, length);
	}
}


typedef struct
{
	const char	*	mudcolor;
	const char	*	mircbbg;
	const char	*	mircwbg;

} replace_colors_t;


static replace_colors_t replace_colors[] = 
{
	{	"\033[0m",		"\003""00,01", 	"\003""01,00"	},	// C_CLEAR
	{	"\033[0;31m",	"\003""05,01", 	"\003""04,00"	},	// C_RED
	{	"\033[0;32m",	"\003""03,01", 	"\003""09,00"	},	// C_GREEN
	{	"\033[0;33m",	"\003""07,01", 	"\003""08,00"	},	// C_YELLOW
	{	"\033[0;34m",	"\003""02,01", 	"\003""12,00"	},	// C_BLUE
	{	"\033[0;35m",	"\003""06,01", 	"\003""13,00"	},	// C_MAGENTA
	{	"\033[0;36m",	"\003""02,01", 	"\003""11,00"	},	// C_CYAN
	{	"\033[0;37m",	"\003""15,01", 	"\003""14,00"	},	// C_WHITE

	{	"\033[1;30m",	"\003""14,01", 	"\003""15,00"	},	// C_D_GREY
	{	"\033[1;31m",	"\003""04,01", 	"\003""05,00"	},	// C_B_RED
	{	"\033[1;32m",	"\003""09,01", 	"\003""03,00"	},	// C_B_GREEN
	{	"\033[1;33m",	"\003""08,01", 	"\003""07,00"	},	// C_B_YELLOW
	{	"\033[1;34m",	"\003""12,01", 	"\003""02,00"	},	// C_B_BLUE
	{	"\033[1;35m",	"\003""13,01", 	"\003""06,00"	},	// C_B_MAGENTA
	{	"\033[1;36m",	"\003""11,01", 	"\003""02,00"	},	// C_B_CYAN
	{	"\033[1;37m",	"\003""00,01", 	"\003""01,00"	},	// C_B_WHITE

	{	"\033[0;5m",	0, 				0			},	// C_CLEAR
	{	"\033[0;4m",	0, 				0			},	// C_BLINK
	{	0,				0, 				0			}
};



void CIrcMud::PreprocessMudLine (std::string& line)
{
	// Replace all the '\xFF\xFF' to \xFF
	std_replace (line, "\xFF\xFF", "\xFF");

	for ( replace_colors_t * rc = replace_colors; rc->mudcolor; rc++ )
	{
		const char * to = 0;

        if ( color_mode == NORMAL )
        	to = rc->mircbbg;
		else if ( color_mode == INVERSED )
        	to = rc->mircwbg;

		std_replace (line, rc->mudcolor, to);
	}
}


void CIrcMud::ExecActions(const std::string& value)
{
	if ( value == "Your select: " ) 
	{
		pthread_mutex_lock (&mutex_outgoing_buf);
		strcat (outgoing_buf, "5\n");
		outgoing_offset = strlen (outgoing_buf);
		pthread_mutex_unlock (&mutex_outgoing_buf);
	}
}


void CIrcMud::ParseInputBuffer()
{
	while ( incoming_offset > 0 )
	{
		unsigned int offset;
		std::string mudstr;

		for ( offset = 0; offset < incoming_offset; offset++ )
		{
			if ( incoming_buf[offset] == 0x0A || incoming_buf[offset] == 0x0D )
			{
				incoming_buf[offset] = '\0';

				if ( (offset + 1 < incoming_offset)
				&& (incoming_buf[offset + 1] == 0x0D || incoming_buf[offset + 1] == 0x0A) )
					offset++;

				offset++;
				mudstr = incoming_buf;

				if ( incoming_offset - offset > 0 )
					memmove (incoming_buf, incoming_buf + offset, incoming_offset - offset);

				incoming_offset -= offset;
				break;
			}
		}

		// If there is an unfinished line, send it after a short delay
		if ( offset == incoming_offset )
		{
			if ( time(0) - last_read_time > 0 )
			{
				incoming_buf[incoming_offset] = '\0';
				mudstr = incoming_buf;
				incoming_offset = 0;
			}
			else
				break;
		}

		PreprocessMudLine (mudstr);
		ExecActions (mudstr);

		if ( mudstr.find_first_not_of (" \t") == std::string::npos )
			break;

		mud2irc.push_back (mudstr);
	}
}
