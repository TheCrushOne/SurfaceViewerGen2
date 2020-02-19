float4x4 mat_mvp;  //������������ �������, ������� � ������������ ������.
float4x4 mat_world; //������� �������.
float4   vec_light; //������� ��������� �����

struct VS_INPUT_STRUCT //�������� ������
{
    float4 position: POSITION;
    float3 normal:   NORMAL;
};

struct VS_OUTPUT_STRUCT //��������� ������
{
    float4 position: POSITION;
    float3 light:    TEXCOORD0;
    float3 normal:   TEXCOORD1;
};

VS_OUTPUT_STRUCT main(VS_INPUT_STRUCT In_struct)
{
    VS_OUTPUT_STRUCT Out_struct;
    //��������� ������� �������.
    Out_struct.position = mul(mat_mvp, In_struct.position);
    //��������� ������� ��������� ��� �������� �� ����������� ������ � ����
    //3D ���������� ���������.
    Out_struct.light = vec_light;
    //������������ ������� ����������� � ��������� ��� ������������ �������.
    Out_struct.normal = normalize(mul(mat_world, In_struct.normal));
    //*��� ������ ���������� ������� ����� ��������� ������ � ������������,
    //� ������ ����� � ��������� ������.
    return Out_struct;
}