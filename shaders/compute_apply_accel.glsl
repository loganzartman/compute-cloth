layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

void main() {
    ivec2 pos = ivec2(gl_WorkGroupID.xy);
    if (pinned(pos)) {
        return;
    }
    uint current = index(pos);
    vertex[current].position += vertex[current].accel;
    vertex[current].accel = vec3(0);
}