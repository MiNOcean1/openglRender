#include "plyLoader.h"
#include"../glframework/material/whiteMaterial.h"
PlyLoader::PlyLoader()
{
}

PlyLoader::~PlyLoader()
{
}

Object* PlyLoader::load(const std::string& path)
{
    std::vector<float>position;
    std::vector<float>normal;
    std::vector<float>color;
    std::vector<unsigned int>indices;
    int i, j;
    char strBuff[512];
    char type[512] = "";
    bool bStartPropertyCount = true;
    float alpha;
    glm::vec3 vMin, vMax;
    vMin.x = vMin.y = vMin.z = 99999.0;
    vMax.x = vMax.y = vMax.z = -99999.0;

    FILE* fp = fopen(path.c_str(), "r");

    if (fp == NULL)
    {
        printf("ERROR: unable to open model [%s]!\n", path.c_str());
        exit(0);
    }

    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    int g_nVerticesNumber, g_nFaceNumber = 0;
    fscanf(fp, "element vertex %d\n", &g_nVerticesNumber);
    printf("Number of Vertices: %d\n", g_nVerticesNumber);
    j = 0;
    while (strcmp(type, "element") != 0)
    {
        fgets(strBuff, 256, fp);
        i = 0;
        while (strBuff[i] != '\0' && strBuff[i] != ' ' && strBuff[i] != '\n')
        {
            type[i] = strBuff[i];
            i++;
        }
        if (bStartPropertyCount && strcmp(type, "property") == 0)
            j++;
        else
            bStartPropertyCount = false;
        type[i] = '\0';
        // std::cout << strBuff << std::endl;
    }
    i = 0;
    while (strBuff[i] != '\0' && strBuff[i] != '\n')
    {
        if (strBuff[i] >= '0' && strBuff[i] <= '9') {
            g_nFaceNumber = g_nFaceNumber * 10 + strBuff[i] - '0';
        }
        i++;
    }
    printf("Number of faces: %d\n", g_nFaceNumber);
    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    //fgets(strBuff, 256, fp);
    std::cout << j << std::endl;
    //Allocate the triangle array
    //g_vet = new VERTEX3D[g_nVerticesNumber];
    //g_norm = new VERTEX3D[g_nVerticesNumber];
    //g_color = new VERTEX3D[g_nVerticesNumber];

    //Read in the triangles
    for (i = 0; i < g_nVerticesNumber; i++)
    {
        float px, py, pz;
        if (j == 3) {
            fscanf(fp, "%f %f %f", &px, &py, &pz);
            position.push_back(px);
            position.push_back(py);
            position.push_back(pz);

        }
        else if (j == 6) {
            float  nx, ny, nz;
            fscanf(fp, "%f %f %f %f %f %f", &px, &py, &pz, &nx, &ny, &nz);
            position.push_back(px);
            position.push_back(py);
            position.push_back(pz);
            normal.push_back(nx);
            normal.push_back(ny);
            normal.push_back(nz);
            //if (i < 6)std::cout << px << " " << py << " " << pz << std::endl;
        }

        else if (j == 7) {//ÔÝ²»¹Ücolor¶ÁÈ¡
            float cx, cy, cz;
            fscanf(fp, "%f %f %f %f %f %f %f", &px, &py, &pz, &cx, &cy, &cz, &alpha);
            position.push_back(px);
            position.push_back(py);
            position.push_back(pz);
            color.push_back(cx);
            color.push_back(cy);
            color.push_back(cz);
        }
        else if (j == 10) {
            float  nx, ny, nz, cx, cy, cz;
            fscanf(fp, "%f %f %f %f %f %f %f %f %f %f", &px, &py, &pz, &nx, &ny, &nz,
                &cx, &cy, &cz, &alpha);position.push_back(px);
            position.push_back(py);
            position.push_back(pz);
            normal.push_back(nx);
            normal.push_back(ny);
            normal.push_back(nz);
            color.push_back(cx);
            color.push_back(cy);
            color.push_back(cz);

        }
        else
        {
            printf("Warning: the viewer cann't read the colors of models\n");
            exit(1);
        }

        if (px < vMin.x) vMin.x = px;
        if (py < vMin.y) vMin.y = py;
        if (pz < vMin.z) vMin.z = pz;
        if (px > vMax.x) vMax.x = px;
        if (py > vMax.y) vMax.y = py;
        if (pz > vMax.z) vMax.z = pz;
    }
    for (i = 0;i < g_nFaceNumber;i++) {
        int num;
        int a[3];
        fscanf(fp, "%d", &num);
        int res = 0;
        for (int j = 0;j < num;j++) {
            fscanf(fp, "%d", &a[j % 3]);
            if (j == 0)res = a[0];
            if (j >= 2) {
                indices.push_back(res);
                indices.push_back(a[(j - 1 + 3) % 3]);
                indices.push_back(a[j % 3]);
                if (i < 6)std::cout << res << " " << a[(j - 1 + 3) % 3] <<" "<< a[j % 3] << std::endl;
            }
               
        }
    }
    /*
        for (i=0; i<g_nVerticesNumber; i++)
        {
            g_color[i].x = (g_vet[i].x - vMin.x)/(vMax.x - vMin.x);
            g_color[i].y = (g_vet[i].y - vMin.y)/(vMax.y - vMin.y);
            g_color[i].z = (g_vet[i].z- vMin.z)/(vMax.z - vMin.z);
        }*/
    float g_scale = 10.0f;

    if ((vMax.x - vMin.x) > (vMax.y - vMin.y))
    {
        if ((vMax.x - vMin.x) > (vMax.z - vMin.z))
            g_scale = g_scale / (vMax.x - vMin.x);
        else
            g_scale = g_scale / (vMax.z - vMin.z);
    }
    else
    {
        if ((vMax.y - vMin.y) > (vMax.z - vMin.z))
            g_scale = g_scale / (vMax.y - vMin.y);
        else
            g_scale = g_scale / (vMax.z - vMin.z);
    }

    for (i = 0; i < g_nVerticesNumber; i++) {
        position[i * 3] = g_scale * (position[i * 3] - 0.5 * (vMin.x + vMax.x));
        position[i * 3 + 1] = g_scale * (position[i * 3 + 1] - 0.5 * (vMin.x + vMax.x));
        position[i * 3 + 2] = g_scale * (position[i * 3 + 2] - 0.5 * (vMin.x + vMax.x));

    }
    fclose(fp);
     Geometry*  geometry = new Geometry(position, normal, color, indices, 0);
   
        

    auto material = new WhiteMaterial();
    return new Mesh(geometry, material);
}
