float4x4 mat_mvp;  //Произведение мировой, видовой и проекционной матриц.
float4x4 mat_world; //Мировая матрица.
float4   vec_light; //Позиция источника света

struct VS_INPUT_STRUCT //Входящие данные
{
    float4 position: POSITION;
    float3 normal:   NORMAL;
};

struct VS_OUTPUT_STRUCT //Исходящие данные
{
    float4 position: POSITION;
    float3 light:    TEXCOORD0;
    float3 normal:   TEXCOORD1;
};

VS_OUTPUT_STRUCT main(VS_INPUT_STRUCT In_struct)
{
    VS_OUTPUT_STRUCT Out_struct;
    //Вычисляем позицию вершины.
    Out_struct.position = mul(mat_mvp, In_struct.position);
    //Сохраняем позицию источника для передачи во фрагментный шейдер в виде
    //3D текстурных координат.
    Out_struct.light = vec_light;
    //Рассчитываем нормаль поверхности и сохраняем для фрагментного шейдера.
    Out_struct.normal = normalize(mul(mat_world, In_struct.normal));
    //*под словом “сохраняем” имеется ввиду посылание данных в растеризатор,
    //а только потом в вершинный шейдер.
    return Out_struct;
}