#ifndef _MESSAGE_CODE_H_
#define _MESSAGE_CODE_H_

#define MESSAGE_DIVIDER_EOT 0x04
#define MESSAGE_DIVIDER_ETX 0x03

// client to server
#define MESSAGE_CODE_SIGN_IN_REQUEST 0xf001
#define MESSAGE_CODE_SIGN_UP_REQUEST 0xf004
#define MESSAGE_CODE_CLIENT_EXIT_REQUEST 0xf007
#define MESSAGE_CODE_CLIENT_ACCEPT_SERVER_EXIT_REQUEST 0xf009

// server to client
#define MESSAGE_CODE_SIGN_IN_SUCCESS 0xf002
#define MESSAGE_CODE_SIGN_IN_FAILURE 0xf003
#define MESSAGE_CODE_SIGN_UP_SUCCESS 0xf005
#define MESSAGE_CODE_SIGN_UP_FAILURE 0xf006
#define MESSAGE_CODE_SERVER_EXIT_REQUEST 0xf008
#define MESSAGE_CODE_SERVER_ACCEPT_CLIENT_EXIT_REQUEST 0xf010

#endif
