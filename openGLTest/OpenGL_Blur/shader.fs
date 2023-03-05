#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）
in vec2 TexCoord; 
uniform int uBlurRadius;
uniform vec2 uBlurOffset;
uniform sampler2D ourTexture;
uniform float uSumWeight;

vec2 clampCoordinate(vec2 coordinate) 
{    
    return vec2(clamp(coordinate.x, 0.0, 1.0), clamp(coordinate.y, 0.0, 1.0));
    
}

float getWeight(int i) 
{    
    const float PI = 3.1415926;
    float sigma = uBlurRadius / 3.0f;
    return (1.0 / sqrt(2 * PI * sigma *sigma)) * exp(-(i * i) / (2 * sigma * sigma)) / uSumWeight;
}

void main()
{
    vec4 sourceColor = texture(ourTexture, TexCoord);
    if(uBlurRadius <=1)
    {
        FragColor = sourceColor;        
        return; 
    }
    float weight = getWeight(0);
    vec3 finalColor = sourceColor.rgb* weight;
     
    for (int i = 1; i < uBlurRadius; i++)
    {
            weight = getWeight(i);
            finalColor += texture2D(ourTexture, clampCoordinate(TexCoord + uBlurOffset * float(i))).rgb*weight;
            finalColor += texture2D(ourTexture, clampCoordinate(TexCoord - uBlurOffset * float(i))).rgb*weight; 
    }
    FragColor = vec4(finalColor, sourceColor.a);
}
  