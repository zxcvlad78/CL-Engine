//object.cpp

#include "object.h"
#include <iostream>

Object::Object(const std::string& name)
    : m_name(name)
{
    std::cout << "Object created: " << m_name << std::endl;
}

Object::~Object()
{
    std::cout << "Object destroyed: " << m_name << std::endl;
}

void Object::set_name(const std::string& name)
{
    m_name = name;
}

std::string Object::to_string() const
{
    return "[Object: " + m_name + "]";
}