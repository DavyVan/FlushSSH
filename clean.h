#ifndef _CLEAN_H_
#define _CLEAN_H_
#include <libssh2.h>

void clean_up_session(LIBSSH2_SESSION *session);

void clean_up_sock_libssh2(int sock);

void clean_up_channel(LIBSSH2_CHANNEL *channel);

#endif  // _CLEAN_H_
