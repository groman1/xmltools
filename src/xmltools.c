#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *attr;
    char *value;
} xmlArgs;

typedef struct xmlStruct xml;
typedef struct xmlValueStruct xmlValue;

struct xmlValueStruct {
    char *name;
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
	xmlValue *arr;
};

void fillDefault(xmlValue *val)
{
	val->value.str = malloc(1);
	val->name = malloc(1);
	val->argsQty = 1;
	val->args = malloc(sizeof(xmlArgs));
	val->args->attr = malloc(1);
	val->args->value = malloc(1);
}
xml *parseXML(char *string)
{
    xml *xmlDocument = malloc(sizeof(xml));
	xmlDocument->tagQty = 1;
	xmlDocument->arr = malloc(sizeof(xmlValue));
	xml *currPtr = xmlDocument;
	fillDefault(xmlDocument->arr);
    int opened = 0, pars = 0, len, readElem = 1, readArgs = 0, readValue = 0;
	int size = 0;
    for (len = 0; string[len]!='\0'; ++len)
    {
        if (string[len] == '<') ++opened;
        else if (string[len] == '>') --opened;
		else if (string[len] == '"') pars = !pars;
    }
    if (opened||!len||pars) return NULL;
	for (int i = 0; i<len; ++i)
    {
		if (string[i] == '<')
        {
            if (readValue&&string[i+1]=='/') 
			{ 
				readValue = 0; readElem = 1; 
				currPtr->arr[currPtr->tagQty-1].value.str[size] = '\0';
				++(currPtr->tagQty); 
				currPtr->arr = realloc(currPtr->arr, currPtr->tagQty*sizeof(xmlValue));
				size = 0;
				fillDefault(&currPtr->arr[currPtr->tagQty-1]);
				while(string[++i]!='>');  // TODO: IMPLEMENT TAG CHECK HERE
			}
			else if (readValue&&string[i+1]!='/') 
			{ 
				readValue = 0; readElem = 1; 
				currPtr->arr[currPtr->tagQty-1].value.xmlVal = malloc(sizeof(xml)); 
				currPtr->arr[currPtr->tagQty-1].value.xmlVal->parent = &currPtr[currPtr->tagQty-1]; 
				currPtr = currPtr->arr[currPtr->tagQty-1].value.xmlVal; 
				currPtr->arr = malloc(sizeof(struct xmlStruct));
				currPtr->tagQty = 1;
				fillDefault(currPtr->arr); // nesting tags
			}
        }
        else
        {
            if (readElem)
			{
				if (string[i]=='>') 
				{ 
					readElem = 0; readValue = 1;
					currPtr->arr[currPtr->tagQty-1].name[size] = '\0';
					size = 0;
				}
				else if (string[i]!=' ')
				{
					currPtr->arr[currPtr->tagQty-1].name[size++] = string[i];
					currPtr->arr[currPtr->tagQty-1].name = realloc(currPtr->arr[currPtr->tagQty-1].name, size);
				}
				else 
				{
					readElem = 0; readArgs = 1;
					currPtr->arr[currPtr->tagQty-1].name[size] = '\0';
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
						currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].value[size] = '\0';
						size = 0;
					}
                    if (readArgValue)
                    {
                        if (string[i]!=' '&&!insideQuote) 
						{ 
                            ++(currPtr->arr[currPtr->tagQty-1].argsQty);
							currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].value[size++] = string[i];
							currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].value = realloc(currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].value, size);
							readArgValue = 0; 
                        }
                        else
                        {
							if (string[i]=='"'||string[i]=='\'') insideQuote = !insideQuote;
                            currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].attr[size++] = string[i];
							currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].attr = realloc(currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].attr, size);
                        }
                    }
                    else
                    {
                        if (string[i]=='='||string[i]==' ') 
						{
							currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].attr[size] = '\0';
							readArgValue = 1;
							size = 0;
						}
						else
						{
							currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].attr[size++] = string[i];
							currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].attr = realloc(currPtr->arr[currPtr->tagQty-1].args[currPtr->arr[currPtr->tagQty-1].argsQty].attr, size);
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
                currPtr->arr[currPtr->tagQty-1].value.str[size++] = string[i];
				currPtr->arr[currPtr->tagQty-1].value.str = realloc(currPtr->arr[currPtr->tagQty-1].value.str, size);
            }
        }
    }
    return xmlDocument;
}
