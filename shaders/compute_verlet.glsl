layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

const float damping = 2;

void main() {
    // give each compute invocation a unique ID.
    // it is assumed that the dimensions of gl_NumWorkGroups match the size of the cloth
    uint x_index = gl_WorkGroupID.x;
    uint y_index = gl_WorkGroupID.y;
    uint index = cloth_dimension.x * y_index + x_index;

    // pin corners
    if (pinned(ivec2(gl_WorkGroupID.xy))) {
        return;
    }
    
    float f = perlin3d(vec3(vertex[index].position.xy * 0.1, time)) + 0.5;
    vec3 temp = vertex[index].position;
    vertex[index].accel += vec3(0,-50,0);
    // vertex[index].accel += proj(vec3(0,0,-100*f), vertex[index].normal);
    vertex[index].position += max(1-damping*time_step, 0) * (vertex[index].position - vertex[index].prev_pos) + vertex[index].accel * time_step * time_step;
    vertex[index].prev_pos = temp;
    vertex[index].accel = vec3(0);
}
