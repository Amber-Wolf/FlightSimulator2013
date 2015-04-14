// Targeting version 1.5 of GLSL. If the GLSL compiler does not support 
// 1.5 or newer it will cause an error.
#version 150

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings (but can cause an error on some machines)
//precision mediump float;

struct Attenuation {

	float constant;
	float linear;
	float quadratic;
};


struct AmbientLight
{
    vec4 colorIntensity;
	
	bool enabled;
};

struct DirectionalLight
{
    vec4 colorIntensity;

	vec4 specularIntensity;

	vec3 direction;

	bool enabled;
};


struct PositionalLight
{
    vec4 colorIntensity;

	vec4 specularIntensity;

	vec3 position;

	Attenuation atten;

	bool enabled;
};

struct SpotLight
{
    vec4 colorIntensity;

	vec4 specularIntensity;

	vec3 position;

	vec3 spotDirection;

	float spotCutoffCos;

	Attenuation atten;

	bool enabled;
};

uniform AmbientLight ambLight;
uniform DirectionalLight directLight;
uniform PositionalLight posLight;
uniform SpotLight spLight;

// Structure for material properties
struct Material
{
	vec4 ambientMat;

	vec4 diffuseMat;

	vec4 specularMat;

	float specularExp;

	vec4 emissiveMat;

	bool textureMapped;
	
	bool toon;
	
	bool sky;
};


uniform Material object;

uniform vec3 worldEyePosition;

// Vertex attributes passed in by the vertex shader
in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;  

// Output color for the fragment (pixel)
out vec4 FragColor;

vec4 calcAmbientLight( AmbientLight ambLight, Material obj )
{
	vec4 totalAmbientLight = vec4(0.0f,0.0f,0.0f,0.0f);
	
	if(ambLight.enabled == true) {
 
		totalAmbientLight = obj.ambientMat * ambLight.colorIntensity; 
	}

	return totalAmbientLight;
}


float cel_shade(float intensity){
	if(intensity < 0.1f) return 0.0f;
	else if(intensity < 0.25f)
		return 0.3f;
	else if(intensity < 0.5f)
		return 0.7f;
	//else if(intensity < 0.8f)
		//return 0.75f;
	else
		return 0.9f;
}

vec4 calcDirectionalLight( DirectionalLight directLight, Material obj )
{
	vec4 totalDirectionalLight = vec4(0.0f,0.0f,0.0f,0.0f);

	 
	 if(directLight.enabled == true) {
 
		// Diffuse
		float DiffuseFactor = max(dot(Normal0, directLight.direction),0.0f);
		
		if(obj.toon == true){
			DiffuseFactor = cel_shade(DiffuseFactor);
		}
 
		vec4 diffuseTerm = DiffuseFactor * obj.diffuseMat * directLight.colorIntensity;

		 // Specular
		vec3 VertexToEye = normalize(worldEyePosition - WorldPos0);
 
		vec3 LightReflect = normalize(reflect(-directLight.direction, Normal0));
 
		float SpecularFactor = max(dot(VertexToEye, LightReflect), 0.0f);
 
		SpecularFactor = pow( SpecularFactor, object.specularExp);
		
		if(obj.toon == true){
			SpecularFactor = cel_shade(SpecularFactor);
		}
 
		vec4 specularTerm = SpecularFactor * obj.specularMat * directLight.specularIntensity;
 
		totalDirectionalLight = diffuseTerm + specularTerm;

	 }


	return totalDirectionalLight;

}

