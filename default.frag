#version 330 core

// Output colors in RGBA format
out vec4 FragColor;

// Flat color if not texture
in vec3 color;
// Spot to place textures
in vec2 texCoord;
// The normal vector for which we orient
in vec3 Normal;
// Position of pixel being shaded
in vec3 crntPos;

// Grab texture
uniform sampler2D tex0;
// Grab texture's specular map
uniform sampler2D tex1;
// The color of the light being emitted
uniform vec4 lightColor;
// The place of origin for the light
uniform vec3 lightPos;
// The position of the user for lights
uniform vec3 camPos;

uniform vec3 playerOrient;

// Glow light from all directions
vec4 glowLight()
{

	// Formula for varying intensity of light source
	vec3 lightVec = lightPos - crntPos;
	float dist = length(lightVec);
	float a = 2.0f;
	float b = 0.7f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);


	// Our ambient (world) min lighting (0 : pitch black room)
	float ambient = 0.5f;

	// Normalize the normal to make sure is in range
	vec3 normal = normalize(Normal);

	// Normalize to range of one
	vec3 lightDirection = normalize(lightVec);

	// The lights diffusion / glow on a textures pixel will be based on its normal vector and the direction of the beam of light
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	// The vibrancy of the specular map
	float specularLight = 0.5f;

	// Get the players sight of a object
	vec3 viewDirection = normalize(camPos - crntPos);

	// Get the bounced lights direction
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	// The specular of a textures pixel is based on the view direction and reflection direction
	float specAmnt = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);

	// Amount of specular is based on vibrancy and its amount based on camera origin
	float specular = specAmnt * specularLight;

	// Our base texture will be shined by the base light color at a intensity based on the diffusal of the lights intensity and the intensity of light
	return	(texture(tex0, texCoord) * lightColor * (diffuse * inten + ambient) + texture(tex1, texCoord).r * specular * inten) * lightColor;

}

vec4 spotLight()
{

	// Cones which will shine the light beam out of
	float outerCone = 0.80f;
	float innerCone = 0.85f;


	float ambient = 0.12f;

	vec3 normal = normalize(Normal);

	vec3 lightDirection = normalize(lightPos - crntPos);

	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	float specularLight = 0.5f;

	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	float specAmnt = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 2);

	float specular = specAmnt * specularLight;

	float angle = dot(playerOrient, -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 0.85f);

	return (texture(tex0, texCoord) * lightColor * (diffuse * inten + ambient) + texture(tex1, texCoord).r * specular * inten) * lightColor;

}

float near = 0.1f;
float far = 1000.0f;

float linearizeDepth(float depth)
{

	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));

}

float logisticDepth(float depth, float steepness = 0.5f, float offset = 5.0f)
{

	float zVal = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zVal - offset))));

}

void main()
{

	// Color :)
	float depth = logisticDepth(gl_FragCoord.z);
	FragColor = glowLight();
}