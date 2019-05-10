in vec4 vs_vertex_pos;
in vec3 uvw;
out vec4 frag_color;

uniform vec3 camera_position;
uniform float time;

const float inf = 1e20;
const vec3 clouds_pos = vec3(0, 100, 0);
const float clouds_vis_dist = 1000;

vec3 line_plane_intersect(vec3 line_point, vec3 line_dir, vec3 plane_point, vec3 plane_normal) {
    // https://stackoverflow.com/a/52711312/1175802
    if (dot(plane_normal, line_dir) == 0) {
        return vec3(inf,inf,inf);
    }
    float t = (dot(plane_normal, plane_point) - dot(plane_normal, line_point)) / dot(plane_normal, line_dir);
    if (t < 0) {
        return vec3(inf,inf,inf);
    }
    return line_point + line_dir * t;
}

void main() {
    vec3 ray_direction = normalize(vec3(vs_vertex_pos) - camera_position);
    frag_color = vec4(ray_direction * 0.1 + 0.5, 1);
}
