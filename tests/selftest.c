// Feeds data from the input file to itself for 10 times

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include "../src/xmltools.h"

struct timespec timespecDiff(struct timespec ts1, struct timespec ts2)
{
	struct timespec ret;
	ret.tv_sec = ts2.tv_sec-ts1.tv_sec-(ts1.tv_nsec>ts2.tv_nsec);
	ret.tv_nsec = (ts2.tv_nsec>ts1.tv_nsec?(ts2.tv_nsec):(ts2.tv_nsec+1000000000))-ts1.tv_nsec;
	return ret;
}

int main(int argc, char **argv)
{
	if (argc!=2)
	{
		puts("The only argument must be a filename");
		return 1;
	}
		
	FILE *f = fopen(argv[1], "r");
	if (!f)
	{
		puts("Invalid filename");
		return 1;
	}

	size_t size;
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *str = malloc(size+1);
	fread(str, 1, size, f);
	str[size] = 0;
	fclose(f);

	struct timespec start, end, diff;
	clock_gettime(CLOCK_REALTIME, &start);

	for (uint8_t t = 0; t<10; ++t)
	{
		xml *doc = parseXML(str);
		if (!doc)
		{
			puts("Parse failed");
			return 1;
		}
		
		free(str);

		str = xmlToString(doc, 1);

		freeXML(doc);
	}

	clock_gettime(CLOCK_REALTIME, &end);

	diff = timespecDiff(start, end);
	double secElapsed = diff.tv_sec+diff.tv_nsec/1000000000.0;
	printf("Took %.4lfs (avg. rate %lf MB/s)\n", secElapsed, ((double)(size*10))/secElapsed/1000000);

	return 0;
}
