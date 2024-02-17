#include "Object3d.hlsli"

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

struct Camera
{
    float32_t3 position;
};

struct PointLight
{
    float32_t4 color; //ライトの色
    float32_t3 position; //ライトの位置
    float intensity; //輝度
    float radius; //ライトの届く最大距離
    float decay; //減衰率
};

struct SpotLight
{
    float32_t4 color; //ライトの色
    float32_t3 position; //ライトの位置
    float32_t intensity; //輝度
    float32_t3 direction; //スポットライトの方向
    float32_t distance; //ライトの届く最大距離
    float32_t decay; //減衰率
    float32_t cosAngle; //スポットライトの余弦
    float32_t cosFalloffStart;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCameraPos : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);
ConstantBuffer<SpotLight> gSpotLight : register(b4);

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    if (gMaterial.enableLighting != 0)
    {
        //phong
        float32_t3 toEye = normalize(gCameraPos.position - input.worldPosition);
        float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
        float RdotE = dot(reflectLight, toEye);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess);
        //HalfLambert
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        //Lambert
        //float cos = saturate(dot(normalize(input.normal),-gDirectionalLight.direction));
        
        //拡散反射
        float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        //鏡面反射
        float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        
        //PointLight
        float32_t3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        reflectLight = reflect(pointLightDirection, normalize(input.normal));
        RdotE = dot(reflectLight, toEye);
        specularPow = pow(saturate(RdotE), gMaterial.shininess);
        NdotL = dot(normalize(input.normal), -pointLightDirection);
        cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float32_t distance = length(gPointLight.position - input.worldPosition);
        float32_t factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
        diffuse += gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
        specular += gPointLight.color.rgb * gPointLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f) * factor;
        
        //SpotLight
        float32_t3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
        reflectLight = reflect(spotLightDirectionOnSurface, normalize(input.normal));
        RdotE = dot(reflectLight, toEye);
        specularPow = pow(saturate(RdotE), gMaterial.shininess);
        NdotL = dot(normalize(input.normal), -spotLightDirectionOnSurface);
        cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        distance = length(gSpotLight.position - input.worldPosition);
        factor = pow(saturate(-distance / gSpotLight.distance + 1.0f), gSpotLight.decay);
        float32_t cosAngle = dot(normalize(spotLightDirectionOnSurface), normalize(gSpotLight.direction));
        float32_t falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
        diffuse += gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * cos * gSpotLight.intensity * factor * falloffFactor;
        specular += gSpotLight.color.rgb * gSpotLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f) * factor * falloffFactor;
        
        //拡散反射+鏡面反射+PointLight+SpotLight
        output.color.rgb = diffuse + specular;
        output.color.a = gMaterial.color.a * textureColor.a;
        //output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }
    else
    { //Lightingしない場合。前回までと同じ演算
        output.color = gMaterial.color * textureColor;
    }
    return output;
}
