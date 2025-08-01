#include <stdio.h>
#include <stdlib.h>
#include "xmltools.h"

int main()
{
	char *teststring =	"<bottom direction=\"down\">\n\
							<nestedtag>Some text here</nestedtag>\n\
						</bottom>";
	xml *document = parseXML(teststring);

	xmlValue testvalue;
	testvalue.isNested = 0;
	testvalue.value.str = "Some more text here";
	testvalue.args = malloc(sizeof(xmlArgs));
	testvalue.args[0].attr = "testarg1";
	testvalue.args[0].value = "testval1";
	testvalue.argsQty = 1;
	testvalue.tagName = "second";

	appendElement(document, testvalue);
	printf("Before swap: %s\n", document->dataArr[0].tagName);
	swapElements(document, 0, 1);
	printf("After swap: %s\n", document->dataArr[0].tagName);
	printf("Find test: %s\n", document->dataArr[findElement(document, "bottom")].args[0].attr);
	freeXML(document);
	free(testvalue.args);
}
