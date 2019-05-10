layout (location = 0) in vec3 vertex_pos;
layout (location = 4) in vec3 accel;
layout (location = 6) in vec3 debug_color;

uniform mat4 projection;
uniform mat4 view;

out vec4 vs_vertex_pos;
out vec3 vs_accel;
out vec3 vs_debug_color;

void main() {
    gl_Position = vec4(vertex_pos, 1.);
    vs_vertex_pos = vec4(vertex_pos, 1.);
    vs_accel = accel;
    vs_debug_color = debug_color;
}
