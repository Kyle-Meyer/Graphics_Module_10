#include "scene/unit_square.hpp"

#include "geometry/geometry.hpp"

#include <iostream>

namespace cg
{

UnitSquareSurface::UnitSquareSurface() {}

UnitSquareSurface::UnitSquareSurface(uint32_t n, int32_t position_loc, int32_t normal_loc)
{
    // Only allow 250 subdivision (so it creates less that 65K vertices)
    if(n > 250) n = 250;

    // Create VBOs and VAO
    // Normal is 0,0,1. z = 0 so all vertices lie in x,y plane.
    // Having issues with roundoff when n = 40,50 - so compare with some tolerance
    VertexAndNormal vtx;
    vtx.normal = {0.0f, 0.0f, 1.0f};
    vtx.vertex.z = 0.0f;
    float spacing = 1.0f / static_cast<float>(n);
    for(vtx.vertex.y = -0.5f; vtx.vertex.y <= 0.5f + EPSILON; vtx.vertex.y += spacing)
    {
        for(vtx.vertex.x = -0.5f; vtx.vertex.x <= 0.5f + EPSILON; vtx.vertex.x += spacing)
        {
            vertices_.push_back(vtx);
        }
    }

    // Construct the face list and create VBOs
    construct_row_col_face_list(n + 1, n + 1);
    create_vertex_buffers(position_loc, normal_loc);

    std::cout << "vertex list size = " << vertices_.size();
    std::cout << " face list size = " << faces_.size() << '\n';
}

} // namespace cg
