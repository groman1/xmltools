#ifndef XMLTOOLS_H_
#define XMLTOOLS_H_

#include <stdbool.h>
#include <wchar.h>
#include <stdint.h>

#define appendElement(ptr, value) insertElement(ptr, value, ptr->tagQty)
#define initEmptyXML() parseXML("")

typedef struct {
    wchar_t *name;
    wchar_t *value;
} xmlArg;

typedef struct xmlStruct xml;
typedef struct xmlTagStruct xmlTag;

struct xmlTagStruct {
    wchar_t *tagName;
	xml *child;
    xmlArg *args;
    uint32_t argsQty;
	bool isString;
};

struct xmlStruct {
	xml *parent;
	uint32_t tagQty;
	xmlTag *tagArr;
};

char *xmlToString(xml *ptr);
void fillEmptyXML(xml *ptr);
xml *parseXML(wchar_t *string);
void freeXML(xml *xmlDocument);
void freeXMLValue(xml *ptr, uint32_t index);
void copyElement(xml *ptr, xmlTag value, int position);
int removeElement(xml *ptr, uint32_t index);
int insertElement(xml *ptr, xmlTag value, uint32_t index);
xmlTag *findElement(xml *ptr, wchar_t *text);
void swapElements(xml *ptr, uint32_t index1, uint32_t index2);
xml *nestElement(xml *ptr, int index);

#endif
