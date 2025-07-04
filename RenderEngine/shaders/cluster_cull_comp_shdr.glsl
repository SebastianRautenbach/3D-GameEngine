#version 460 core

#define LOCAL_SIZE 128
#define PI_DIV_4 0.7853975
layout(local_size_x = LOCAL_SIZE, local_size_y = 1, local_size_z = 1) in;

struct PointLight
{
    vec4  position;
    vec4  color;
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



layout(std430, binding = 1) buffer clusterSSBO
{
    Cluster clusters[];
};

layout(std430, binding = 2) buffer pointLightSSBO
{
    PointLight pointLight[];
};

layout(std430, binding = 3) buffer spotLightSSBO
{
    SpotLight spotLight[];
};

uniform mat4 viewMatrix;







bool sphereAABBIntersection(vec3 center, float radius, vec3 aabbMin, vec3 aabbMax) {
    vec3 closestPoint = clamp(center, aabbMin, aabbMax);
    float distanceSquared = dot(closestPoint - center, closestPoint - center);
    return distanceSquared <= (radius * radius);
}


bool test_spotlightAABB(uint i, Cluster cluster)
{

    float radius = spotLight[i].distance * tan(spotLight[i].outerCutOff);
    if (spotLight[i].outerCutOff <= PI_DIV_4) {
        radius = spotLight[i].distance * 0.5f / pow(cos(spotLight[i].outerCutOff), 2.0f);
    }

    radius += radius * 5;
 
    vec3 position = vec3(viewMatrix * spotLight[i].position);
 
    vec3 aabbMin = cluster.minPoint.xyz;
    vec3 aabbMax = cluster.maxPoint.xyz;
 
    return sphereAABBIntersection(position, radius, aabbMin, aabbMax);
}


bool test_pointlightAABB(uint i, Cluster cluster) {
    if (pointLight[i].radius == 0.0) return false;

    vec3 center = vec3(viewMatrix * pointLight[i].position);
    vec3 aabbMin = cluster.minPoint.xyz;
    vec3 aabbMax = cluster.maxPoint.xyz;

    return sphereAABBIntersection(center, 65, aabbMin, aabbMax);
}

void main() {
    uint pointlightCount = pointLight.length();
    uint spotlightCount = spotLight.length();

    uint index = gl_WorkGroupID.x * LOCAL_SIZE + gl_LocalInvocationID.x;

    if (index >= clusters.length()) return;

    Cluster cluster = clusters[index];
    cluster.point_count = 0;
    cluster.spot_count = 0;

    for (uint i = 0; i < pointlightCount; ++i) {
        if (cluster.point_count < 100 && test_pointlightAABB(i, cluster)) {
            cluster.pointLightIndices[cluster.point_count++] = i;
        }
    }
    
   for(uint i = 0; i < spotlightCount; ++i) {
       if(cluster.spot_count < 100 && test_spotlightAABB(i, cluster)) {
            cluster.spotLightIndices[cluster.spot_count++] = i;
       }
   }

    clusters[index] = cluster;
}