layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

const float damping = 0.2;
const float sphere_mass = 300;

void main() {
    uint current = gl_WorkGroupID.x;
    sphere[current].accel = vec3(0);

    // cloth collision
    vec3 velocity = sphere[current].position - sphere[current].prev_pos;
    vec3 net_force = vec3(0);
    uint collisions = 0;
    for (int i = 0; i < vertex.length(); ++i) {
        if (distance(vertex[i].position, sphere[current].position) < vertex_sphere_radius + sphere[current].radius) {
            ++collisions;
            vec3 sphere_dir = normalize(sphere[current].position - vertex[i].position);
            float overlap = distance(vertex[i].position, sphere[current].position) - (vertex_sphere_radius + sphere[current].radius);
            net_force += sphere_dir * overlap;
        }
    }
    if (collisions > 0) {
        sphere[current].position -= net_force / collisions;
        sphere[current].prev_pos = sphere[current].position - (velocity - net_force / sphere_mass);
    }

    // verlet integration
    vec3 temp = sphere[current].position;
    sphere[current].accel += vec3(0,-50,0);
    sphere[current].position += max(1-damping*time_step, 0) * (sphere[current].position - sphere[current].prev_pos) + sphere[current].accel * time_step * time_step;
    sphere[current].prev_pos = temp;
}