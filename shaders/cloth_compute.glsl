layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

layout(std430) struct Vertex
{
    vec3 position;
    float _pad0;
    vec3 prev_pos;
    float _pad1;
};

layout(std430, binding=0) buffer VertexBlock
{
    Vertex vertex[];
};

uniform uvec2 cloth_dimension;
uniform float time;
uniform float time_step;

void main() {
    // give each compute invocation a unique ID.
    // it is assumed that the dimensions of gl_NumWorkGroups match the size of the cloth
    uint x_index = gl_WorkGroupID.x;
    uint y_index = gl_WorkGroupID.y;
    uint index = gl_NumWorkGroups.x * y_index + x_index;
    vec3 temp = vertex[index].position;
    vertex[index].position = 2.0 * vertex[index].position - vertex[index].prev_pos + vec3(0.0,-9.8,0.0) * time_step * time_step;
    vertex[index].prev_pos = temp;

  
}
