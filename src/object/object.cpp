//object.cpp

#include "object.h"
#include <iostream>

Object::Object(const std::string& name)
    : m_name(name)
{
}

Object::~Object()
{
}

void Object::set_name(const std::string& name)
{
    m_name = name;
}

std::string Object::to_string() const
{
    return "[Object: " + m_name + "]";
}