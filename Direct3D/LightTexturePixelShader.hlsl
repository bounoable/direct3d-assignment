Texture2D mainTexture;
sampler mainSampler;

struct Light
{
    int type;
    float intensity;
    float radius;
    float angle;
    float3 position;
    float padding;
    float3 direction;
    float padding2;
    float4 color;
};

cbuffer LightBuffer
{
    Light lights[12];
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normal : NORMAL;
    float3 uv : TEXCOORD;
    float3 view : TEXCOORD1;
};

float4 calculateDirectionalDiffuse(Light light, PixelInput IN)
{
    float3 nLightDirection = normalize(-light.direction);
    float diffuse = dot(IN.normal, nLightDirection);

    if (diffuse > 0.0f)
    {
        return saturate(light.color * diffuse * light.intensity);
    }

    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 calculateSpotDiffuse(Light light, PixelInput IN)
{
    float3 lightDirection = light.position - IN.positionW;
    float lightDirectionLength = length(lightDirection);

    if (lightDirectionLength <= light.radius)
    {
        float3 lightVec = lightDirection / lightDirectionLength;
        float diffuse = dot(IN.normal, lightVec);

        float angle = max(dot(-lightVec, normalize(light.direction)), 0.0f);

        float lightAttenuation = 1.0f / (1.0f + 0.2f * lightDirectionLength + 0.1f * lightDirectionLength * lightDirectionLength) * pow(angle, light.angle);

        if (diffuse > 0.0f)
        {
            return saturate(light.color * diffuse * light.intensity * lightAttenuation);
        }
    }

    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 calculateSpotSpecular(Light light, PixelInput IN)
{
    float3 lightDirection = light.position - IN.positionW;
    float lightDirectionLength = length(lightDirection);

    if (lightDirectionLength <= light.radius)
    {
        float3 lightVec = lightDirection / lightDirectionLength;
        float diffuse = dot(IN.normal, lightVec);

        float angle = max(dot(-lightVec, normalize(light.direction)), 0.0f);

        float lightAttenuation = 1.0f / (1.0f + 0.2f * lightDirectionLength + 0.1f * lightDirectionLength * lightDirectionLength) * pow(angle, light.angle);

        float specular = pow(dot(IN.normal, (IN.view + lightVec) * 0.5f), 256.0f);
        return saturate(light.color * specular * light.intensity * lightAttenuation);
    }

    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 calculatePointDiffuse(Light light, PixelInput IN)
{
    float3 lightDirection = IN.positionW - light.position;
    float lightDirectionLength = length(lightDirection);

    if (lightDirectionLength <= light.radius)
    {
        float3 lightVec = -lightDirection / lightDirectionLength;
        float diffuse = dot(IN.normal, lightVec);
        float lightAttenuation = 1.0f / (1.0f + 0.2f * lightDirectionLength + 0.1f * lightDirectionLength * lightDirectionLength);

        if (diffuse > 0.0f)
        {
            return saturate(light.color * diffuse * light.intensity);
        }
    }

    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 calculatePointSpecular(Light light, PixelInput IN)
{
    float3 lightDirection = IN.positionW - light.position;
    float lightDirectionLength = length(lightDirection);

    if (lightDirectionLength <= light.radius)
    {
        float3 lightVec = -lightDirection / lightDirectionLength;
        float diffuse = dot(IN.normal, lightVec);
        float lightAttenuation = 1.0f / (1.0f + 0.2f * lightDirectionLength + 0.1f * lightDirectionLength * lightDirectionLength);
    
        if (diffuse > 0.0f)
        {
            float specular = pow(dot(IN.normal, (IN.view + lightVec) * 0.5f), 256.0f);
            return saturate(light.color * specular * light.intensity * lightAttenuation);
        }
    }

    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 main(PixelInput IN) : SV_TARGET
{
    IN.normal = normalize(IN.normal);
    IN.view = normalize(IN.view);

    float4 tex = mainTexture.Sample(mainSampler, IN.uv.xy);

    float4 diffuseColor = 0;
    float4 specularColor = 0;

    for (int x = 0; x < 12; x++)
    {
        Light light = lights[x];

        switch (light.type)
        {
            case 1:
                diffuseColor += calculateDirectionalDiffuse(light, IN);
                break;
            case 2:
                diffuseColor += calculateSpotDiffuse(light, IN);
                specularColor += calculateSpotSpecular(light, IN);
                break;
            case 3:
                diffuseColor += calculatePointDiffuse(light, IN);
                specularColor += calculatePointSpecular(light, IN);
                break;
            default:
                break;
        }
    }

    // tex * (ambient + diffuse) + specular + emission
    return saturate(saturate(tex * saturate(float4(0.05f, 0.05f, 0.05f, 1.0f) + diffuseColor)) + specularColor + float4(0.0f, 0.0f, 0.0f, 1.0f)) * IN.uv.z;
}
