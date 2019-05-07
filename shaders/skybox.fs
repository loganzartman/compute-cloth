in vec4 vs_vertex_pos;
in vec3 uvw;

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
    gl_FragColor = vec4(0.6, 0.8, 1.0, 1.0);

    vec3 ray_direction = normalize(vec3(vs_vertex_pos) - camera_position);
    vec3 sky_pos = line_plane_intersect(camera_position, ray_direction, clouds_pos, vec3(0,-1,0));
    float dist = length(sky_pos - camera_position);
    if (dist < clouds_vis_dist) {
        float sample = perlin3d(sky_pos * 0.015 + vec3(time * 0.2, time * 0.05, 0), 2, 0.8);
        float p = max(0, sample * max(0, 1 - dist / clouds_vis_dist));
        gl_FragColor += vec4(p, p, p, 0);
    }
}
