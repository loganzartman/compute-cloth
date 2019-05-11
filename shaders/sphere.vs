layout (location = 0) in vec3 vertex_pos;
layout (location = 1) in vec3 position;
layout (location = 3) in float radius;

uniform mat4 projection;
uniform mat4 view;
out vec3 vs_vertex_pos;

void main() {
    gl_Position = projection * view * vec4(vertex_pos * radius + position, 1); 
    vs_vertex_pos = vertex_pos;
}