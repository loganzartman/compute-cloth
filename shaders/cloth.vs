layout (location = 0) in vec3 vertex_pos;

uniform mat4 projection;
uniform mat4 view;

out vec4 vs_vertex_pos;

void main() {
    gl_Position = projection * view * vec4(vertex_pos, 1.);
    vs_vertex_pos = vec4(vertex_pos, 1.);
}
