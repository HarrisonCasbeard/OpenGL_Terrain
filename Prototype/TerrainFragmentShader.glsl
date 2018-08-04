#version 330 core

in VS_OUT
{
	vec3 fragPos;
	vec2 UV;
	vec3 normal;
	vec4 fragPosLightSpace;

	float yPos;
	float fragMaxHeight;
} fsIn;

//Output data
out vec4 colour;

//Values that stay constant for the entire mesh
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D snowTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;

float shadowCalculation(vec4 fragPosLightSpace)
{
	//The depth map is in [0,1] as gl_Position does the perspective divide - since this isn't gl_position - DIY
	//Perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	//The depth map is in the range of 0,1 and projCoords is -1,1
	//so transform is 0,1
	projCoords = projCoords * 0.5 + 0.5;

	//So now the x,y coords are where the lightPos is in NDC
	//Query that to find it's depth
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	//Since we've done all the calc on the entire vec, the current Z represents the current depth
	float currentDepth = projCoords.z;

	// check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

	if(projCoords.z > 1.0f)
		shadow = 0.0f;

    return shadow;
}

void main()
{
	float fScale = fsIn.yPos / fsIn.fragMaxHeight;

	const float fRange1 = 0.30f;
	const float fRange2 = 0.50f;
	const float fRange3 = 0.70f;
	const float fRange4 = 0.85f;

	colour = vec4(0.0f);

	if(fScale >= 0.0f && fScale <= fRange1 )
	{
		colour = texture(grassTexture, fsIn.UV);
	}
	else if(fScale <= fRange2)
	{
		fScale -= fRange1;
		fScale /= (fRange2 - fRange1);

		float fScale2 = fScale;
		fScale = 1.0f - fScale;

		colour += texture(grassTexture, fsIn.UV) * fScale;
		colour += texture(rockTexture, fsIn.UV) * fScale2;
	}
	else if(fScale <= fRange3)
	{
		colour = texture(rockTexture, fsIn.UV);
	}
	else if(fScale <= fRange4)
	{
		fScale -= fRange3;
		fScale /= (fRange4 - fRange3);

		float fScale2 = fScale;
		fScale = 1.0f - fScale;

		colour += texture(rockTexture, fsIn.UV) * fScale;
		colour += texture(snowTexture, fsIn.UV) * fScale2;
	
	}
	else
	{
		colour = texture( snowTexture, fsIn.UV);
	}

	//colour = texture(grassTexture, fsIn.UV);
	vec3 normal = normalize(fsIn.normal);
	vec3 lightColour = vec3(0.5);

	//Ambient
	vec3 ambient = 0.05f * colour.rgb;

	//Diffuse
	vec3 lightDir = normalize(lightPos - fsIn.fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColour;

	float shadow = shadowCalculation(fsIn.fragPosLightSpace);
	vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * colour.rgb;

	colour = vec4(lighting, 1.0f);


}