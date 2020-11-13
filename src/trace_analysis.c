#include "trace_analysis.h"
#include <stdbool.h>

bool analysis_is_ipv4(struct sockaddr *ip)
{
	if (ip->sa_family == AF_INET)return true;
	return false;
}