#include <stdlib.h>
#include <string.h>

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
	bool isNesting;
};

struct xmlStruct {
	xml *parent;
	int tagQty;
	xmlValue *dataArr;
};

void fillDefault(xmlValue *val)
{
	val->value.str = malloc(1);
	val->tagName = malloc(1);
	val->tagName[0] = 0;
	val->isNesting = 0;
	val->argsQty = 1;
	val->args = malloc(sizeof(xmlArgs));
	val->args->attr = malloc(1);
	val->args->value = malloc(1);
}

void fillEmptyXML(xml *ptr, xml *parent) // leave parent at NULL if the tag is at top level
{
	ptr->parent = parent;
	ptr->tagQty = 1;
	ptr->dataArr = malloc(sizeof(xmlValue));
	fillDefault(ptr->dataArr);
}

char *xmlToString(xml *ptr)
{
	if (ptr->dataArr[0].tagName[0]!=0) return "";
	xml *currPtr = ptr->dataArr->value.xmlVal;
	int currTag = 0;
	char *string = malloc(1);
	int i, tabQty = 0, nested = 0, read = 0; // 0 - element; 1 - arguments; 2 - value
	for (i = 0; !(currPtr->tagQty<=currTag&&currPtr->parent==ptr);)
	{
		if (read==0)
		{
			++tabQty;
			string[i] = '<';
			string = realloc(string, ++i+1);
			for (int x = 0; currPtr->dataArr[currTag].tagName[x]!='\0'; ++x)
			{
				string[i] = currPtr->dataArr[currTag].tagName[x];
				string = realloc(string, ++i+1);
			}
			read = 1;
		}
		else if (read==1)
		{
			for (int x = 0; x<currPtr->dataArr[currTag].argsQty; ++x)
			{
				if (currPtr->dataArr[currTag].args[x].attr[0]) { string[i] = ' '; string = realloc(string, ++i+1); }
				for (int z = 0; currPtr->dataArr[currTag].args[x].attr[z]!=0; ++z)
				{
					string[i] = currPtr->dataArr[currTag].args[x].attr[z];
					string = realloc(string, ++i+1);
				}
				if (currPtr->dataArr[currTag].args[x].value[0]) //empty value
				{
					string[i++] = '=';
					string = realloc(string, ++i+1);
					string[i-1] = '"';
					for (int z = 0; currPtr->dataArr[currTag].args[x].value[z]; ++z)
					{
						string[i] = currPtr->dataArr[currTag].args[x].value[z];
						string = realloc(string, ++i+1);
					}
					string[i] = '"';
					string = realloc(string, ++i+1);
				}
			}
			string[i] = '>';
			string = realloc(string, ++i+1);
			read = 2;
		}
		else
		{
			if (currPtr->dataArr[currTag].value.xmlVal->parent!=currPtr) // func fillDefault has not been called
			{
				--tabQty;
				for (int x = 0; currPtr->dataArr[currTag].value.str[x]; ++x)
				{
					string[i] = currPtr->dataArr[currTag].value.str[x];
					string = realloc(string, ++i+1);
				}
				string[i++] = '<';
				string = realloc(string, ++i+1);
				string[i-1] = '/';
				for (int x = 0; currPtr->dataArr[currTag].tagName[x]; ++x)
				{
					string[i] = currPtr->dataArr[currTag].tagName[x];
					string = realloc(string, ++i+1);
				}
				string[i] = '>';
				string = realloc(string, ++i+1);
				if (++currTag==currPtr->tagQty)
				{
					for (int x = 0; x<currPtr->parent->tagQty; ++x)
					{
						if (currPtr->parent->dataArr[x].value.xmlVal==currPtr)
						{
							currTag = x+1;
							currPtr = currPtr->parent;
							nested = 1;
							break;
						}
					}
					if (nested!=1) break;
				}
				read = 0;
			}
			else
			{
				read = 0;
				currPtr = currPtr->dataArr[currTag].value.xmlVal;
				currTag	= 0;
			}
goback:
			if (currPtr==ptr) break;
			if (nested) --tabQty;
			string[i] = '\n';
			string = realloc(string, ++i+tabQty+1);
			for (int x = 0; x<tabQty; ++x)
			{
				string[i++] = '\t';
			}
			if (nested)
			{
				string[i++] = '<';
				string = realloc(string, ++i+1);
				string[i-1] = '/';
				for (int z = 0; currPtr->dataArr[currTag-1].tagName[z]; ++z)
				{
					string[i] = currPtr->dataArr[currTag-1].tagName[z];
					string = realloc(string, ++i+1);
				}
				string[i] = '>';
				string = realloc(string, ++i+1);
				if (currPtr->parent==ptr)
				{
					string[i] = '\n';
					string = realloc(string, ++i+1);
				}
				nested = 0;
				if (currTag>=currPtr->tagQty)
				{
					for (int x = 0; x<currPtr->parent->tagQty; ++x)
					{
						if (currPtr->parent->dataArr[x].value.xmlVal==currPtr)
						{
							currTag = x+1;
							currPtr = currPtr->parent;
							nested = 1;
							goto goback;
						}
					}
				}
			}
		}
	}
	string[i] = 0;
	return string;
}

