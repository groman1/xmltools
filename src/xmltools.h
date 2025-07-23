#ifndef XMLTOOLS_H_
#define XMLTOOLS_H_

#define bool unsigned char
#define appendElement(ptr, value) insertElement(ptr, value, ptr->tagQty)

typedef struct {
	char *attr;
	char *value;
} xmlArgs;

typedef struct xmlStruct xml;
typedef struct xmlValueStruct xmlValue;


struct xmlValueStruct {
	char *tagName;
	union {
		char *str;
		xml *xmlVal;
	} value;
	xmlArgs *args;
	int argsQty;
	bool isNested;
};

struct xmlStruct {
	xml *parent;
	int tagQty;
	xmlValue *dataArr;
};

char *xmlToString(xml *ptr);
void fillEmptyXML(xml *ptr);
xml *parseXML(char *string);
void freeXML(xml *xmlDocument);
void freeXMLValue(xmlValue *value, int nestingCleared);
void copyElement(xml *ptr, xmlValue value, int position);
void removeElement(xml *ptr, int index);
void insertElement(xml *ptr, xmlValue value, int index);

#endif
