#version 330 core
out vec4 FragColor;

in vec3  fragNormal;
in float fragBlockType;
in float fragAO;

// Matches BlockType enum: EMPTY=0, GRASS=1, STONE=2, GLASS=3, DIRT=4, WATER=5
vec4 palette[6] = vec4[6](
    vec4(0.00, 0.00, 0.00, 0.00),  // EMPTY  (unused)
    vec4(0.45, 0.72, 0.33, 1.00),  // GRASS
    vec4(0.50, 0.50, 0.50, 1.00),  // STONE
    vec4(0.50, 0.70, 0.90, 0.45),  // GLASS
    vec4(0.55, 0.40, 0.25, 1.00),  // DIRT
    vec4(0.15, 0.35, 0.75, 0.70)   // WATER
);

void main()
{
    int   idx       = clamp(int(round(fragBlockType)), 0, 5);
    vec4  entry     = palette[idx];

    vec3  lightDir  = normalize(vec3(0.8, 1.0, 0.6));
    float ambient   = 0.35;
    float diffuse   = max(dot(normalize(fragNormal), lightDir), 0.0) * 0.65;
    float lighting  = ambient + diffuse;

    // AO darkens (fragAO is 0..1 where 1=fully lit)
    float ao        = mix(0.5, 1.0, fragAO);

    FragColor = vec4(entry.rgb * lighting * ao, entry.a);
}
