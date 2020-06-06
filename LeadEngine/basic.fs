#version 330 core

out vec4 fragColour;

in vec2 texCoord;

in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCamVector;

uniform sampler2D texSampler;
uniform vec3 colour = vec3(1.0);

uniform vec3 lightColour;
uniform float ambientLight = 1;
uniform float shineDamper = 1;
uniform float reflectivity = 0;

void main()
{
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(toLightVector);

    float cosTheta = dot(unitNormal, unitLightVector);
    float brightness = max(cosTheta, ambientLight);
    vec3 diffuse = brightness * lightColour;

    vec3 unitCamVector = normalize(toCamVector); 
    vec3 lightDir = -unitLightVector;
    vec3 reflectedLightDir = reflect(lightDir, unitNormal);

    float specularFactor = dot(reflectedLightDir, unitCamVector);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, shineDamper);
    vec3 finalSpecular = dampedFactor * reflectivity * lightColour;

    vec4 texColour = texture(texSampler, texCoord) * vec4(colour, 1.0);
    if (texColour.a < 0.5)
    {
       discard;
    }
    fragColour = vec4(diffuse, 1.0) * texColour + vec4(finalSpecular, 1.0);
}