#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <curl/curl.h>

#include "platform.h"

struct String
{
	char *str;
	u32 length;
};
struct StringArray
{
	String *strs;

	u32 length;
	u32 capacity;
};

inline StringArray
CreateStringArray(u32 capacity)
{
	StringArray result = {};

	result.capacity = capacity;
	result.length   = 0;
	result.strs     = (String*) malloc(sizeof(String) * capacity);

    return result;
}

inline String
CreateString(u32 len)
{
	String result = {};

	result.str    = (char*) malloc(len);
	result.length = 0;

	return result;
}

inline String*
GetArrayHead(StringArray *array)
{
	String *result = (array->strs + (array->length - 1));
	return result;
}

inline String
GetQuoteText(char *quote)
{
	String result = {};

	const char *textMatch   = "\"text\":";
	const char *authorMatch = "\"author\":";

	char *start = strstr(quote, textMatch);
	char *end   = strstr(quote, authorMatch);

	result.str    = start + strlen(textMatch) + 1;
	result.length = ((end - strlen(authorMatch)) - start) - 1;

	return result;
}

inline String
GetQuoteAuthor(char *quote)
{
	String result = {};

	const char *authorMatch = "\"author\":";
	const char *endMatch    = "}]";

	char *start = strstr(quote, authorMatch);
	char *end   = strstr(start, endMatch);

	result.str    = start + strlen(authorMatch) + 1;
	result.length = end - result.str - 1;

	return result;
}

size_t
curlWriteback(void *ptr, size_t size, size_t nmemb, void *stream)
{	
	UNUSED(size);

	StringArray *array = (StringArray*) stream;

	assert(array->length <= (array->capacity - 1));

	String *head = (array->strs + array->length++);
	*head = CreateString(nmemb);

	head->length = nmemb;
	memcpy(head->str, (char*) ptr, nmemb);

	return size * nmemb;
}

int
main(int argc, char **argv)
{	
	u32 quoteCount = 1;

	if (argc > 1)
	{
		char *arg = argv[1];

		int c = atoi(arg);

		if (c)
		{
			quoteCount = c;
		}
	}

	StringArray array = CreateStringArray(quoteCount);

	CURL *curlHandle = curl_easy_init();
	if (curlHandle)
	{
		CURLcode res;
		curl_easy_setopt(curlHandle, CURLOPT_URL, "https://www.adafruit.com/api/quotes.php?random=1");
	    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, curlWriteback);
	    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &array);

	    for (u32 i = 0; i < quoteCount; ++i)
	    {
			res = curl_easy_perform(curlHandle);

			if (res != CURLE_OK)
			{
				fprintf(stderr, "Curl perform failed in some way, w/ code %u...\n", res);
				exit(1);
			}

			String *head = GetArrayHead(&array);

			String text   = GetQuoteText(head->str);
			String author = GetQuoteAuthor(head->str);

			printf("\"%.*s\" -- %.*s\n", (int) text.length, text.str, (int) author.length, author.str);
	    }
	    
		curl_easy_cleanup(curlHandle);
	}

	return 0;
}