void freeXML(xml *xmlDocument)
{
	int currTag = 0;
	xml *currPtr = xmlDocument->dataArr->value.xmlVal;
	while(!(currTag==currPtr->tagQty&&currPtr->parent==xmlDocument))
	{
		free(currPtr->dataArr[currTag].tagName);
		for (int i = 0; i<currPtr->dataArr[currTag].argsQty; ++i)
		{
			free(currPtr->dataArr[currTag].args[i].attr);
			free(currPtr->dataArr[currTag].args[i].value);
		}
		free(currPtr->dataArr[currTag].args);
		if (!currPtr->dataArr[currTag].isNesting)
		{
			if (currTag<currPtr->tagQty-1)
			{
				++currTag;
				free(currPtr->dataArr[currTag].value.str);
			}
			else
			{
				do
				{
					for (int i = 0; i<currPtr->parent->tagQty; ++i)
					{
						if (currPtr->parent->dataArr[i].value.xmlVal==currPtr)
						{
							currTag = i;
							free(currPtr->dataArr[i].value.xmlVal->dataArr);
							free(currPtr->dataArr[i].value.xmlVal);
							currPtr = currPtr->parent;
							break;
						}
					}
				}
				while(currTag>=currPtr->parent->tagQty-1&&currPtr->parent!=xmlDocument);
				++currTag;
			}
		}
		else
		{
			currPtr = currPtr->dataArr[currTag].value.xmlVal;
			currTag = 0;
		}
	}
	free(xmlDocument->dataArr->args->attr);
	free(xmlDocument->dataArr->args->value);
	free(xmlDocument->dataArr->value.xmlVal);
	free(xmlDocument->dataArr);
	free(xmlDocument);
}

void freeXMLValue(xmlValue *value, int nestedCleared)
{
	for (int i = 0; i<value->argsQty; ++i)
	{
		free(value->args[i].attr);
		free(value->args[i].value);
	}
	free(value->args);
	free(value->tagName);
	if (!nestedCleared) free(value->value.str);
}

