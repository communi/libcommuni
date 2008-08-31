#include "global.h"


static void ConvertAddr (const std::string& addrport, unsigned short default_port, sockaddr_in * saddr)
{
	std::string::size_type sep;
	std::string addr = addrport;

	if ( (sep = addr.find_last_of(": ")) != std::string::npos )
	{
		std::string port = addr.substr(sep+1);
		default_port = atoi (port.c_str());
		addr.erase (sep);

		if ( default_port <= 0 )
			throw (std::string ("Port " + port + " is incorrect"));
	}
	else if ( default_port == 0 )
		throw (std::string ("Port should be specified in " + addr));

	memset (saddr, 0, sizeof(*saddr));
	saddr->sin_family = AF_INET;
	saddr->sin_port = htons (default_port);
	saddr->sin_addr.s_addr = inet_addr (addr.c_str());

	if ( saddr->sin_addr.s_addr == INADDR_NONE )
	{
		struct hostent *host = gethostbyname (addr.c_str());
		
		if ( !host )
			throw (std::string ("Could not resolve hostname " + addr));

		saddr->sin_addr.s_addr = ((struct in_addr*)host->h_addr)->s_addr;
	}
}


CConfig::CConfig()
{
}


void CConfig::Load (const char * path) throw (std::string)
{
	// just init, not load
	irc_server = irc.address.here;
	irc_nick = irc.bot.nick;
	irc_nickserv_pass = irc.bot.nickserv.pass;
	irc_join_channel = irc.channel.to.join;
	mud_server = address.of.mud;

	log_path = "log.log";
	log_level = 5;

	// Convert and analyze values
	ConvertAddr (mud_server, 0, &mud_serv_in);
}
