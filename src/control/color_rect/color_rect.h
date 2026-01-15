#ifndef COLOR_RECT_H
#define COLOR_RECT_H

#include "control/control.h"
#include "color/color.h"

class ColorRect : public Control
{
public:
    explicit ColorRect(const std::string& name = "ColorRect");
    
    void set_color(const glm::vec4& p_color);
    glm::vec4 get_color() const;

protected:
    void _draw() override;

private:
    void draw_solid_rect();
    glm::vec4 m_color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
};

#endif // color_rect.h