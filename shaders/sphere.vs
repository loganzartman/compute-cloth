layout (location = 0) in vec3 vertex_pos;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 sphere_pos;
uniform float sphere_radius;

out vec3 vs_vertex_pos;

void main() {
    gl_Position = projection * view * vec4(vertex_pos * sphere_radius + sphere_pos, 1); 
    vs_vertex_pos = vertex_pos;
}