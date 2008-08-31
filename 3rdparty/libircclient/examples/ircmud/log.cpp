
#include "global.h"


CLog::CLog()
{
	level = 0;
	pthread_mutex_init (&mutex, 0);
}


CLog::~CLog()
{
	pthread_mutex_destroy (&mutex);
	file.close();
}

void CLog::Init (unsigned int level, const std::string& path) throw (std::string)
{
	this->level = level;

	file.open (path.c_str(), std::ios_base::binary|std::ios_base::out|std::ios_base::app);

	if ( !file )
		throw (std::string ("Could not create log file ") + path);
}

		
void CLog::Add (llev_t level, const char * fmt, ...)
{
	char buf[1024];
	va_list va_alist;

	if ( level > gCfg.log_level )
		return;

	va_start (va_alist, fmt);
	vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	printf ("%s\n", buf);

	file << buf << std::endl;
}
