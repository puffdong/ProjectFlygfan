#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;      // Vertex position
layout(location = 1) in vec3 aNormal;   // Vertex normal
layout(location = 2) in vec2 aTexCoord; // Vertex texture coordinates

uniform mat4 u_MVP;
uniform mat4 modelMatrix;
uniform mat4 worldMatrix;
uniform mat4 proj;
uniform vec3 playerPosition;


uniform sampler2D perlinNoise; // The Perlin noise texture
uniform float time; // increment this in your application

out vec2 TexCoord;
out vec3 v_pos;
out vec3 newPos;


void main()
{
    // Sample the Perlin noise texture
    // Sample the Perlin noise texture, using time to animate the noise
    vec2 repeatTexCoord = fract(aTexCoord + vec2(time));
    float noise = texture(perlinNoise, repeatTexCoord).r * 5;

    // Scale and bias the noise value so it ranges from -0.5 to 0.5 instead of 0 to 1
    noise = noise * 2.0 - 1.0;

    // Displace the y position by the noise value
    float newY = aPos.y + noise * 0.1;

    vec3 newPosition = vec3(aPos.x, newY, aPos.z);

   
    newPos = newPosition; //+ draggedPosition;
    // Compute the normal of the surface
    
    gl_Position = u_MVP * vec4(newPosition, 1.0);

    TexCoord = aTexCoord;

    v_pos = aPos;
}

#shader fragment
#version 330 core

in vec2 TexCoord;
in vec3 v_pos;
in vec3 newPos;
//uniform sampler2D waterTexture;

uniform mat4 u_MVP;
uniform mat4 modelMatrix;
uniform mat4 worldMatrix;

uniform vec3 sunDir;
uniform vec3 sunColor;

layout(location = 0) out vec4 color;

void main()
{
    vec3 dPosition_dx = dFdx(newPos);
    vec3 dPosition_dy = dFdy(newPos);
    vec3 n = normalize(cross(dPosition_dx, dPosition_dy));
    vec3 Normal = mat3(worldMatrix) * transpose(inverse(mat3(modelMatrix))) * n;
    vec3 norm = normalize(Normal);

    vec3 surfacePos = vec3(worldMatrix * modelMatrix * vec4(v_pos, 1.0));
    vec3 viewDir = normalize(-surfacePos);

    vec3 r_color = vec3(0.0, 0.0, 0.0);
    vec3 lightDir = normalize(mat3(worldMatrix) * sunDir);

    // Diffuse lighting
    float shade = max(dot(norm, lightDir), 0.0);
    r_color = r_color + sunColor * shade * 0.5;

    // Specular lighting
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    r_color = r_color + sunColor * spec * 0.8;

    color = vec4(r_color, 0.5) * vec4(0.2, 0.2, 0.9, 1.0); //texture(waterTexture, TexCoord);
}