void copyElement(xml *ptr, xmlValue value, int position)
{
	ptr->dataArr = realloc(ptr->dataArr, ++ptr->tagQty*sizeof(xmlValue));

	ptr->dataArr[position].tagName = malloc(strlen(value.tagName));
	strcpy(ptr->dataArr[position].tagName, value.tagName);

	ptr->dataArr[position].argsQty = value.argsQty;
	ptr->dataArr[position].args = malloc(sizeof(xmlArgs)*value.argsQty);
	for (int i = 0; i<value.argsQty; ++i)
	{
		ptr->dataArr[position].args[i].attr = malloc(strlen(value.args[i].attr));
		ptr->dataArr[position].args[i].value = malloc(strlen(value.args[i].value));

		strcpy(ptr->dataArr[position].args[i].attr, value.args[i].attr);
		strcpy(ptr->dataArr[position].args[i].value, value.args[i].value);
	}

	ptr->dataArr[position].isNesting = value.isNesting;
	if (value.isNesting)
	{
		ptr->dataArr[position].value.xmlVal = value.value.xmlVal;
	}
	else
	{
		ptr->dataArr[position].value.str = malloc(strlen(value.value.str));
		strcpy(ptr->dataArr[position].value.str, value.value.str);
	}
}

void removeElement(xml *ptr, int index)
{
	if (index>ptr->tagQty-1) return;
	--ptr->tagQty;

	if (ptr->dataArr[index].isNesting)	
	{
		freeXML(ptr->dataArr[index].value.xmlVal); 
		freeXMLValue(&ptr->dataArr[index], 1);
	}
	else freeXMLValue(&ptr->dataArr[index], 0);

	for (int i = index; i<ptr->tagQty; ++i)
	{
		ptr->dataArr[i] = ptr->dataArr[i+1];
	}
	ptr->dataArr = realloc(ptr->dataArr, ptr->tagQty);
}

void insertElement(xml *ptr, xmlValue value, int index)
{
	if (index>ptr->tagQty) return;
	else if (index==ptr->tagQty) copyElement(ptr, value, ptr->tagQty);
	else
	{
		ptr->dataArr =  realloc(ptr->dataArr, ptr->tagQty+1);
		for (int i = ptr->tagQty; i>index; --i)
		{
			ptr->dataArr[i] = ptr->dataArr[i-1];
		}
		copyElement(ptr, value, index);
	}
}

