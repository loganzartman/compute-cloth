layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

struct Vertex
{
    vec3 position;
    float _pad0;
    vec3 prev_pos;
    float _pad1;
    vec3 accel;
    float _pad2;
    vec3 debug_color;
    float _pad3;
    vec3 normal;
    float _pad4;
};

layout(std430, binding=0) buffer VertexBlock
{
    Vertex vertex[];
};

uniform uvec2 cloth_dimension;
uniform float time;
uniform float time_step;
uniform vec3 sphere_pos;

const float vertex_sphere_radius = 0.1;
const float sphere_radius = 5.0;

uint index(ivec2 pos) {
    return pos.y * cloth_dimension.x + pos.x;
}

bool in_bounds(ivec2 pos) {
    return pos.x >= 0 && pos.y >= 0 && pos.x < cloth_dimension.x && pos.y < cloth_dimension.y;
}

bool pinned(ivec2 pos) {
    return pos == ivec2(0,0) || pos == ivec2(cloth_dimension.x-1,0) || pos == ivec2(0,cloth_dimension.y-1) || pos == ivec2(cloth_dimension.x-1,cloth_dimension.y-1);
}