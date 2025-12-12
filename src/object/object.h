//object.h

#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <memory>

class Object {
public:
    explicit Object(const std::string& name = "Object");
    virtual ~Object();
    
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    
    Object(Object&&) = default;
    Object& operator=(Object&&) = default;
    
    void set_name(const std::string& name);
    const std::string& get_name() const { return m_name; }
    
    virtual std::string to_string() const;

private:
    std::string m_name;

};

#endif