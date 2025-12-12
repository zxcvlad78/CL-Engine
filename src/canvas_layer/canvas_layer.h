#ifndef CANVAS_LAYER_H
#define CANVAS_LAYER_H

#include "node2d/node2d.h"
#include <vector>
#include <memory>

class CanvasItem;

class CanvasLayer : public Node2D {
public:
    explicit CanvasLayer(const std::string& name = "CanvasLayer");
    ~CanvasLayer() override = default;
    
    void set_layer(int layer);
    int get_layer() const { return m_layer; }
    
    void set_follow_viewport(bool follow);
    bool is_following_viewport() const { return m_follow_viewport; }
    
    void add_item(CanvasItem* item);
    void remove_item(CanvasItem* item);
    
    void ready() override;
    void process(float delta) override;
    void render();
    
private:
    int m_layer = 0;
    bool m_follow_viewport = false;
    std::vector<CanvasItem*> m_items;
};

#endif // CANVAS_LAYER_H