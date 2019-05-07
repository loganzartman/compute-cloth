layout (location = 0) in vec3 vertex_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec4 vs_vertex_pos;
out vec3 uvw;

void main() {
    gl_Position = projection * view * transform * vec4(vertex_pos, 1.);
    vs_vertex_pos = transform * vec4(vertex_pos, 1.);
    uvw = vertex_pos;
}
