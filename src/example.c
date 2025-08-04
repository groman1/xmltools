#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include "xmltools.h"

int main()
{
	char *teststring =	"<bottom direction=\"down\">\n\t<nestedtag>Some text here</nestedtag>\n</bottom>\n", *returnstring;

	xmlValue testvalue;
	testvalue.isNested = 0;
	testvalue.value.str = "Some more text here";
	testvalue.args = malloc(sizeof(xmlArgs));
	testvalue.args[0].attr = "testarg1";
	testvalue.args[0].value = "testval1";
	testvalue.argsQty = 1;
	testvalue.tagName = "second";

	xml *document = parseXML(teststring);
	printf("Input XML string:\n%s\n", teststring);

	appendElement(document, testvalue);
	printf("Before swap: %s\n", document->dataArr[0].tagName);
	swapElements(document, 0, 1);
	printf("After swap: %s\n", document->dataArr[0].tagName);

	removeElement(document, 1);
	printf("Removed element with id 1\n");

	printf("Find test: %s\n", document->dataArr[findElement(document, "second")].args[0].attr);

	returnstring = xmlToString(document);
	printf("%s\n", returnstring);
	free(returnstring);

	freeXML(document);
	free(testvalue.args);
}
