#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include "object/object.h"

class Resource : Object
{
public:
    Resource();
    ~Resource();

    void set_resource_path() {};
    std::string get_resource_path() const;
    void set_resource_name() {};
    std::string get_resource_name() const;

priavte:
    std::string resource_path;
    std::string resource_name;
    
}

#endif RESOURCE_H // resource.h
