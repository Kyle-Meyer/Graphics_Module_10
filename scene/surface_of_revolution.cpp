#include "scene/surface_of_revolution.hpp"

#include <algorithm>

namespace cg
{

SurfaceOfRevolution::SurfaceOfRevolution() {}

SurfaceOfRevolution::SurfaceOfRevolution(std::vector<Point3> &v,
                                         uint32_t             n,
                                         int32_t              position_loc,
                                         int32_t              normal_loc)
{
    // Set number of rows and columns
    num_rows_ = static_cast<uint32_t>(v.size());
    num_cols_ = n + 1;

    // Add vertices to the vertex list, compute normals
    Vector3         normal, prev_normal;
    VertexAndNormal vtx;
    auto            vtx_iter_1 = v.begin();
    auto            vtx_iter_2 = vtx_iter_1 + 1;
    for(uint32_t i = 0; vtx_iter_2 != v.end(); vtx_iter_1++, vtx_iter_2++, i++)
    {
        normal = {vtx_iter_2->z - vtx_iter_1->z, 0.0f, vtx_iter_1->x - vtx_iter_2->x};
        normal.normalize();
        vtx.vertex = {vtx_iter_1->x, vtx_iter_1->y, vtx_iter_1->z};
        if(i == 0)
        {
            // Use normal for the first edge vertex
            vtx.normal = normal;
        }
        else
        {
            // Average normals of successive edges
            vtx.normal = (prev_normal + normal).normalize();
        }
        vertices_.push_back(vtx);

        // Copy normal for use in averaging
        prev_normal = normal;
    }

    // Store last vertex
    vtx.vertex = {vtx_iter_1->x, vtx_iter_1->y, vtx_iter_1->z};
    vtx.normal = normal;
    vertices_.push_back(vtx);

    // Reverse the vertex list so we go from top to bottom so
    // ConstructRowColFaceList forms ccw triangles
    std::reverse(vertices_.begin(), vertices_.end());

    // Create a rotation matrix
    Matrix4x4 m;
    m.rotate_z(360.0f / static_cast<float>(n));

    // Rotate the prior "edge" vertices
    uint32_t index = 0; // Index to the prior edge at this row
    for(uint32_t i = 0; i < n; i++)
    {
        for(uint32_t j = 0; j < num_rows_; j++)
        {
            // Rotate the vertex and the normal
            vtx.vertex = m * vertices_[index].vertex;
            vtx.normal = m * vertices_[index].normal;
            vertices_.push_back(vtx);
            index++;
        }
    }

    // Copy the first column of vertices
    for(uint32_t i = 0; i < num_rows_; i++) { vertices_.push_back(vertices_[i]); }

    // Construct the face list and create VBOs
    construct_row_col_face_list(num_cols_, num_rows_);
    create_vertex_buffers(position_loc, normal_loc);
}

} // namespace cg
