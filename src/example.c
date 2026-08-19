#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <threads.h>
#include "xmltools.h"

int main()
{
	wchar_t *teststring = L"<bottom direction=\"down\">\n\t<nestedtag>Some text here</nestedtag>\n\tTest bro\n</bottom>\n<string arg=\"test\">\n\tTest\n</string>", *returnstring;

	//wchar_t arg[] = "testarg1", val[] = "testval1";

	/*xmlTag testvalue;
	testvalue.tagName = malloc("Some more text here";*/
	xml *document = parseXML(teststring);
	printf("Input XML string:\n%ls\n", teststring);

	printf("Before swap: %ls\n", document->tagArr[0].tagName);
	swapElements(document, 0, 1);
	printf("After swap: %ls\n", document->tagArr[0].tagName);

	removeElement(document, 1);
	printf("Removed element with id 1\n");

	printf("Find test: %ls\n", findElement(document, L"string")->args[0].name);

	//returnstring = xmlToString(document);
	//printf("%s\n", returnstring);
	//free(returnstring);

	freeXML(document);
}
