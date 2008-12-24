/*
* Copyright (C) 2008 J-P Nurmi jpnurmi@gmail.com
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
* License for more details.
*
* $Id$
*/

#ifndef IRC_H
#define IRC_H

#include <ircglobal.h>

namespace Irc
{
    IRC_EXPORT const char* version();

    static const uint RPL_WELCOME	= 001;
    static const uint RPL_YOURHOST	= 002;
    static const uint RPL_CREATED	= 003;
    static const uint RPL_MYINFO	= 004;
    static const uint RPL_BOUNCE	= 005;

    static const uint RPL_USERHOST	= 302;
    static const uint RPL_ISON	    = 303;
    static const uint RPL_AWAY	    = 301;
    static const uint RPL_UNAWAY	= 305;
    static const uint RPL_NOWAWAY	= 306;

    static const uint RPL_WHOISUSER	    = 311;
    static const uint RPL_WHOISSERVER	= 312;
    static const uint RPL_WHOISOPERATOR	= 313;
    static const uint RPL_WHOISIDLE	    = 317;
    static const uint RPL_ENDOFWHOIS	= 318;
    static const uint RPL_WHOISCHANNELS	= 319;
    static const uint RPL_WHOWASUSER	= 314;
    static const uint RPL_ENDOFWHOWAS	= 369;

    static const uint RPL_LIST	    = 322;
    static const uint RPL_LISTEND	= 323;

    static const uint RPL_UNIQOPIS	    = 325;
    static const uint RPL_CHANNELMODEIS	= 324;

    static const uint RPL_NOTOPIC	= 331;
    static const uint RPL_TOPIC	    = 332;

    static const uint RPL_INVITING	= 341;
    static const uint RPL_SUMMONING	= 342;

    static const uint RPL_INVITELIST	    = 346;
    static const uint RPL_ENDOFINVITELIST	= 347;

    static const uint RPL_EXCEPTLIST	    = 348;
    static const uint RPL_ENDOFEXCEPTLIST	= 349;

    static const uint RPL_VERSION	= 351;

    static const uint RPL_WHOREPLY	= 352;
    static const uint RPL_ENDOFWHO	= 315;

    static const uint RPL_NAMREPLY	= 353;
    static const uint RPL_ENDOFNAMES	= 366;

    static const uint RPL_LINKS	= 364;
    static const uint RPL_ENDOFLINKS	= 365;

    static const uint RPL_BANLIST	= 367;
    static const uint RPL_ENDOFBANLIST	= 368;

    static const uint RPL_INFO	= 371;
    static const uint RPL_ENDOFINFO	= 374;

    static const uint RPL_MOTDSTART	= 375;
    static const uint RPL_MOTD	= 372;
    static const uint RPL_ENDOFMOTD	= 376;

    static const uint RPL_YOUREOPER	= 381;
    static const uint RPL_REHASHING	= 382;
    static const uint RPL_YOURESERVICE	= 383;

    static const uint RPL_TIME	= 391;
    static const uint RPL_USERSSTART	= 392;

    static const uint RPL_USERS	= 393;
    static const uint RPL_ENDOFUSERS	= 394;
    static const uint RPL_NOUSERS	= 395;

    static const uint RPL_TRACELINK	= 200;
    static const uint RPL_TRACECONNECTING	= 201;
    static const uint RPL_TRACEHANDSHAKE	= 202;
    static const uint RPL_TRACEUNKNOWN	= 203;
    static const uint RPL_TRACEOPERATOR	= 204;
    static const uint RPL_TRACEUSER	= 205;
    static const uint RPL_TRACESERVER	= 206;
    static const uint RPL_TRACESERVICE	= 207;
    static const uint RPL_TRACENEWTYPE	= 208;
    static const uint RPL_TRACECLASS	= 209;
    static const uint RPL_TRACELOG	= 261;
    static const uint RPL_TRACEEND	= 262;

    static const uint RPL_STATSLINKINFO	= 211;
    static const uint RPL_STATSCOMMANDS	= 212;
    static const uint RPL_ENDOFSTATS	= 219;
    static const uint RPL_STATSUPTIME	= 242;
    static const uint RPL_STATSOLINE	= 243;

