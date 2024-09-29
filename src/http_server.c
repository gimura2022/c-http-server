#include <stdbool.h>
#include <regex.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "http_server.h"

#define error(msg, code) ({ perror(msg); exit(code); })
#define HTTP_BUFFER_SIZE 1024 * 64 

http_server_data_t http_get_default_server_data(void) {
	return (http_server_data_t) {
		.port = 8080
	};
}

typedef struct {
	int client_fd;
	http_server_context_t* server_context;
	http_path_responder_t* responders;
	size_t responder_count;
} http_reciver_args_t;

static void* http_reciver(http_reciver_args_t* args);

void http_start_server(http_server_data_t server_data, http_path_responder_t* responders, size_t responder_count) {
	int server_fd;
	struct sockaddr_in address;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) error("socket failed", EXIT_FAILURE);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(server_data.port);

	if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) error("bind failed", EXIT_FAILURE);
	if (listen(server_fd, 3) < 0) error("listen failed", EXIT_FAILURE);

	http_server_context_t context = {
		.runned = true
	};

	while (context.runned) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		http_reciver_args_t* http_rec_args = malloc(sizeof(http_reciver_args_t));
		
		if ((http_rec_args->client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_addr_len)) < 0) continue; 
		http_rec_args->server_context = &context;
		http_rec_args->responders = responders;
		http_rec_args->responder_count = responder_count;

		pthread_t thread_id;
		pthread_create(&thread_id, NULL, http_reciver, (void*) http_rec_args);	
		pthread_detach(thread_id);
	}
}

static http_request_type_t http_request_type_from_str(const char* str) {
	if (strcmp(str, "GET") == 0) {
		return HTTPRQT_GET;
	} else if (strcmp(str, "POST") == 0) {
		return HTTPRQT_POST;
	} else {
		return -1;
	}
}

static void* http_reciver(http_reciver_args_t* args) {
	uint8_t* buf = (uint8_t*) malloc(HTTP_BUFFER_SIZE); 
	ssize_t recv_size = recv(args->client_fd, buf, HTTP_BUFFER_SIZE, 0);

	if (recv_size <= 0) goto done;

	const char* http_ver = strtok(buf, " ");
	const char* reqv_type = strtok(buf, " ");
	const char* url = strtok(buf, "\r\n");

	if (http_ver == NULL) goto done;
	if (strcmp(http_ver, "HTTP/1.0") != 0 && strcmp(http_ver, "HTTP/1.1") != 0) goto done; 

	if (reqv_type == NULL) goto done;
	const http_request_type_t type = http_request_type_from_str(reqv_type);
	if (type == -1) goto done;

	if (url == NULL) goto done;

	for (size_t i = 0; i < args->responder_count; i++) {
		const http_path_responder_t* responder = &args->responders[i];
		bool is_avalidable = false;

		if (type != responder->request_type) is_avalidable = false;
		if (responder->use_regex) {
			regex_t regex;
			if (regcomp(&regex, responder->path, 0) != 0) goto done;

			switch (regexec(&regex, url, 0, NULL, 0)) {
				case 0:
					break;

				case REG_NOMATCH:
					is_avalidable = false;
					break;

				default:	
					regfree(&regex);
					goto done;
			}

			regfree(&regex);
		} else {
			if (strcmp(responder->path, url) != 0) is_avalidable = false;
		}

		if (is_avalidable) {
			const http_request_t request = (http_request_t) {
				.type = type,
				.content = strtok(NULL, "") 
			};

			responder->responder(args->server_context, request);
		}
	}

done:
	close(args->client_fd);
	free(buf);
	free(args);
}
