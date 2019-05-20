layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

vec3 resolve_constraint(ivec2 pos, ivec2 neighbor, float constraint_len) {
    if (!in_bounds(neighbor)) {
        return vec3(0);
    }
    uint c = index(pos);
    uint n = index(neighbor);

    vec3 displacement = vertex[n].position - vertex[c].position;
    float dist = length(displacement);
    vec3 correction = normalize(displacement) * (dist - constraint_len);
    return correction * 0.1; // values higher than this seem to destabilize it; investigate
}

void main() {
    ivec2 pos = ivec2(gl_WorkGroupID.xy);
    uint current = index(pos);

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

    vertex[current].accel = vec3(0);
    for (int i = 0; i < offsets.length(); ++i) {
        ivec2 neighbor_pos = pos + offsets[i];
        float constraint_len = constraint_lens[i];
        vertex[current].accel += resolve_constraint(pos, neighbor_pos, constraint_len);
    }

    // compute per-vertex normals
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
}
