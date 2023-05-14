#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;      // Vertex position
layout(location = 1) in vec3 aNormal;   // Vertex normal
layout(location = 2) in vec2 aTexCoord; // Vertex texture coordinates

out vec3 v_pos;
out vec3 v_normal;
out vec2 v_texCoord;

uniform mat4 u_MVP;

void main()
{
	v_pos = aPos;
	v_normal = aNormal;
	v_texCoord = aTexCoord;
	gl_Position = u_MVP * vec4(aPos, 1.0f);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_texCoord;

uniform mat4 modelMatrix;
uniform mat4 worldMatrix;
uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform vec3 lightColors[1];
uniform vec3 lightDirs[1];
uniform bool isDirectional[1];
uniform float specularExponent;

void main()
{
	float specularStrength = 0.8;
	float diffuseStrength = 0.5;
	int shininess = 64;

	vec3 surfacePos = vec3(worldMatrix * modelMatrix * vec4(v_pos, 1.0));

	mat3 normalMatrix = transpose(inverse(mat3(mdlMatrix)));
	vec3 norm = mat3(wldMatrix) * normalMatrix * in_Normal;
	normalize(exNormal);

	vec3 result_color = vec3(0, 0, 0);

	for (int i = 0; i < 1; i++) {
		vec3 lightDir;
		if (isDirectional[i]) {
			lightDir = normalize(mat3(wldMatrix) * lightSourcesDirPosArr[i]);
		} else {
			vec3 lightPosition = vec3(wldMatrix * vec4(lightSourcesDirPosArr[i], 1.0));
			lightDir = normalize(lightPosition - exSurfacePos);
		}
		vec3 lightColor = lightSourcesColorArr[i];
		vec3 viewDir = normalize(-exSurfacePos);

		// Diffuse lighting
		float shade = max(dot(norm, lightDir), 0.0);
		result_color = result_color + lightColor * shade * diffuseStrength;

		// Specular lighting
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		result_color = result_color + lightColor * spec * specularStrength ;
	}

	color = vec4(result_color, 1.0); 
	//vec4 texColor = texture(u_Texture, v_texCoord);
	//color = texColor * u_Color;
};