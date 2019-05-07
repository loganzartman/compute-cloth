layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 view;
flat out vec4 normal;
out vec4 light_direction;
out vec4 vertex_pos;

out vec3 barycentric;
const vec3[] barycentric_vals = vec3[3](vec3(1,0,0), vec3(0,1,0), vec3(0,0,1));

void main() {
	int n = 0;
	vec3 ab = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 bc = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	normal = vec4(cross(ab, bc), 0.);
	for (n = 0; n < gl_in.length(); n++) {
		// light_direction = light_position - gl_in[n].gl_Position;
		light_direction = vec4(normalize(vec3(0, 1, 0)), 0);
		vertex_pos = gl_in[n].gl_Position;
		gl_Position = projection * view * gl_in[n].gl_Position;
		barycentric = barycentric_vals[n];
		EmitVertex();
	}
	EndPrimitive();
}
