#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "platform.h"

struct StringList
{
	char *str;
	StringList *next;
};

struct String
{
	char *str;
	u32 length;
};

StringList*
AllocStringList()
{
    StringList *result = (StringList*) malloc(sizeof(StringList));
    *result = {};

    return result;
}

StringList*
GetHead(StringList *list)
{
	StringList *result = list;

	while (result->next)
	{
		result = result->next;
	}

	return result;
}

String
GetQuoteText(char *quote)
{
	String result = {};

	const char *textMatch = "\"text\":";
	const char *authorMatch = "\"author\":";

	char *start = strstr(quote, textMatch);
	char *end   = strstr(quote, authorMatch);

	result.str = start + strlen(textMatch) + 1;
	result.length = ((end - strlen(authorMatch)) - start) - 1;

	return result;
}

String
GetQuoteAuthor(char *quote)
{
	String result = {};

	const char *authorMatch = "\"author\":";

	char *start = strstr(quote, authorMatch);

	result.str    = start + strlen(authorMatch) + 1;
	result.length = strlen(result.str) - 3;

	return result;
}

void
curlWriteback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	StringList *newStr = AllocStringList();
	newStr->str  = (char*) malloc(size * nmemb + 1);
	newStr->next = 0;

	StringList *list = (StringList*) stream;

	if (list->str)
	{
		StringList *head = GetHead(list);
		head->next = newStr;
	}
	else
	{
		list->str = newStr->str;
	}

	sprintf(newStr->str, "%.*s", (int) nmemb, (char*) ptr);
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

			#if defined(DEBUG)
			puts("Please note, requesting multiple quotes can take a while, we're rate limited by Adafruit...");
			#endif
		}
	}

	CURL *curlHandle = curl_easy_init();
	if (curlHandle)
	{
		StringList *list = AllocStringList();

		CURLcode res;
		curl_easy_setopt(curlHandle, CURLOPT_URL, "https://www.adafruit.com/api/quotes.php?random=1");
	    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, curlWriteback);
	    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, list);
		//curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L);

	    for (u32 i = 0; i < quoteCount; ++i)
	    {
			res = curl_easy_perform(curlHandle);

			if (res != CURLE_OK)
			{
				#if defined(DEBUG)
				fprintf(stderr, "Curl perform failed in some way...\n");
				#endif

				exit(1);
			}

			StringList *head = GetHead(list);

			String text   = GetQuoteText(head->str);
			String author = GetQuoteAuthor(head->str);

			printf("\"%.*s\" -- %.*s\n", (int) text.length, text.str, (int) author.length, author.str);
	    }
	    
		#if 0
		else
		{
			StringList *index = list;

			while (index)
			{
				puts(index->str);

				index = index->next;
			}
		}
		#endif

		curl_easy_cleanup(curlHandle);
	}

	return 0;
}
