float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 n) {
	const vec2 d = vec2(0.0, 1.0);
    vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

float noise(vec2 pos, int octaves, float persistence) {
    float sum = 0;
    float amplitude = 1;
    float frequency = 1;
    for (int i = 0; i < octaves; ++i) {
        sum += noise(pos * frequency) * amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    return sum;
}