# XMLTools by groman1

# struct xml:
tagQty : quanitity of tags on the same level;\
parent : the parent group of tags;\
dataArr : the tags on the same level:
- value : can be either string or struct xml, depending on the isNesting variable
- tagName : name of the tag
- args : arguments of the tag:
- - attr : the attribute of the argument
- - value : the value of the attribute
- argsCount : the quanitity of arguments ( 1 by default, increments only if there is more than one tag present )
- isNesting : specifies whether the tag has another tag in it

# Functions: 
parseXML(char* string) : parses struct xml from string, returns xml*\
fillEmptyXML(xml *ptr) : allocates memory for the default xml*\
freeXML(xml *xmlDocument) : frees all the allocated memory for an xml structure\
freeXMLValue(xmlValue *value, int nestingCleared) : frees all memory from a specified dataArr element, nestingCleared specifies whether the memory at dataArr.value has been deallocated\
xmlToString(xml *ptr) : creates a string from a struct xml*\
copyElement(xml *ptr, xmlValue value, int index) : copies xmlValue to a specified index, shouldn't be used on its own\
removeElement(xml *ptr, int index) : removes element from ptr.dataArr at index specified\
insertElement(xml *ptr, xmlValue value, int index) : insert element at a specified position and shift element after it\
appendElement(xml *ptr, xmlValue value) : appends an element to a ptr.dataArr\
findElement(xml *ptr, char *textToFind) : finds an element with the tag name textToFind and returns its index
