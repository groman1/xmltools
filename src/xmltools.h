#ifndef XMLTOOLS_H_
#define XMLTOOLS_H_

typedef struct {
	char *attr;
	char *value;
} xmlArgs;

typedef struct xmlStruct xml;
typedef struct xmlValueStruct xmlValue;

struct xmlValueStruct {
	char *tagName;
	struct {
		char *str;
		xml *xmlVal;
	} value;
	xmlArgs *args;
	int argsQty;
};

struct xmlStruct {
	xml *parent;
	int tagQty;
	xmlValue *dataArr;
};

xml *parseXML(char *string);
void freeXML(xml *xmlDocument);

#endif