xml *parseXML(char *string)
{
    xml *xmlDocument = malloc(sizeof(xml));
	fillEmptyXML(xmlDocument, 0);
	xmlDocument->tagQty = -1;
	free(xmlDocument->dataArr->value.str);
	xml *currPtr = xmlDocument;
	currPtr->dataArr->value.xmlVal = malloc(sizeof(xml));
	currPtr = currPtr->dataArr->value.xmlVal;
	fillEmptyXML(currPtr, xmlDocument);
	int len, size = 0, nested = 1;
#ifndef NOCHECKS
	{
		int opened = 0, quoteQty = 0, eqQty = 0;
		for (len = 0; string[len]!='\0'; ++len)
		{
			if (string[len] == '<') opened+=3;
			else if (string[len] == '>') opened-=2;
			else if (string[len] == '/') opened-=2;
			else if (string[len] == '"') ++quoteQty;
			else if (string[len] == '=') ++eqQty; //check if all args have 2 quotes
		}
		if (opened||!len||(quoteQty!=eqQty<<1))
		{
			return (xml*)(long long)((opened?1<<8:0)+(!len<<4)+(quoteQty!=eqQty<<1));
		}
	}
#else
	for (len = 0; string[len]!='\0'; ++len);
#endif
	int readElem = 1, readArgs = 0, readValue = 0;
	for (int i = 0; i<len; ++i)
    {
		if (string[i] == '<')
        {
            if (string[i+1]=='/') // tag closing
			{
				i+=2;
				if (--nested<0) // two tags closed in a row
				{
					++nested;
					currPtr = currPtr->parent;
					readValue = 0;
				}
				else if (nested==0)
				{
					readValue = 0;
				}
				for (int x = 0; string[i]!='>'; ++x, ++i)
				{
#ifndef NOCHECKS
					if (string[i]!=currPtr->dataArr[currPtr->tagQty-1].tagName[x]) 
					{
						return (void*)1;	// check if opening and closing tags are matching
					}
#endif
				}
				if(size!=0)
				{
					currPtr->dataArr[currPtr->tagQty-1].value.str[size] = '\0';
					size = 0;
				}
			}
			else if (!readValue&&string[i+1]!='/'&&currPtr->dataArr[0].tagName[0]!='\0') //tag opening
			{
				++(currPtr->tagQty);
				nested = 1;
				currPtr->dataArr = realloc(currPtr->dataArr, currPtr->tagQty*sizeof(xmlValue));
				fillDefault(&currPtr->dataArr[currPtr->tagQty-1]);	
				readElem = 1; // group elements with equal indentation
			}
			else if (readValue&&string[i+1]!='/') // tag opening inside another tag, nesting
			{ 
				currPtr->dataArr[currPtr->tagQty-1].isNesting = 1;
				free(currPtr->dataArr[currPtr->tagQty-1].value.str); 
				currPtr->dataArr[currPtr->tagQty-1].value.xmlVal = malloc(sizeof(xml)); 
				fillEmptyXML(currPtr->dataArr[currPtr->tagQty-1].value.xmlVal, currPtr); 
				currPtr = currPtr->dataArr[currPtr->tagQty-1].value.xmlVal;			// nesting tags
				readValue = 0, readElem = 1, nested = 1; 
			}
        }
		else if (string[i]=='\n'||string[i]=='\t'); //ignore all newlines and \t
        else
        {
            if (readElem)
			{
				if (string[i]=='>') 
				{ 
					readElem = 0; readValue = 1;
					currPtr->dataArr[currPtr->tagQty-1].tagName[size] = '\0';
					size = 0;
				}
				else if (string[i]!=' ')
				{
					currPtr->dataArr[currPtr->tagQty-1].tagName[size++] = string[i];
					currPtr->dataArr[currPtr->tagQty-1].tagName = realloc(currPtr->dataArr[currPtr->tagQty-1].tagName, size+1);
				}
				else 
				{
					readElem = 0; readArgs = 1;
					currPtr->dataArr[currPtr->tagQty-1].tagName[size] = '\0';
					size = 0;
				}
            }
            else if (readArgs)
            {
                int readArgValue = 0, insideQuote = 0;
                while (1)
                {
					if (string[i]=='>')
					{
						currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].value[size] = '\0';
						size = 0;
						break;
					}
                    if (readArgValue)
                    {
                        if (string[i]==' '&&!insideQuote) 
						{ 
							currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].value[size] = '\0';
                            ++(currPtr->dataArr[currPtr->tagQty-1].argsQty);
							currPtr->dataArr[currPtr->tagQty-1].args = realloc(currPtr->dataArr[currPtr->tagQty-1].args, currPtr->dataArr[currPtr->tagQty-1].argsQty*sizeof(xmlArgs));
							currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].attr = malloc(1);
							currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].value = malloc(1);
							size = 0;
							readArgValue = 0; 
                        }
                        else
                        {
							if (string[i]=='"') insideQuote = !insideQuote;
							else
							{
								currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].value[size++] = string[i];
								currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].value = realloc(currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].value, size+1);
							}
                        }
                    }
                    else
                    {
                        if (string[i]=='='||string[i]==' ') 
						{
							currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].attr[size] = '\0';
							readArgValue = 1;
							size = 0;
						}
						else
						{
							currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].attr[size++] = string[i];
							currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].attr = realloc(currPtr->dataArr[currPtr->tagQty-1].args[currPtr->dataArr[currPtr->tagQty-1].argsQty-1].attr, size+1);
						}
                    }        
                    ++i;
                }
				readArgs = 0;
				readValue = 1;
				size = 0;
            }
            else if (readValue)
            {
                currPtr->dataArr[currPtr->tagQty-1].value.str[size++] = string[i];
				currPtr->dataArr[currPtr->tagQty-1].value.str = realloc(currPtr->dataArr[currPtr->tagQty-1].value.str, size+1);
            }
        }
    }
    return xmlDocument;
}
