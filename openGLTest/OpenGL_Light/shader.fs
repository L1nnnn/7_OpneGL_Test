#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//材质
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;


};

//定向光
struct dirLight {
    vec4 lightDir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//点光源
struct pointLight {
    vec3 lightPos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

//聚光源
struct flashLight {
    vec3 lightPos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3  direction;
    float cutOff;
    float outerCutOff;
};

#define POINT_NUM 4
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Material material;//材质：物体表面的光照颜色分量和镜面反射光强度
uniform dirLight dlight;//定向平行光
uniform pointLight plight[POINT_NUM];//点光源
uniform flashLight flight;//聚光源


vec3 CalcFlashLight(dirLight dlight, vec3 Normal, vec3 FragPos, vec3 viewPos);
vec3 CalcDirLight(flashLight flight, vec3 Normal, vec3 FragPos, vec3 viewPos);
vec3 CalcPointLight(pointLight plight, vec3 Normal, vec3 FragPos, vec3 viewPos);


//计算平行光
vec3 CalcFlashLight(dirLight dlight, vec3 Normal, vec3 FragPos, vec3 viewPos)
{
    vec3 dirColor = vec3(0.0f);
    //纹理采样颜色
    vec4 texColor = texture(material.diffuse,TexCoords);
    vec4 texSpecularColor = texture(material.specular,TexCoords);
    //判断光照方向or光照位置
    vec3 lightDirIn = vec3(0.0f,0.0f,0.0f);
if(dlight.lightDir.z == 0.0f)
{
    lightDirIn = vec3(normalize(dlight.lightDir));
}else
{
    lightDirIn = normalize(dlight.lightDir.xyz-FragPos);
 
}
 lightDirIn = vec3(normalize(dlight.lightDir));
lightDirIn = normalize(dlight.lightDir.xyz-FragPos);
    // 环境光(Ambient Lighting):全局光照算法比较复杂且开销高昂，目前只是简单使用全局的光照模型，即给一个全局的光照颜色。
    vec3 ambient = dlight.ambient*texColor.rgb;
    //漫反射光照（Diffuse Lighting）:漫反射系数是光照方向和法线方向的角度值，角度越大，反射系数越小。
    vec3 norm = normalize(Normal);
    vec3 lightDir = lightDirIn;
    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = diff *dlight.diffuse*texColor.rgb;
    //镜面高光（Specular Highlight）：高光的实现基于光照基于法线的反射与观察者视角的角度，角度越小高光越强。
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(reflectDir,viewDir),0),material.shininess);//镜面反射光强度，代表高光的反光度，反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
    vec3 specular = dlight.specular * texSpecularColor.rgb*spec ;
    dirColor = ambient+diffuse+specular;
    return  dirColor;
}

//计算点光源
vec3 CalcPointLight(pointLight plight, vec3 Normal, vec3 FragPos, vec3 viewPos)
{
     vec3 pointColor = vec3(0.0f);
    //点光源衰减强度值
    float distance    = length(plight.lightPos - FragPos);
    float attenuation = 1.0 / (plight.constant + plight.linear * distance + plight.quadratic * (distance * distance));
    //光照方向
    vec3 lightDir = normalize(plight.lightPos - FragPos);
    //纹理采样颜色
    vec4 texColor = texture(material.diffuse,TexCoords);
    vec4 texSpecularColor = texture(material.specular,TexCoords);
    // 环境光(Ambient Lighting):全局光照算法比较复杂且开销高昂，目前只是简单使用全局的光照模型，即给一个全局的光照颜色。
    vec3 ambient = plight.ambient*texColor.rgb;
    //漫反射光照（Diffuse Lighting）:漫反射系数是光照方向和法线方向的角度值，角度越大，反射系数越小。
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = diff *plight.diffuse*texColor.rgb;
    //镜面高光（Specular Highlight）：高光的实现基于光照基于法线的反射与观察者视角的角度，角度越小高光越强。
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(reflectDir,viewDir),0),material.shininess);//镜面反射光强度，代表高光的反光度，反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
    vec3 specular = plight.specular * texSpecularColor.rgb*spec ;
    pointColor = ambient + diffuse + specular;
    return pointColor;
}

//计算聚光灯
vec3 CalcFlashLight(flashLight flight, vec3 Normal, vec3 FragPos, vec3 viewPos)
{
    
    vec3 flashColor = vec3(0.0f);
    //纹理采样颜色
    vec4 texColor = texture(material.diffuse,TexCoords);
    vec4 texSpecularColor = texture(material.specular,TexCoords);
    //光照方向
    vec3 lightDir = normalize(flight.lightPos - FragPos);
    //计算光照方向与聚光所指向的方向的夹角
    float theta = dot(lightDir,normalize(-flight.direction));
    // 环境光(Ambient Lighting):全局光照算法比较复杂且开销高昂，目前只是简单使用全局的光照模型，即给一个全局的光照颜色。
    vec3 ambient = flight.ambient*texColor.rgb;
    //漫反射光照（Diffuse Lighting）:漫反射系数是光照方向和法线方向的角度值，角度越大，反射系数越小。
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = diff *flight.diffuse*texColor.rgb;
    //镜面高光（Specular Highlight）：高光的实现基于光照基于法线的反射与观察者视角的角度，角度越小高光越强。
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(reflectDir,viewDir),0),material.shininess);//镜面反射光强度，代表高光的反光度，反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
    vec3 specular = flight.specular * texSpecularColor.rgb*spec ;
    //如果夹角大于聚光所能覆盖的角度，只显示环境光。
    if(theta <flight.cutOff)
    {
        flashColor = ambient;
    }else
    {
        // 计算聚光灯效果
        float epsilon   = flight.cutOff - flight.outerCutOff;
        float intensity = clamp((theta - flight.outerCutOff) / epsilon, 0.0, 1.0);    
        // 将不对环境光做出影响，让它总是能有一点光
        diffuse  *= intensity;
        specular *= intensity;
        flashColor = ambient+diffuse+specular;
    }


    return flashColor;

}

void main()
{

  
  
   //FragColor.a = texColor.a*texSpecularColor.a;
   vec3 dirColor =  CalcFlashLight(dlight,Normal,FragPos,viewPos);
   vec3 pointColor = vec3(0.0f);
   for(int i = 0; i < POINT_NUM; i++)
   {
        pointColor += CalcPointLight(plight[i],Normal,FragPos,viewPos);     
   }
 
   vec3 flashColor =CalcFlashLight(flight,Normal,FragPos,viewPos);

   FragColor = vec4(dirColor+flashColor+pointColor, 1.0);
   
   
}