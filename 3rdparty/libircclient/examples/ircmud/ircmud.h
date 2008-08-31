/*
 * $Id$
 */

#ifndef INCLUDE_IRMUD_H
#define INCLUDE_IRMUD_H

const unsigned int INCOMING_BUF_SIZE = 32768;
const unsigned int OUTGOING_BUF_SIZE = 256;


class CIrcMud
{
	public:
		enum color_mode_t
		{
			BLACKWHITE,
			NORMAL,
			INVERSED
		};

		CIrcMud (irc_session_t * session, const std::string& nick, color_mode_t cmode);
		~CIrcMud ();

		bool	IsAlive();
		void	Terminate();
		void	NewLineFromIRC (const char * data);
		bool	Start(irc_dcc_t id);
		void 	Run();

	private:
		void	ParseInputBuffer();
		void	ExecActions(const std::string& value);
		void	PreprocessMudLine(std::string& value);

		enum state_t
		{
			CONNECTING,
			FORWARDING,
			DISCONNECTED
		};

		state_t			state;
		int				mudsock;
		irc_session_t * session;
		std::string		nick;
		irc_dcc_t		dccid;
		color_mode_t	color_mode;

		std::vector<std::string>	mud2irc;

		// This buffer keeps the data received from MUD and waiting 
		// to be sent to IRC
		char 			incoming_buf[INCOMING_BUF_SIZE];
		unsigned int	incoming_offset;
		time_t			last_read_time;

		// This buffer keeps the data received from IRC and waiting 
		// to be sent to MUD
		char 			outgoing_buf[OUTGOING_BUF_SIZE];
		unsigned int	outgoing_offset;

		// Because NewLineFromIRC() will be called asynchronously from a
		// different thread, we need to use a mutex to lock the outgoing_buf
		// access
		pthread_mutex_t	mutex_outgoing_buf;
};


#endif /* INCLUDE_IRMUD_H */
