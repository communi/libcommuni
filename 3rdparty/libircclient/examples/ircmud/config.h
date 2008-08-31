/*
 * $Id$
 */

#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H


class CConfig
{
	public:
		std::string		irc_server;
		std::string		irc_nick;
		std::string		irc_nickserv_pass;
		std::string		irc_join_channel;
		std::string		join_channel;

		std::string		mud_server;

		std::string		log_path;
		unsigned int	log_level;

		// Run-time values
		sockaddr_in		mud_serv_in;

	public:
		CConfig();
		void	Load (const char * path) throw (std::string);
};


#endif /* INCLUDE_CONFIG_H */
