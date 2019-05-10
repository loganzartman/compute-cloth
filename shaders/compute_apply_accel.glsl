void main() {
    ivec2 pos = ivec2(gl_WorkGroupID.xy);
    if (pinned(pos)) {
        return;
    }
    uint current = index(pos);
    vertex[current].position += vertex[current].accel;
    vertex[current].accel = vec3(0);
}