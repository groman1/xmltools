# XMLTools by groman1

# struct xml:
tagQty: quanitity of tags on the same level;\
parent: the parent group of tags;\
dataArr: the tags on the same level:\
- value: can be either string or struct xml, depending on the isNesting variable\
- tagName: name of the tag\
- args: arguments of the tag:\
- - attr: the attribute of the argument\
- - value: the value of the attribute\
- argsCount: the quanitity of arguments ( 1 by default, increments only if there is more than one tag present )\

# Functions: 
parseXML(char* string) : parses struct xml from string, returns xml*
freeXML(xml *xmlDocument) : frees all the allocated memory for an xml structure
xmlToString(xml *ptr) : creates a string from a struct xml*
