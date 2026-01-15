#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
    Color(float r, float g, float b, float a);
    Color(float r, float g, float b);
    ~Color();

    float Red = 0.0f;
    float Green = 0.0f;
    float Blue = 0.0f;
    float Alpha = 0.0f;

private:

};

#endif