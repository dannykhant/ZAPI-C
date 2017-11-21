#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <regex.h>
#include "zvars.h"


char * requestData(char *url, char *data);

int main(int argc, char *argv[]) {

	if(argc != 6) {
		// check required user argument 
		printf("Usage: %s url username password method params\n", argv[0]);

	} else {

		char * url = "http://10.11.95.68/zabbix/api_jsonrpc.php";
		char * data;
		char * sessionId;
		char * userData;
		size_t size;

		// login to zabbix & get session id
		size = snprintf(NULL, 0, "{\"jsonrpc\": \"2.0\", \"method\": \"user.login\", \"params\": { \"user\": \"%s\", \"password\": \"%s\"}, \"id\": 1}", argv[2], argv[3]);
		data = (char *)malloc(size + 1);
		snprintf(data, size+1, "{\"jsonrpc\": \"2.0\", \"method\": \"user.login\", \"params\": { \"user\": \"%s\", \"password\": \"%s\"}, \"id\": 1}", argv[2], argv[3]);

		sessionId = requestData(url, data);

	
		// get size of user query 
		size = snprintf(NULL, 0, "{\"jsonrpc\": \"2.0\",\"method\": \"%s\",\"params\": %s,\"auth\": \"%s\",\"id\": 1 }", argv[4], argv[5], sessionId);

		// allocate memory for user query
		userData = (char *)malloc(size + 1);

		// allocate as userData
		snprintf(userData, size+1, "{\"jsonrpc\": \"2.0\",\"method\": \"%s\",\"params\": %s,\"auth\": \"%s\",\"id\": 1 }", argv[4], argv[5], sessionId);

		// return response
		printf("%s\n", requestData(url, userData));

	}
	
	return 0;
}


char * requestData(char *url, char *data) {
	/* login first */

	json_object * jobj = json_tokener_parse(poller(url, data));
	return json_parse(jobj);
}
