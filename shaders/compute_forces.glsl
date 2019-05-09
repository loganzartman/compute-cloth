layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

layout(std430) struct Vertex
{
    vec3 position;
    float _pad0;
    vec3 prev_pos;
    float _pad1;
    vec3 accel;
    float _pad2;
};

layout(std430, binding=0) buffer VertexBlock
{
    Vertex vertex[];
};

uniform uvec2 cloth_dimension;
uniform float time;
uniform float time_step;

void main() {
    // give each compute invocation a unique ID.
    // it is assumed that the dimensions of gl_NumWorkGroups match the size of the cloth
    uint x_index = gl_WorkGroupID.x;
    uint y_index = gl_WorkGroupID.y;
    uint index = gl_NumWorkGroups.x * y_index + x_index;

    const float len = 1.0;
    const float diagonal_len = sqrt(2.0) * len;
    const float k = 0.5;
    vec3 force = vec3(0.0);
    for(int i=-1; i<=1; i++) {
        for(int j = -1; j<=1; j++) {
            if (i == 0 && j == 0)
                continue;
            if (x_index + i < 0 || y_index + j < 0 || x_index + i >= cloth_dimension.x || y_index + j >= cloth_dimension.y)
                continue;
            uint neighbor = gl_NumWorkGroups.x * (y_index + j) + x_index + i;
            float dist = distance(vertex[neighbor].position, vertex[index].position);
            vec3 direction = normalize(vertex[neighbor].position - vertex[index].position);
            float x;
            if (i != 0 && j != 0) // diagonal neighbor
                x = dist - diagonal_len; 
            else
                x = dist - len;
            
            force += k*x*direction;
        }
    }


    vertex[index].accel = force;
  
}
