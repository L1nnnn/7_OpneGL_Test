#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // �Ӷ�����ɫ�����������������������ͬ��������ͬ��
in vec2 TexCoord; 
uniform sampler2D ourTexture;
void main()
{
    vec4 sourceColor = texture(ourTexture, TexCoord);
    FragColor = sourceColor;
}
  