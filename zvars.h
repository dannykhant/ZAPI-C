struct MemoryStruct {
	char * memory;
	size_t size;
};


static size_t write_function(void *contents, size_t size, size_t nmemb, void *userp) {

	size_t realsize = size * nmemb;
	struct MemoryStruct * mem = (struct MemoryStruct *)userp;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if(mem->memory == NULL) {
		//out of memory
		printf("Not enough memory!");
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

char * json_parse(json_object * jobj) {
	/* parse & print json result */
	json_object * session_obj;
	const char * session_result;
	char * session_result_return;

	json_object_object_get_ex(jobj, "result", &session_obj);
	session_result = json_object_get_string(session_obj);
	strcpy(session_result_return, session_result);

	return session_result_return;
}

char * poller(char *url, char *data) {
	/* curl serve as a poller */
	
	CURL * curl;
	CURLcode res;

	struct curl_slist * header = NULL;
	struct MemoryStruct chunk;

	chunk.memory = malloc(1);
	chunk.size = 0;

	// curl handle
	curl = curl_easy_init();
	// assign header
	header = curl_slist_append(header, "Content-Type: application/json");
	
	if(curl) {
		// curl options to perform
		curl_easy_setopt(curl, CURLOPT_URL, url); 
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
	}

	// perform requests
	res = curl_easy_perform(curl);

	// cleanup
	curl_easy_cleanup(curl);
	curl_slist_free_all(header);

	return chunk.memory; 
}