vec4 calcPositionalLight( PositionalLight posLight, Material obj )
{
	vec4 totalPositionalLight = vec4(0.0f,0.0f,0.0f,0.0f);

	 // Point Light
	if(posLight.enabled == true) {
 
	// Find a unit vector that points at the light source
	vec3 directionToPosLight = normalize( posLight.position - WorldPos0 );
 
	// Diffuse
	float DiffuseFactor = max(dot(Normal0, directionToPosLight), 0.0f);
	
	if(obj.toon)
			DiffuseFactor = cel_shade(DiffuseFactor);
	
	vec4 diffuseTerm = DiffuseFactor * obj.diffuseMat * posLight.colorIntensity;
 
	// Specular
	vec3 VertexToEye = normalize( worldEyePosition - WorldPos0);
	vec3 LightReflect = normalize( reflect(-directionToPosLight, Normal0) );
 
	float SpecularFactor = max( dot( VertexToEye, LightReflect ), 0.0f);
 
	SpecularFactor = pow(SpecularFactor, obj.specularExp);
	
	if(obj.toon)
			SpecularFactor = cel_shade(SpecularFactor);
 
	vec4 specularTerm = SpecularFactor * obj.specularMat * posLight.specularIntensity;
 
	totalPositionalLight = diffuseTerm + specularTerm; 
 
 }


	return totalPositionalLight;
}

vec4 calcSpotLight( SpotLight spLight, Material obj )
{
	vec4 totalSpotLight = vec4(0.0f,0.0f,0.0f,0.0f);
 
	 // SpotLight
	 if(spLight.enabled == true) {
 
		vec3 LightToPixel = normalize(WorldPos0 - spLight.position); 
 
		float spotCosFactor = dot(LightToPixel, spLight.spotDirection); 
 
		if( spotCosFactor > spLight.spotCutoffCos ) {
 
			vec3 directionToSpotLight = - LightToPixel;
 
			 // Diffuse
			 float DiffuseFactor = max(dot(Normal0, directionToSpotLight), 0.0f);

			 if(obj.toon)
				DiffuseFactor = cel_shade(DiffuseFactor);
			 
			 vec4 diffuseTerm = DiffuseFactor * obj.diffuseMat * spLight.colorIntensity;
 
			 // Specular
			 vec3 VertexToEye = normalize(worldEyePosition - WorldPos0);
			 vec3 LightReflect = normalize(reflect(-directionToSpotLight, Normal0));
			 float SpecularFactor = max(dot(VertexToEye, LightReflect), 0.0f);
 
			 SpecularFactor = pow(SpecularFactor, obj.specularExp);
			 
			 if(obj.toon)
				SpecularFactor = cel_shade(SpecularFactor);
 
			 vec4 specularTerm = SpecularFactor *  obj.specularMat * spLight.specularIntensity;

			 totalSpotLight = diffuseTerm + specularTerm;
 
			 // Traditional method for attenuating illumination
			 // near the edges of the cone
			 float falloff = pow( spotCosFactor, 100.0f );
 
			 // Alternative method for attenuating illumination
			 // near the edges of the cone
			 //float falloff = 1.0f - (1.0f - spotCosFactor) / (1.0f - spLight.spotCutoffCos);
 
			 totalSpotLight = falloff * totalSpotLight;
		}
	 }

	return totalSpotLight;
}



uniform sampler2D gSampler;


void main()
{

	if (object.textureMapped) {
		Material textured = object;

		textured.diffuseMat = texture2D(gSampler, TexCoord0.st);
		textured.ambientMat = texture2D(gSampler, TexCoord0.st);
		textured.specularMat = texture2D(gSampler, TexCoord0.st);

		if(object.sky){
			FragColor  = texture2D(gSampler, TexCoord0.st);
		}
		else{		
			FragColor = calcAmbientLight( ambLight, textured ) + 
						calcDirectionalLight( directLight, textured ) + 
						calcPositionalLight( posLight, textured ) + 
						calcSpotLight( spLight, textured ) + 
						object.emissiveMat;

			FragColor  *= texture2D(gSampler, TexCoord0.st);
		}
	}
	
	else{
		FragColor = calcAmbientLight( ambLight, object ) + 
					calcDirectionalLight( directLight, object ) + 
					calcPositionalLight( posLight, object ) + 
					calcSpotLight( spLight, object ) + 
					object.emissiveMat;
	}
	
}