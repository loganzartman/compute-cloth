uint hash(uint a) {
    a = (a+uint(0x7ed55d16)) + (a<<12);
    a = (a^uint(0xc761c23c)) ^ (a>>19);
    a = (a+uint(0x165667b1)) + (a<<5);
    a = (a+uint(0xd3a2646c)) ^ (a<<9);
    a = (a+uint(0xfd7046c5)) + (a<<3);
    a = (a^uint(0xb55a4f09)) ^ (a>>16);
    return a;
}

vec3 directions[12] = vec3[](
    vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1), vec3(-1, 0, 1),
    vec3(0, 1, 1), vec3(0, 1, -1), vec3(0, -1, -1), vec3(0, -1, 1)
);

vec3 gradient_direction(vec3 grid_pos) {
    uint coord = hash(hash(hash(uint(grid_pos.x)) + uint(grid_pos.y)) + uint(grid_pos.z));
    return directions[coord % uint(12)]; 
}

float dot_grid_gradient(vec3 grid_pos, vec3 pos) {
    vec3 f = pos - vec3(grid_pos);
    vec3 gradients = gradient_direction(grid_pos);
    return dot(f, gradients);
}

vec3 fade(vec3 t) {
    return t * t * t * (t * (t * 6.f - 15.f) + 10.f);
}

float perlin3d(vec3 pos) {
    pos += 1000;
    vec3 cell_pos = floor(pos);
    vec3 weights = fade(pos - cell_pos);

    float n0 = dot_grid_gradient(cell_pos + vec3(0, 0, 0), pos);
    float n1 = dot_grid_gradient(cell_pos + vec3(1, 0, 0), pos);
    float ix0 = mix(n0, n1, weights.x);

    float n2 = dot_grid_gradient(cell_pos + vec3(0, 1, 0), pos);
    float n3 = dot_grid_gradient(cell_pos + vec3(1, 1, 0), pos);
    float ix1 = mix(n2, n3, weights.x);

    float n4 = dot_grid_gradient(cell_pos + vec3(0, 0, 1), pos);
    float n5 = dot_grid_gradient(cell_pos + vec3(1, 0, 1), pos);
    float ix2 = mix(n4, n5, weights.x);

    float n6 = dot_grid_gradient(cell_pos + vec3(0, 1, 1), pos);
    float n7 = dot_grid_gradient(cell_pos + vec3(1, 1, 1), pos);
    float ix3 = mix(n6, n7, weights.x);

    // interpolate the 4 interpolated values by y
    float ixy0 = mix(ix0, ix1, weights.y);
    float ixy1 = mix(ix2, ix3, weights.y);

    // interpolate the 2 remaining values by z
    return mix(ixy0, ixy1, weights.z);
}

float perlin3d(vec3 pos, int octaves, float persistence) {
    float sum = 0;
    float amplitude = 1;
    float frequency = 1;
    for (int i = 0; i < octaves; ++i) {
        sum += perlin3d(pos * frequency) * amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    return sum;
}