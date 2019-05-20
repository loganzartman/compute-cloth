layout(local_size_x=1, local_size_y=1, local_size_z=1) in;

const float sphere_mass = 10;
const float elasticity = 0.8;


void main() {
    uint stride = gl_WorkGroupSize.x;
    for (uint i_sphere = 0; i_sphere < sphere.length(); ++i_sphere) {
        int collisions = 0;
        vec3 sphere_accel = vec3(0);
        vec3 v_sphere = sphere[i_sphere].position - sphere[i_sphere].prev_pos;
        for (uint i_vert = gl_LocalInvocationID.x; i_vert < vertex.length(); i_vert += stride) {
            // collision handling
            vec3 disp = sphere[i_sphere].position - vertex[i_vert].position;
            float dist = length(disp);
            float sdist = sphere[i_sphere].radius + vertex_sphere_radius;
            if (dist < sdist) {
                collisions++;
                vec3 v_vert = vertex[i_vert].position - vertex[i_vert].prev_pos;
                vec3 h_disp = normalize(disp) * ((dist - sdist) / 2);
                vertex[i_vert].accel += v_sphere * sphere_mass + h_disp;
                sphere_accel += v_vert / sphere_mass - h_disp;
            }
        }

        // scale force
        memoryBarrier();
        if (gl_LocalInvocationID.x == 0 && collisions > 0) {
            for (uint i_vert = gl_LocalInvocationID.x; i_vert < vertex.length(); i_vert += stride) {
                vertex[i_vert].position += vertex[i_vert].accel / collisions * elasticity;
            }
            sphere[i_sphere].position += sphere_accel / collisions * elasticity;
        }
        memoryBarrier();
    }
}