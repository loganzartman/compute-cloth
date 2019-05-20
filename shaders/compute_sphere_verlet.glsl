layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

const float damping = 0.2;

void main() {
    uint current = gl_WorkGroupID.x;

    // verlet integration
    vec3 temp = sphere[current].position;
    sphere[current].accel += vec3(0,-50,0);
    sphere[current].position += max(1-damping*time_step, 0) * (sphere[current].position - sphere[current].prev_pos) + sphere[current].accel * time_step * time_step;
    sphere[current].prev_pos = temp;
    sphere[current].accel = vec3(0);
}