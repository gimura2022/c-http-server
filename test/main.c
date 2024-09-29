#include <stdbool.h>

#include "http_server.h"

static http_responce_t handler(http_server_context_t* context, http_request_t request) {
	return (http_responce_t) {
		.type = HTTPRST_SUCESS_OK,
		.content = "Hello, world!"
	};
}

int main(int argc, char* argv[]) {
	http_start_server(
		(http_server_data_t) {
			.port = 8080	
		},
		&(http_path_responder_t) {
			.path = "/",
			.responder = handler,
			.request_type = HTTPRQT_GET,
			.use_regex = false
		},
		1
	);	

	return 0;
}
