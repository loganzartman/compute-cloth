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

const float stiffness = 2500.0;

layout(std430, binding=0) buffer VertexBlock
{
    Vertex vertex[];
};

uniform uvec2 cloth_dimension;
uniform float time;
uniform float time_step;

uint index(ivec2 pos) {
    return pos.y * cloth_dimension.x + pos.x;
}

bool in_bounds(ivec2 pos) {
    return pos.x >= 0 && pos.y >= 0 && pos.x < cloth_dimension.x && pos.y < cloth_dimension.y;
}

vec3 compute_force(ivec2 pos, ivec2 neighbor, float constraint_len) {
    if (!in_bounds(neighbor)) {
        return vec3(0);
    }
    uint c = index(pos);
    uint n = index(neighbor);
    
    float dist = distance(vertex[c].position, vertex[n].position);
    vec3 offset = normalize(vertex[n].position - vertex[c].position);
    return offset * (dist - constraint_len) * stiffness;
}

void main() {
    ivec2 pos = ivec2(gl_WorkGroupID.xy);
    uint current = index(pos);

    vec3 force = vec3(0);
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) {
                continue;
            }
            ivec2 neighbor_pos = pos + ivec2(dx, dy);

            float constraint_len = 1.0;
            if (dx != 0 && dy != 0)
                constraint_len *= sqrt(2.0);
            
            force += compute_force(pos, neighbor_pos, constraint_len);
        }
    }

    vertex[current].accel = force;
}
