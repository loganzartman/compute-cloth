const float damping = 2;

vec3 proj(vec3 a, vec3 b) {
    return b * dot(a,b) / pow(length(b), 2);
}

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

    vec3 sphere_pos = sphere[0].position;
    float sphere_radius = sphere[0].radius;
    float distance_to_sphere = distance(sphere_pos, vertex[index].position);
    if (distance_to_sphere < sphere_radius + vertex_sphere_radius) { // collision
        vec3 sphere_dir = normalize(vertex[index].position - sphere_pos);
        vec3 velocity = vertex[index].position - vertex[index].prev_pos;
        vec3 projection = proj(velocity, sphere_dir);
        velocity -= projection;
        vertex[index].position = sphere_pos +  sphere_dir * (sphere_radius + vertex_sphere_radius); 
        vertex[index].prev_pos = vertex[index].position - velocity;
    }
    
    float f = perlin3d(vec3(vertex[index].position.xy * 0.1, time)) + 0.5;
    vec3 temp = vertex[index].position;
    vertex[index].accel += vec3(0,-50,0);
    vertex[index].accel += proj(vec3(0,0,-100*f), vertex[index].normal);
    vertex[index].position += max(1-damping*time_step, 0) * (vertex[index].position - vertex[index].prev_pos) + vertex[index].accel * time_step * time_step;
    vertex[index].prev_pos = temp;
}
