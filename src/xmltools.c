#include <stdlib.h>

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

void fillDefault(xmlValue *val)
{
	val->value.str = malloc(1);
	val->tagName = malloc(1);
	val->argsQty = 1;
	val->args = malloc(sizeof(xmlArgs));
	val->args->attr = malloc(1);
	val->args->value = malloc(1);
}

void freeXML(xml *xmlDocument)
{
	int currTag = 0;
	xml *currPtr = xmlDocument;
	while(currTag<xmlDocument->tagQty&&currPtr==xmlDocument)
	{
		if (currPtr->dataArr[currTag].value.xmlVal!=NULL)
		{
			currPtr = currPtr->dataArr[currTag].value.xmlVal;
		}
		else
		{
			free(currPtr->dataArr[currTag].tagName);
			currPtr->dataArr[currTag].tagName = NULL;
			for (int i = 0; i<currPtr->dataArr[currTag].argsQty; ++i)
			{
				free(currPtr->dataArr[currTag].args[i].attr);
				free(currPtr->dataArr[currTag].args[i].value);
			}
			currPtr->dataArr[currTag].args->attr = NULL;
			currPtr->dataArr[currTag].args->value = NULL;
			free(currPtr->dataArr[currTag].args);
			currPtr->dataArr[currTag].args = NULL;
			free(currPtr->dataArr[currTag].value.str);
			currPtr->dataArr[currTag].value.str = NULL;
			if (&currPtr->dataArr[currTag+1]!=NULL)
			{
				++currTag;
			}
			else
			{
				free(currPtr->dataArr);
				currPtr = currPtr->parent;
			}
		}
	}
}

xml *parseXML(char *string)
{
    xml *xmlDocument = malloc(sizeof(xml));
	xmlDocument->tagQty = 1;
	xmlDocument->dataArr = malloc(sizeof(xmlValue));
	xml *currPtr = xmlDocument;
	int len, size = 0, nested = 0;
	fillDefault(xmlDocument->dataArr);
	{
		int opened = 0, quoteQty = 0, eqQty = 0;
		for (len = 0; string[len]!='\0'; ++len)
		{
			if (string[len] == '<') ++opened;
			else if (string[len] == '>') --opened;
			else if (string[len] == '"') ++quoteQty;
			else if (string[len] == '=') ++eqQty; //check if all args have 2 quotes
		}
		if (opened||!len||(quoteQty!=eqQty<<1)) return (void*)2;
	}
	int readElem = 1, readArgs = 0, readValue = 0;
	for (int i = 0; i<len; ++i)
    {
		if (string[i] == '<')
        {
            if (readValue&&string[i+1]=='/') 
			{
				currPtr->dataArr[currPtr->tagQty-1].value.str[size] = '\0';
				if (--nested<0)
				{
					currPtr = currPtr->parent;
					++nested;
				}
				i+=2;
				size = 0;
				readValue = 0;
				for (int x = 0; string[i]!='>'; ++x, ++i)
				{
					if (string[i]!=currPtr->dataArr[currPtr->tagQty-1].tagName[x]) 
					{
						return (void*)1;	// check if opening and closing tags are matching
					}
				}
			}
			else if (!readValue&&string[i+1]!='/'&&currPtr->dataArr[0].tagName[0]!='\0')
			{
				++(currPtr->tagQty);
				currPtr->dataArr = realloc(currPtr->dataArr, currPtr->tagQty*sizeof(xmlValue));
				fillDefault(&currPtr->dataArr[currPtr->tagQty-1]);	
				readElem = 1; //group elements with equal indentation
			}
			else if (readValue&&string[i+1]!='/') 
			{ 
				readValue = 0, readElem = 1, nested = 1; 
				currPtr->dataArr[currPtr->tagQty-1].value.xmlVal = malloc(sizeof(xml)); 
				currPtr->dataArr[currPtr->tagQty-1].value.xmlVal->parent = currPtr; 
				currPtr = currPtr->dataArr[currPtr->tagQty-1].value.xmlVal; 
				currPtr->dataArr = malloc(sizeof(xmlValue));
				currPtr->tagQty = 1;
				fillDefault(&currPtr->dataArr[0]); // nesting tags
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
