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

    const ivec2 offsets[] = ivec2[](
        // 3x3 neighborhood
        ivec2(-1,-1), ivec2(-1,0), ivec2(-1,1),
        ivec2(0,-1), ivec2(0,1),
        ivec2(1,-1), ivec2(1,0), ivec2(1,1),

        // bending constraints
        ivec2(-2,0), ivec2(0,-2), ivec2(2,0), ivec2(0,2)
    );
    const float len = 1.0;
    const float len_diag = len * sqrt(2.0);
    const float len_bend = len * 2.0;
    const float constraint_lens[] = float[](
        len_diag, len, len_diag,
        len, len,
        len_diag, len, len_diag

        ,len_bend, len_bend, len_bend, len_bend
    );

    for (int i = 0; i < offsets.length(); ++i) {
        ivec2 neighbor_pos = pos + offsets[i];
        float constraint_len = constraint_lens[i];
        force += compute_force(pos, neighbor_pos, constraint_len);
    }

    if (pos.x < cloth_dimension.x-1 && pos.y < cloth_dimension.y-1) {
        vec3 first = vertex[index(ivec2(pos.x, pos.y+1))].position - vertex[current].position;
        vec3 second = vertex[index(ivec2(pos.x+1, pos.y))].position - vertex[current].position;

        vertex[current].normal = cross(first, second);
    }

   else if (pos.x == cloth_dimension.x - 1 && pos.y == cloth_dimension.y -1 ) {
       uint new_ind = index(ivec2(pos.x-1, pos.y-1));

        vec3 first = vertex[index(ivec2(pos.x-1, pos.y))].position - vertex[new_ind].position;
        vec3 second = vertex[index(ivec2(pos.x, pos.y-1))].position - vertex[new_ind].position;

        vertex[current].normal = cross(first, second);
    }

   else if (pos.x == cloth_dimension.x - 1 ) {

       uint new_ind = index(ivec2(pos.x-1, pos.y));
        vec3 first = vertex[index(ivec2(pos.x-1, pos.y+1))].position - vertex[new_ind].position;
        vec3 second = vertex[index(ivec2(pos.x, pos.y))].position - vertex[new_ind].position;

        vertex[current].normal = cross(first, second);
    }

   else if (pos.y == cloth_dimension.y - 1 ) {
       uint new_ind = index(ivec2(pos.x, pos.y-1));
        vec3 first = vertex[index(ivec2(pos.x, pos.y))].position - vertex[new_ind].position;
        vec3 second = vertex[index(ivec2(pos.x+1, pos.y-1))].position - vertex[new_ind].position;

        vertex[current].normal = cross(first, second);
    }

    vertex[current].accel = force;
}
