#include "directory.h"

Directory :: Directory(Value newName, size_t position, size_t newSize, size_t newSizeInFS) :
    File(newName, position, newSize, newSizeInFS)
{
    ;
}
