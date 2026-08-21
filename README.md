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
parseXML(char_t \*string) : parses struct xml from string, returns xml\*\
freeXML(xml \*xmlDocument) : frees all the allocated memory for an xml structure\
xmlToString(xml \*ptr) : creates a string from a struct xml\*\
removeElement(xml \*ptr, uint32_t index) : removes element from ptr.dataArr at index specified\
insertElement(xml \*ptr, xmlValue value, uint32_t index) : insert element at a specified position and shift element after it\
appendElement(xml \*ptr, xmlValue value) : appends an element to a ptr.dataArr\
findElement(xml \*ptr, char \*textToFind) : finds an element with the tag name textToFind and returns its index\
swapElements(xml \*ptr, int firstElemId, int secondElemId) : swaps elements at specified positions