    static const uint RPL_UMODEIS	= 221;

    static const uint RPL_SERVLIST	= 234;
    static const uint RPL_SERVLISTEND	= 235;

    static const uint RPL_LUSERCLIENT	= 251;
    static const uint RPL_LUSEROP	= 252;
    static const uint RPL_LUSERUNKNOWN	= 253;
    static const uint RPL_LUSERCHANNELS	= 254;
    static const uint RPL_LUSERME	= 255;

    static const uint RPL_ADMINME	= 256;
    static const uint RPL_ADMINLOC1	= 257;
    static const uint RPL_ADMINLOC2	= 258;
    static const uint RPL_ADMINEMAIL	= 259;

    static const uint RPL_TRYAGAIN	= 263;

    static const uint ERR_NOSUCHNICK	= 401;
    static const uint ERR_NOSUCHSERVER	= 402;
    static const uint ERR_NOSUCHCHANNEL	= 403;
    static const uint ERR_CANNOTSENDTOCHAN	= 404;
    static const uint ERR_TOOMANYCHANNELS	= 405;
    static const uint ERR_WASNOSUCHNICK	= 406;
    static const uint ERR_TOOMANYTARGETS	= 407;
    static const uint ERR_NOSUCHSERVICE	= 408;
    static const uint ERR_NOORIGIN	= 409;
    static const uint ERR_NORECIPIENT	= 411;
    static const uint ERR_NOTEXTTOSEND	= 412;
    static const uint ERR_NOTOPLEVEL	= 413;
    static const uint ERR_WILDTOPLEVEL	= 414;
    static const uint ERR_BADMASK	= 415;
    static const uint ERR_UNKNOWNCOMMAND	= 421;
    static const uint ERR_NOMOTD	= 422;
    static const uint ERR_NOADMININFO	= 423;
    static const uint ERR_FILEERROR	= 424;
    static const uint ERR_NONICKNAMEGIVEN	= 431;
    static const uint ERR_ERRONEUSNICKNAME	= 432;
    static const uint ERR_NICKNAMEINUSE	= 433;
    static const uint ERR_NICKCOLLISION	= 436;
    static const uint ERR_UNAVAILRESOURCE	= 437;
    static const uint ERR_USERNOTINCHANNEL	= 441;
    static const uint ERR_NOTONCHANNEL	= 442;
    static const uint ERR_USERONCHANNEL	= 443;
    static const uint ERR_NOLOGIN	= 444;
    static const uint ERR_SUMMONDISABLED	= 445;
    static const uint ERR_USERSDISABLED	= 446;
    static const uint ERR_NOTREGISTERED	= 451;
    static const uint ERR_NEEDMOREPARAMS	= 461;
    static const uint ERR_ALREADYREGISTRED	= 462;
    static const uint ERR_NOPERMFORHOST	= 463;
    static const uint ERR_PASSWDMISMATCH	= 464;
    static const uint ERR_YOUREBANNEDCREEP	= 465;
    static const uint ERR_YOUWILLBEBANNED	= 466;
    static const uint ERR_KEYSET	= 467;
    static const uint ERR_CHANNELISFULL	= 471;
    static const uint ERR_UNKNOWNMODE	= 472;
    static const uint ERR_INVITEONLYCHAN	= 473;
    static const uint ERR_BANNEDFROMCHAN	= 474;
    static const uint ERR_BADCHANNELKEY	= 475;
    static const uint ERR_BADCHANMASK	= 476;
    static const uint ERR_NOCHANMODES	= 477;
    static const uint ERR_BANLISTFULL	= 478;
    static const uint ERR_NOPRIVILEGES	= 481;
    static const uint ERR_CHANOPRIVSNEEDED	= 482;
    static const uint ERR_CANTKILLSERVER	= 483;
    static const uint ERR_RESTRICTED	= 484;
    static const uint ERR_UNIQOPPRIVSNEEDED	= 485;
    static const uint ERR_NOOPERHOST	= 491;
    static const uint ERR_UMODEUNKNOWNFLAG	= 501;
    static const uint ERR_USERSDONTMATCH	= 502;
}

#endif // IRC_H
