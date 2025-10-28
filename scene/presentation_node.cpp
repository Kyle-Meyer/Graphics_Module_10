#include "scene/presentation_node.hpp"

namespace cg
{

PresentationNode::PresentationNode()
{
    node_type_ = SceneNodeType::PRESENTATION;
    material_shininess_ = 1.0f;
}

PresentationNode::PresentationNode(const Color4 &ambient,
                                   const Color4 &diffuse,
                                   const Color4 &specular,
                                   const Color4 &emission,
                                   float         shininess) :
    material_ambient_(ambient),
    material_diffuse_(diffuse),
    material_specular_(specular),
    material_emission_(emission),
    material_shininess_(shininess)
{
}

void PresentationNode::set_material_ambient(const Color4 &c) { material_ambient_ = c; }

void PresentationNode::set_material_diffuse(const Color4 &c) { material_diffuse_ = c; }

void PresentationNode::set_material_ambient_and_diffuse(const Color4 &c)
{
    material_ambient_ = c;
    material_diffuse_ = c;
}

void PresentationNode::set_material_specular(const Color4 &c) { material_specular_ = c; }

void PresentationNode::set_material_emission(const Color4 &c) { material_emission_ = c; }

void PresentationNode::set_material_shininess(float s) { material_shininess_ = s; }

void PresentationNode::draw(SceneState &scene_state)
{
    // Set the material uniform values
    glUniform4fv(scene_state.material_ambient_loc, 1, &material_ambient_.r);
    glUniform4fv(scene_state.material_diffuse_loc, 1, &material_diffuse_.r);
    glUniform4fv(scene_state.material_specular_loc, 1, &material_specular_.r);
    glUniform4fv(scene_state.material_emission_loc, 1, &material_emission_.r);
    glUniform1f(scene_state.material_shininess_loc, material_shininess_);

    // Draw children of this node
    SceneNode::draw(scene_state);
}

} // namespace cg
