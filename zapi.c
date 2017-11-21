#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <regex.h>
#include "zvars.h"


char * requestData(char *url, char *data);

int main(int argc, char *argv[]) {

	if(argc != 5) {
		// check required user argument 
		printf("Usage: %s url username password query\n", argv[0]);

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
		//size = snprintf(NULL, 0, "{\"jsonrpc\": \"2.0\",\"method\": \"trend.get\",\"params\":{\"output\": [\"value_avg\",\"item_id\"],\"time_from\":\"1504828800\",\"time_till\":\"1505056500\",\"limit\":10},\"auth\": \"%s\",\"id\": 1 }", sessionId);
		size = snprintf(NULL, 0, "{\"jsonrpc\": \"2.0\",\"method\": \"trend.get\",\"params\": %s,\"auth\": \"%s\",\"id\": 1 }", argv[4], sessionId);

		// allocate memory for user query
		userData = (char *)malloc(size + 1);

		// allocate as userData
		//snprintf(userData, size+1, "{\"jsonrpc\": \"2.0\",\"method\": \"trend.get\",\"params\":{\"output\": [\"value_avg\",\"item_id\"],\"time_from\":\"1504828800\",\"time_till\":\"1505056500\",\"limit\":10},\"auth\": \"%s\",\"id\": 1 }", sessionId);
		snprintf(userData, size+1, "{\"jsonrpc\": \"2.0\",\"method\": \"trend.get\",\"params\": %s,\"auth\": \"%s\",\"id\": 1 }", argv[4], sessionId);

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
