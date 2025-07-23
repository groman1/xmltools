#include <stdio.h>
#include <stdlib.h>
#include "xmltools.h"

int main()
{
	char *teststring =	"<bottom>\n\
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
	appendElement(document->dataArr->value.xmlVal, testvalue);
	printf("%s", document->dataArr->value.xmlVal->dataArr[1].args[0].attr);
	// the head tag has tagQty = -1
}
