#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

#define min3(a, b, c) (((a)<(b)?(a):(b))<(c)?((a)<(b)?(a):(b)):(c))

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

typedef enum {
	TAG,
	ARG,
	VAL
} parseState;

void fillXMLHeader(xml *header, xml *parent)
{
	header->parent = parent;
	header->tagQty = 0;
	header->tagArr = 0;
}

wchar_t *trim(wchar_t *source, uint32_t len)
{
	while (*source==L' '||*source==L'\t'||*source==L'\n')
	{
		++source;
		--len;
	}
	while ((source[len-1]==L' '||source[len-1]==L'\t'||source[len-1]==L'\n')&&len>0) --len;
	wchar_t *dest = malloc(sizeof(wchar_t)*(len+1));
	wcsncpy(dest, source, len);
	dest[len] = 0;
	return dest;
}

#define currTag (currPtr->tagArr[currPtr->tagQty-1])

xml *parseXML(wchar_t *str)
{
	xml *document = malloc(sizeof(xml)), *currPtr = document;

	fillXMLHeader(document, 0);	

	parseState state = TAG;

	for (uint32_t i = 0; i<wcslen(str); ++i)
	{
		if (str[i]==' ' || str[i]=='\n' || str[i]=='\t') continue;

		if (str+i == wcsstr(str+i, L"<!--")) // comments
			i = wcsstr(str+i+4, L"-->")-str+4;

		switch (state)
		{
			case TAG:
				if (str[i]==L'<') // tag open
				{
					if (str[i+1]==L'?') // pi, dont worry for now
					{
						i = wcsstr(str+i, L"?>")-str+2;
						break;
					}
					currPtr->tagArr = realloc(currPtr->tagArr, (++currPtr->tagQty)*sizeof(xmlTag));
					currTag.child = NULL;
					currTag.args = NULL;
					currTag.argsQty = 0;
					currTag.isString = false;
					++i;
					uint32_t size = min3(wcschr(str+i, L' ')-1, wcschr(str+i, L'/')-1, wcschr(str+i, L'>')-1)+1-str-i; // done so that if not found the value is overflown and isn't counted an min
					currTag.tagName = malloc(sizeof(wchar_t)*(size+1));
					wcsncpy(currTag.tagName, str+i, size);
					currTag.tagName[size] = 0;
					i += size-1;
					++state;
				}
				else // string
				{
					state = VAL;
					--i;
				}
				break;
			case ARG:
				if (str[i]==L'/')
				{
					currTag.child = NULL;
					state = TAG;
					break;
				}
				else if (str[i]==L'>')
				{
					state = VAL;
					currTag.child = malloc(sizeof(xml));
					fillXMLHeader(currTag.child, currPtr);
					currPtr = currTag.child;
					break;
				}
				currTag.args = realloc(currTag.args, (++currTag.argsQty)*sizeof(xmlArg));
				uint32_t lenSpace = wcschr(str+i, L' ')-str-i, lenEq = wcschr(str+i, L'=')-str-i;
				
				if (lenEq>lenSpace) // no value
				{
					currTag.args[currTag.argsQty-1].name = malloc(sizeof(wchar_t)*(lenSpace)); // TODO?
					wcsncpy(currTag.args[currTag.argsQty-1].name, str+i, lenSpace);
					currTag.args[currTag.argsQty-1].value = NULL;
					i += lenSpace;
				}
				else
				{
					currTag.args[currTag.argsQty-1].name = malloc(sizeof(wchar_t)*(lenEq)); // TODO?
					wcsncpy(currTag.args[currTag.argsQty-1].name, str+i, lenEq);
					i += lenEq+1;
					if (str[i++]!=L'"') return (xml*)1;
					lenSpace = wcschr(str+i, L'"')-str-i;
					currTag.args[currTag.argsQty-1].value = malloc(sizeof(wchar_t)*(lenSpace));
					wcsncpy(currTag.args[currTag.argsQty-1].value, str+i, lenSpace);
					i += lenSpace;
				}
				break;
			case VAL:
				if (str[i]==L'<')
				{
					if (str[i+1]==L'/') // tag closing
					{
						++i;
						uint32_t tagEnd = wcschr(str+i, L'>')-str-i-1;
						wchar_t *tagEndText = malloc(sizeof(wchar_t)*(tagEnd+1));
						wcsncpy(tagEndText, str+i+1, tagEnd);
						tagEndText[tagEnd] = 0;
						currPtr = currPtr->parent;
						if (wcscmp(tagEndText, currTag.tagName)) return (xml*)1;
						i += tagEnd+1;
						free(tagEndText);
						break;
					}
					// nest
					state = TAG;
					--i;
				}
				else //string
				{
					currPtr->tagArr = realloc(currPtr->tagArr, (++currPtr->tagQty)*sizeof(xmlTag));
					currTag.isString = true;
					currTag.child = NULL;
					currTag.args = NULL;
					currTag.argsQty = 0;
					uint32_t size = wcschr(str+i, L'<')-str-i;
					currTag.tagName = trim(str+i, size);
					i += size-1;
				}
				break;
			default:
				break;
		}
	}
	return document;
}

#undef currTag

void insertElement(xml *ptr, xmlTag tag, uint32_t index)
{
	if (index>ptr->tagQty) return;
	ptr->tagArr = realloc(ptr->tagArr, sizeof(xmlTag)*(++ptr->tagQty));
	for (uint32_t i = ptr->tagQty-1; i>index; --i)
		ptr->tagArr[i] = ptr->tagArr[i-1];
	ptr->tagArr[index] = tag;
}

void removeElement(xml *ptr, uint32_t index)
{
	if (index>=ptr->tagQty) return;
	for (; index<ptr->tagQty-1; ++index)
		ptr->tagArr[index] = ptr->tagArr[index+1];
	ptr->tagArr = realloc(ptr->tagArr, sizeof(xmlTag)*(--ptr->tagQty));
}

xmlTag *findElement(xml *ptr, wchar_t* text)
{
	for (int i = 0; i<ptr->tagQty; ++i)
		if (wcsstr(ptr->tagArr[i].tagName, text))
			return &ptr->tagArr[i];
	return NULL;
}

void swapElements(xml *ptr, uint32_t index1, uint32_t index2)
{
	if (index1>=ptr->tagQty||index2>=ptr->tagQty) return;
	xmlTag tempTag = ptr->tagArr[index1];
	ptr->tagArr[index1] = ptr->tagArr[index2];
	ptr->tagArr[index2] = tempTag;
}

void freeXML(xml*);

void freeXMLTag(xmlTag ptr)
{
	if (ptr.child)
	{
		freeXML(ptr.child);
		ptr.child = NULL;
	}
	free(ptr.tagName);
	if (ptr.args)
	{
		for (uint32_t i = 0; i<ptr.argsQty; ++i)
		{
			free(ptr.args[i].name);
			free(ptr.args[i].value);
		}
		free(ptr.args);
	}
}

void freeXML(xml *ptr)
{
	ptr->parent = NULL;
	for (uint32_t i = 0; i<ptr->tagQty; ++i)
		freeXMLTag(ptr->tagArr[i]);
}
