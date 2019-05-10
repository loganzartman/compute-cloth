layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

layout(std430) struct Vertex
{
    vec3 position;
    float _pad0;
    vec3 prev_pos;
    float _pad1;
    vec3 accel;
    float _pad2;
    vec3 debug_color;
    float _pad3;
};

layout(std430, binding=0) buffer VertexBlock
{
    Vertex vertex[];
};

const float damping = 0.01;

uniform uvec2 cloth_dimension;
uniform float time;
uniform float time_step;

void main() {
    // give each compute invocation a unique ID.
    // it is assumed that the dimensions of gl_NumWorkGroups match the size of the cloth
    uint x_index = gl_WorkGroupID.x;
    uint y_index = gl_WorkGroupID.y;
    uint index = cloth_dimension.x * y_index + x_index;

    // pin corners
    if (y_index == cloth_dimension.y - 1 && (x_index == 0 || x_index == cloth_dimension.x-1))
        return;
    
    // verlet integration
    vec3 temp = vertex[index].position;
    vertex[index].accel += vec3(0,-5,0);
    vertex[index].position += (1-damping) * (vertex[index].position - vertex[index].prev_pos) + vertex[index].accel * time_step * time_step;
    vertex[index].prev_pos = temp;
}
