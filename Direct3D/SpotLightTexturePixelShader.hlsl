Texture2D mainTexture;
sampler mainSampler;

cbuffer LightBuffer
{
    float3 lightPosition;
    float lightRadius;
    float4 lightColor;
    float lightIntensity;
    float3 spotLightDirection;
    float lightAngle;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 view : TEXCOORD1;
};

float4 main(PixelInput IN) : SV_TARGET
{
    IN.normal = normalize(IN.normal);
    IN.view = normalize(IN.view);

    float4 tex = mainTexture.Sample(mainSampler, IN.uv);

    float3 lightDirection = lightPosition - IN.positionW;
    float lightDirectionLength = length(lightDirection);
    
    
    float4 diffuseColor = 0;
    float4 specularColor = 0;

    if (lightDirectionLength <= lightRadius)
    {
        float3 light = lightDirection / lightDirectionLength;
        float diffuse = dot(IN.normal, light);

        float angle = max(dot(-light, normalize(spotLightDirection)), 0.0f);

        float lightAttenuation = 1.0f / (1.0f + 0.2f * lightDirectionLength + 0.1f * lightDirectionLength * lightDirectionLength) * pow(angle, lightAngle);

    
        if (diffuse > 0.0f)
        {
            diffuseColor = saturate(lightColor * diffuse * lightIntensity * lightAttenuation);

            // Phong
            // float specular = pow(abs(dot(IN.view, 2 * diffuse * IN.normal - light)), 512.0f);

            // Blinn-Phong
            float specular = pow(dot(IN.normal, (IN.view + light) * 0.5f), 256.0f);
            specularColor = saturate(lightColor * specular * lightIntensity * lightAttenuation);
        }
    }


    // return tex * float4(0.12f, 0.1f, 0.05f, 1.0f);

    // tex * (ambient + diffuse) + specular + emission

    return saturate(saturate(tex * saturate(float4(0.1f, 0.1f, 0.1f, 1.0f) + diffuseColor)) + specularColor + float4(0.05f, 0.0f, 0.0f, 1.0f));
}
