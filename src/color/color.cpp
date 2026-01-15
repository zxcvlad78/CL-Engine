#include "color.h"

Color(float r, float g, float b, float a)
{
    Color::Red = r;
    Color::Green = g;
    Color::Blue = b;
    Color::Alpha = a;
}

Color(float r, float g, float b)
{
    Color::Red = r;
    Color::Green = g;
    Color::Blue = b;
    Color::Alpha = 1.0f;
}