#version 460 core
out vec4 FragColor;

// MATERIAL SETUP ------------------------------------------------------------

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec4 position;
    vec4 color;
    float intensity;
    float radius;
};

struct SpotLight
{
	vec4 position;      
    vec4 direction;    
    vec4 ambient;      
    vec4 diffuse;       
    vec4 specular;      
    
    float cutOff;       
    float outerCutOff;
    float distance;     
    float constant;    

    float linear;      
    float quadratic;
    uint pad1;
    uint pad2;
};


struct Cluster
{
    vec4 minPoint;
    vec4 maxPoint;
    uint point_count;
    uint spot_count;
    uint pad1;
    uint pad2;
    uint pointLightIndices[100];
    uint spotLightIndices[100];
};


layout(std430, binding = 1) readonly buffer clusterSSBO
{
    Cluster clusters[];
};

layout(std430, binding = 2) readonly buffer pointLightSSBO
{
    PointLight pointLight[];
};

layout(std430, binding = 3) readonly buffer spotLightSSBO
{
    SpotLight spotLight[];
};


// UNIFORMS ------------------------------------------------------------------

uniform DirLight dirLight;
uniform Material material;

uniform float zNear;
uniform float zFar;
uniform uvec3 gridSize;
uniform uvec2 screenDimensions;
uniform vec3 camPos;
uniform sampler2D shadowMap;




in vec3 FragPos;
in vec3 Normal; 
in vec2 oTexture;
in vec4 FragPosLightSpace;
float gamma = 2.2;





vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);



void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    uint zTile = uint((log(max(abs(FragPos.z), 0.001) / zNear) * gridSize.z) / log(zFar / zNear));


    vec2 tileSize = screenDimensions / gridSize.xy;
    uvec3 tile = uvec3(gl_FragCoord.xy / tileSize, zTile);
    uint tileIndex = tile.x + (tile.y * gridSize.x) + (tile.z * gridSize.x * gridSize.y);

    uint pointlightCount = clusters[tileIndex].point_count;
    uint spotLightCount = clusters[tileIndex].spot_count;


    for (int i = 0; i < pointlightCount; ++i)
    {
        uint lightIndex = clusters[tileIndex].pointLightIndices[i];
        PointLight light = pointLight[lightIndex];
        result += CalcPointLight(light, norm, FragPos, viewDir); 
    }
    
    for(int i = 0; i < spotLightCount; ++i) 
    {
        uint lightIndex = clusters[tileIndex].spotLightIndices[i];
        SpotLight light = spotLight[lightIndex];
        result += CalcSpotLight(light, norm, FragPos, viewDir); 
    }


    FragColor = vec4(pow(result , vec3(1.0/gamma)), 1.0);
}



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, oTexture));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, oTexture));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, oTexture));
    
    float shadow = ShadowCalculation(FragPosLightSpace);


    return (ambient + (1.0 - shadow) * (diffuse + specular)); 
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    float distance = length(light.position.xyz - fragPos);

    float attenuation = 1.0 / (1.0 + (distance / light.radius) * (distance / light.radius));

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = texture(material.diffuse, oTexture).rgb;
    vec3 diffuse = diffuseColor * light.color.rgb * diff * light.intensity * attenuation;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularColor = texture(material.specular, oTexture).rgb;
    vec3 specular = specularColor * light.color.rgb * spec * light.intensity * attenuation;

    vec3 ambient = material.ambient * diffuseColor * light.color.rgb;
    vec3 result = ambient + diffuse + specular;
    return result;

}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    float theta = dot(lightDir, normalize(-light.direction.xyz));

    
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    
    float diff = max(dot(normal, lightDir), 0.0);
    
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    
    vec3 ambient = light.ambient.xyz * vec3(texture(material.diffuse, oTexture));
    vec3 diffuse = light.diffuse.xyz * diff * vec3(texture(material.diffuse, oTexture));
    vec3 specular = light.specular.xyz * spec * vec3(texture(material.specular, oTexture));
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}


float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    return shadow;
}
