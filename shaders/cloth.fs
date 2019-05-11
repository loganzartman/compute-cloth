in vec4 vertex_pos;
in vec3 light_direction;
in vec3 gs_accel;
in vec3 gs_debug_color;
in vec3 barycentric;
in vec3 normal;
in vec3 gs_view_direction;

uniform bool enable_specular;

out vec4 frag_color;

const float PI = 3.14159;

// cloth shading
// https://knarkowicz.wordpress.com/2018/01/04/cloth-shading/
float L(float x, float r) {
	r = clamp(r, 0, 1);
	r = 1.0 - (1. - r) * (1. - r);

	float a = mix( 25.3245,  21.5473, r);
	float b = mix( 3.32435,  3.82987, r);
	float c = mix( 0.16801,  0.19823, r);
	float d = mix(-1.27393, -1.97760, r);
	float e = mix(-4.85967, -4.32054, r);

	return a / (1. + b * pow(x, c)) + d * x + e;
}
float CharlieD(float roughness, float ndoth) {
    float invR = 1. / roughness;
    float cos2h = ndoth * ndoth;
    float sin2h = 1. - cos2h;
    return (2. + invR) * pow(sin2h, invR * .5) / (2. * PI);
}
float AshikhminV(float ndotv, float ndotl) {
    return 1. / (4. * (ndotl + ndotv - ndotl * ndotv));
}
float CharlieV(float roughness, float ndotv, float ndotl) {
	float visV = ndotv < .5 ? exp(L(ndotv, roughness)) : exp(2. * L(.5, roughness) - L(1. - ndotv, roughness));
	float visL = ndotl < .5 ? exp(L(ndotl, roughness)) : exp(2. * L(.5, roughness) - L(1. - ndotl, roughness));

	return 1. / ((1. + visV + visL) * (4. * ndotv * ndotl));
}
vec3 FresnelTerm(vec3 specularColor, float vdoth) {
	vec3 fresnel = specularColor + (1. - specularColor) * pow((1. - vdoth), 5.);
	return fresnel;
}

void main() {
    vec3 real_normal = gl_FrontFacing ? normal : -normal;
    float n_dot_l = clamp(dot(real_normal, light_direction.xyz), 0., 1.);

    vec3 color = vec3(0.72,0.19,0.13);
    vec3 specular_color = vec3(1,1,1);
    float roughness = 0.3;

    vec3 diffuse = vec3(max(0, n_dot_l)) * color;
    vec3 ambient = vec3(0.6,0.6,0.6) * color;
    
    // specular
    vec3 specular = vec3(0);
    if (enable_specular) {
        vec3 half_vec = normalize(-gs_view_direction + light_direction.xyz);
        vec3 fresnel = FresnelTerm(color, clamp(dot(-gs_view_direction, half_vec), 0., 1.));
        float d = CharlieD(roughness, clamp(dot(real_normal, half_vec), 0., 1.));
        float v = AshikhminV(clamp(dot(real_normal, -gs_view_direction), 0., 1.), clamp(n_dot_l, 0., 1.));
        specular = specular_color * fresnel * d * v * PI * n_dot_l;
    }
    
    frag_color = vec4((ambient + diffuse + specular), 1);
    // frag_color = vec4(real_normal * 0.5 + 0.5, 1);
    // frag_color = vec4(gs_debug_color, 1);
}
