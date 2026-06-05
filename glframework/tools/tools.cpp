#include "tools.h"
Tools::Tools()
{
}
Tools::~Tools()
{
}
void Tools::decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale)
{
    //四元数,用来表示旋转变换
    glm::quat quaternion;
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(matrix, scale, quaternion, position, skew, perspective);

    //四元数-欧拉角
    glm::mat4 rotation = glm::toMat4(quaternion);
    glm::extractEulerAngleXYZ(rotation, eulerAngle.x, eulerAngle.y, eulerAngle.z);

    //解构出来的角度是弧度,需要转化为角度信息
    eulerAngle.x = glm::degrees(eulerAngle.x);
    eulerAngle.y = glm::degrees(eulerAngle.y);
    eulerAngle.z = glm::degrees(eulerAngle.z);
}

/*void Tools::calTBN(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2, glm::vec3& T, glm::vec3& B)
{
    // calculate tangent/bitangent vectors of both triangles
    // - triangle 1
    glm::vec3 edge1 = point1 - point0;
    glm::vec3 edge2 = point2 - point0;
    glm::vec2 deltaUV1 = uv1 - uv0;
    glm::vec2 deltaUV2 = uv2 - uv0;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    T.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    T.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    T.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    T = glm::normalize(T);

    B.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    B.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    B.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    B = glm::normalize(B);

}*/
