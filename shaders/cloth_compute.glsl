layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

layout(std430, binding=0) buffer VertexBlock
{
    vec3 vertex_pos[];
};

void main() {
    // give each compute invocation a unique ID.
    // it is assumed that the dimensions of gl_NumWorkGroups match the size of the cloth
    uint index = gl_NumWorkGroups.x * gl_WorkGroupID.y + gl_WorkGroupID.x;

    vertex_pos[index].x += 0.01;
}
