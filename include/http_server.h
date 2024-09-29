#ifndef _http_server_h
#define _http_server_h

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum {
	HTTPRST_INFO_CONTINUE = 100,
	HTTPRST_INFO_SW_PROT  = 101,
	HTTPRST_INFO_PROCESSING = 102,
	HTTPRST_INFO_EARLY_HINTS = 103,

	HTTPRST_SUCESS_OK = 200,
	HTTPRST_SUCESS_CREATED = 201,
	HTTPRST_SUCESS_ACCEPTED = 202,
	HTTPRST_SUCESS_NO_AUTO_INFO = 203,
	HTTPRST_SUCESS_NO_CONTENT = 204,
	HTTPRST_SUCESS_RESET_CONTENT = 205,
	HTTPRST_SUCESS_PARTICAL_CONTENT = 206,
	HTTPRST_SUCESS_MULTI_STATUS = 207,
	HTTPRST_SUCESS_ALREDY_REPORTED = 208,
	HTTPRST_SUCESS_IM_USED = 209,
} http_responce_type_t;

typedef enum {
	HTTPRQT_GET = 0,
	HTTPRQT_POST,
} http_request_type_t;

typedef struct {
	http_responce_type_t type;
	const char* content;
} http_responce_t;

typedef struct {
	http_request_type_t type;
	const char* content;
} http_request_t;

typedef struct {
	bool runned;	
} http_server_context_t;

typedef http_responce_t (*http_responder_t)(http_server_context_t*, http_request_t);

typedef struct {
	const char* path;
	const http_responder_t responder;
	const http_request_type_t request_type;
	const bool use_regex;
} http_path_responder_t;

typedef struct {
	uint16_t port;	
} http_server_data_t;

http_server_data_t http_get_default_server_data(void);

void http_start_server(http_server_data_t server_data, http_path_responder_t* responders, size_t responder_count); 

#endif
