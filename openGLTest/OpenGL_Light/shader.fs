#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//����
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;


};

//�����
struct dirLight {
    vec4 lightDir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//���Դ
struct pointLight {
    vec3 lightPos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

//�۹�Դ
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
uniform Material material;//���ʣ��������Ĺ�����ɫ�����;��淴���ǿ��
uniform dirLight dlight;//����ƽ�й�
uniform pointLight plight[POINT_NUM];//���Դ
uniform flashLight flight;//�۹�Դ


vec3 CalcFlashLight(dirLight dlight, vec3 Normal, vec3 FragPos, vec3 viewPos);
vec3 CalcDirLight(flashLight flight, vec3 Normal, vec3 FragPos, vec3 viewPos);
vec3 CalcPointLight(pointLight plight, vec3 Normal, vec3 FragPos, vec3 viewPos);


//����ƽ�й�
vec3 CalcFlashLight(dirLight dlight, vec3 Normal, vec3 FragPos, vec3 viewPos)
{
    vec3 dirColor = vec3(0.0f);
    //���������ɫ
    vec4 texColor = texture(material.diffuse,TexCoords);
    vec4 texSpecularColor = texture(material.specular,TexCoords);
    //�жϹ��շ���or����λ��
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
    // ������(Ambient Lighting):ȫ�ֹ����㷨�Ƚϸ����ҿ����߰���Ŀǰֻ�Ǽ�ʹ��ȫ�ֵĹ���ģ�ͣ�����һ��ȫ�ֵĹ�����ɫ��
    vec3 ambient = dlight.ambient*texColor.rgb;
    //��������գ�Diffuse Lighting��:������ϵ���ǹ��շ���ͷ��߷���ĽǶ�ֵ���Ƕ�Խ�󣬷���ϵ��ԽС��
    vec3 norm = normalize(Normal);
    vec3 lightDir = lightDirIn;
    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = diff *dlight.diffuse*texColor.rgb;
    //����߹⣨Specular Highlight�����߹��ʵ�ֻ��ڹ��ջ��ڷ��ߵķ�����۲����ӽǵĽǶȣ��Ƕ�ԽС�߹�Խǿ��
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(reflectDir,viewDir),0),material.shininess);//���淴���ǿ�ȣ�����߹�ķ���ȣ������Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС��
    vec3 specular = dlight.specular * texSpecularColor.rgb*spec ;
    dirColor = ambient+diffuse+specular;
    return  dirColor;
}

//������Դ
vec3 CalcPointLight(pointLight plight, vec3 Normal, vec3 FragPos, vec3 viewPos)
{
     vec3 pointColor = vec3(0.0f);
    //���Դ˥��ǿ��ֵ
    float distance    = length(plight.lightPos - FragPos);
    float attenuation = 1.0 / (plight.constant + plight.linear * distance + plight.quadratic * (distance * distance));
    //���շ���
    vec3 lightDir = normalize(plight.lightPos - FragPos);
    //���������ɫ
    vec4 texColor = texture(material.diffuse,TexCoords);
    vec4 texSpecularColor = texture(material.specular,TexCoords);
    // ������(Ambient Lighting):ȫ�ֹ����㷨�Ƚϸ����ҿ����߰���Ŀǰֻ�Ǽ�ʹ��ȫ�ֵĹ���ģ�ͣ�����һ��ȫ�ֵĹ�����ɫ��
    vec3 ambient = plight.ambient*texColor.rgb;
    //��������գ�Diffuse Lighting��:������ϵ���ǹ��շ���ͷ��߷���ĽǶ�ֵ���Ƕ�Խ�󣬷���ϵ��ԽС��
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = diff *plight.diffuse*texColor.rgb;
    //����߹⣨Specular Highlight�����߹��ʵ�ֻ��ڹ��ջ��ڷ��ߵķ�����۲����ӽǵĽǶȣ��Ƕ�ԽС�߹�Խǿ��
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(reflectDir,viewDir),0),material.shininess);//���淴���ǿ�ȣ�����߹�ķ���ȣ������Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС��
    vec3 specular = plight.specular * texSpecularColor.rgb*spec ;
    pointColor = ambient + diffuse + specular;
    return pointColor;
}

//����۹��
vec3 CalcFlashLight(flashLight flight, vec3 Normal, vec3 FragPos, vec3 viewPos)
{
    
    vec3 flashColor = vec3(0.0f);
    //���������ɫ
    vec4 texColor = texture(material.diffuse,TexCoords);
    vec4 texSpecularColor = texture(material.specular,TexCoords);
    //���շ���
    vec3 lightDir = normalize(flight.lightPos - FragPos);
    //������շ�����۹���ָ��ķ���ļн�
    float theta = dot(lightDir,normalize(-flight.direction));
    // ������(Ambient Lighting):ȫ�ֹ����㷨�Ƚϸ����ҿ����߰���Ŀǰֻ�Ǽ�ʹ��ȫ�ֵĹ���ģ�ͣ�����һ��ȫ�ֵĹ�����ɫ��
    vec3 ambient = flight.ambient*texColor.rgb;
    //��������գ�Diffuse Lighting��:������ϵ���ǹ��շ���ͷ��߷���ĽǶ�ֵ���Ƕ�Խ�󣬷���ϵ��ԽС��
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = diff *flight.diffuse*texColor.rgb;
    //����߹⣨Specular Highlight�����߹��ʵ�ֻ��ڹ��ջ��ڷ��ߵķ�����۲����ӽǵĽǶȣ��Ƕ�ԽС�߹�Խǿ��
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(reflectDir,viewDir),0),material.shininess);//���淴���ǿ�ȣ�����߹�ķ���ȣ������Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС��
    vec3 specular = flight.specular * texSpecularColor.rgb*spec ;
    //����нǴ��ھ۹����ܸ��ǵĽǶȣ�ֻ��ʾ�����⡣
    if(theta <flight.cutOff)
    {
        flashColor = ambient;
    }else
    {
        // ����۹��Ч��
        float epsilon   = flight.cutOff - flight.outerCutOff;
        float intensity = clamp((theta - flight.outerCutOff) / epsilon, 0.0, 1.0);    
        // �����Ի���������Ӱ�죬������������һ���
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