const float damping = 1;

void main() {
    uint current = gl_WorkGroupID.x;
    sphere[current].accel = vec3(0);

    for (uint i = 0; i < sphere.length(); ++i) {
        // do something
    }

    vec3 temp = sphere[current].position;
    sphere[current].accel += vec3(0,-50,0);
    sphere[current].position += max(1-damping*time_step, 0) * (sphere[current].position - sphere[current].prev_pos) + sphere[current].accel * time_step * time_step;
    sphere[current].prev_pos = temp;
}