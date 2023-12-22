#version 330

// float AMBIENT = 0.1;

const int MAX_LIGHTS = 4;

struct Light {
    bool enabled;
    int type;
    vec4 ambient, diffuse, specular;
    vec3 direction;
    float angle;
    vec3 position;
};

// uniform vec3 lightPositions[4];
// uniform vec3 lightColors[4];
// uniform vec3 spotlightConeDir;
// uniform vec3 spotlightPhi;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float alpha;
};

// material parameters
// uniform vec3  albedo;
// uniform float metallic;
// uniform float roughness;
// uniform float ao;

// material samplers
// uniform sampler2D albedoMap;
// uniform sampler2D normalMap;
// uniform sampler2D metallicMap;
// uniform sampler2D roughnessMap;
// uniform sampler2D aoMap;

uniform float exposition;

uniform Lights { Light lights[MAX_LIGHTS]; };
uniform vec3 cameraPos;
uniform vec3 color;

in vec3 vecNormal;
in vec3 worldPos;
in vec3 vertCoords;

out vec4 outColor;

const float PI = 3.14159265359;
  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 phongLight(vec3 lightDir, vec3 lightColor, vec3 normal,vec3 viewDir){
	float diffuse=max(0,dot(normal,lightDir));

	vec3 R = reflect(-lightDir, normal);  

	float specular = pow(max(dot(viewDir, R), 0.0), 32);

	vec3 resultColor = color*diffuse*lightColor+lightColor*specular;
	return resultColor;
}

void main()
{		
    vec3 albedo = pow(texture(albedoMap, vertCoords).rgb, 2.2);
    // vec3 normal = getNormalFromNormalMap();
    // float roughness = texture(metallicMap, vertCoords).r;
    // float roughness = texture(roughnessMap, vertCoords).r;
    // float roughness = texture(aoMap, vertCoords).r;
    
    vec4 texColor = texture(albedoMap, vertCoords);

    vec3 N = normalize(vecNormal);
    vec3 V = normalize(cameraPos - worldPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
	           
    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < MAX_LIGHTS; ++i) 
    {
        if (lights[i].enabled) {
            // handle directional spot and point lights

            // calculate per-light radiance
            vec3 L = normalize(lights[i] - worldPos);
            vec3 H = normalize(V + L);
            float distance    = length(lights[i] - worldPos);
            float attenuation = 1.0 / (distance * distance);
            vec3 radiance     = lightColors[i] * attenuation;        
            
            // cook-torrance brdf
            float NDF = DistributionGGX(N, H, roughness);        
            float G   = GeometrySmith(N, V, L, roughness);      
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
            
            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;
            kD *= 1.0 - metallic;	  
            
            vec3 numerator    = NDF * G * F;
            float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
            vec3 specular     = numerator / denominator;  
                
            // add to outgoing radiance Lo
            float NdotL = max(dot(N, L), 0.0);                
            Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
        }
    }   
  
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    outColor = vec4(color, 1.0);
}  