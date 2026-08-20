# XMLTools by groman1
Lightweight C library for reading and writing XML/HTML files

# Documentation

## wchar_t support
Compiled with wchar support by default.  
If it's not necessary and you wish to save some runtime memory, comment out the #define WIDECHAR in src/xmltools.h

## xml:
tagQty : quanitity of tags on the same level;\
parent : the parent group of tags;\
tagArr : the tags on the same level:
- child : the xml (header) of the child element(s)
- tagName : name of the tag
- args : arguments of the tag:
- - name : the name of the argument
- - value : the value of the argument (can be NULL)
- argsQty: the quanitity of arguments
- isString : specifies whether the current element is a string (is so, the text is stored in tagName)

## Functions: 
parseXML(char* string) : parses struct xml from string, returns xml*\
fillEmptyXML(xml *ptr) : allocates memory for the default xml*\
freeXML(xml *xmlDocument) : frees all the allocated memory for an xml structure\
freeXMLValue(xml *ptr, int index) : frees all memory from xml->dataArr[index]\
xmlToString(xml *ptr) : creates a string from a struct xml*\
copyElement(xml *ptr, xmlValue value, int index) : copies xmlValue to a specified index, shouldn't be used on its own\
removeElement(xml *ptr, int index) : removes element from ptr.dataArr at index specified\
insertElement(xml *ptr, xmlValue value, int index) : insert element at a specified position and shift element after it\
appendElement(xml *ptr, xmlValue value) : appends an element to a ptr.dataArr\
findElement(xml *ptr, char *textToFind) : finds an element with the tag name textToFind and returns its index\
swapElements(xml *ptr, int firstElemId, int secondElemId) : swaps elements at specified positions\
nestElement(xml *ptr, xml *newptr, int index) : sets newptr to ptr->dataArr[index].value.xmlVal
