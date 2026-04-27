#version 330 core
out vec4 FragColor;

in vec3 fragNormal;

void main()
{
    vec3 lightDir = normalize(vec3(0.8, 1.0, 0.6));
    float ambient = 0.4;
    float diffuse = max(dot(normalize(fragNormal), lightDir), 0.0) * 0.6;
    vec3 baseColor = vec3(0.45, 0.72, 0.33);
    FragColor = vec4(baseColor * (ambient + diffuse), 1.0);
}
