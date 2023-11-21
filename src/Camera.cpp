#include "Camera.h"
using namespace so;
using namespace glm;
float Camera::aspectRatio;
Camera::Camera(float FOV) : FOV(FOV)
{
    aspectRatio = (float)WINDOW.internal.getSize().x / (float)WINDOW.internal.getSize().y;
}
void Camera::setFOV(float FOV, bool doUpdate)
{
    this->FOV = FOV;
    if (doUpdate)
        update();
}
void Camera::setAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    update();
}
void Camera::update()
{
//     vec3 camPos = transform.getPosition();
//     vec3 camDir = vec3(0, 0, -1);
//     vec3 camUp = vec3(0, 1, 0);
//     mat3 rotationMat = mat3(transform.getRotation());
//     camDir = rotationMat * camDir;
//     camUp = rotationMat * camUp;
//     vec3 lookedAt = camDir + camPos;
//     mat4 viewPerspective = perspective(FOV, aspectRatio, 0.1f, 120.0f) *
//                            lookAt(camPos, lookedAt, camUp);
//     glUniformMatrix4fv(viewPerspectiveLocation, 1, GL_FALSE, &viewPerspective[0][0]);
//     glUniform3fv(posLocation, 1, &camPos.x);
//     assert(glGetError() == 0);
}
// void Camera::fixedUpdate(float deltaTime)
// {
//     rigidbody.velocity += rigidbody.acceleration * deltaTime;
//     rigidbody.angularVelocity += rigidbody.angularAcceleration * deltaTime;
//     vec3 appliedVelocity = rigidbody.velocity;
//     vec3 appliedAngularVelocity = rigidbody.angularVelocity;
//     if (rigidbody.isLinearLocked(AXIS::x))
//         appliedVelocity.x = 0;
//     if (rigidbody.isLinearLocked(AXIS::y))
//         appliedVelocity.y = 0;
//     if (rigidbody.isLinearLocked(AXIS::z))
//         appliedVelocity.z = 0;
//     if (rigidbody.isAngularLocked(AXIS::x))
//         appliedAngularVelocity.x = 0;
//     if (rigidbody.isAngularLocked(AXIS::y))
//         appliedAngularVelocity.y = 0;
//     if (rigidbody.isAngularLocked(AXIS::z))
//         appliedAngularVelocity.z = 0;
//     transform.translate(appliedVelocity * deltaTime);
//     transform.rotate(appliedAngularVelocity * deltaTime);
// }
void Camera::setViewPerspectiveLocation(GLint location)
{
    viewPerspectiveLocation = location;
}
void Camera::setPosLoaction(GLint location){
    posLocation = location;
}

glm::mat4 Core::createPerspectiveMatrix(float zNear, float zFar, float frustumScale)
{
	glm::mat4 perspective;
    perspective[0][0] = 1.f;
	perspective[1][1] = frustumScale;
	perspective[2][2] = (zFar + zNear) / (zNear - zFar);
	perspective[3][2] = (2 * zFar * zNear) / (zNear - zFar);
	perspective[2][3] = -1;
	perspective[3][3] = 0;

	return perspective;
}

glm::mat4 Core::createViewMatrix( glm::vec3 position, glm::vec3 forward, glm::vec3 up )
{
	glm::vec3 side = glm::cross(forward, up);

	// Trzeba pamietac o minusie przy ustawianiu osi Z kamery.
	// Wynika to z tego, ze standardowa macierz perspektywiczna zaklada, ze "z przodu" jest ujemna (a nie dodatnia) czesc osi Z.
	glm::mat4 cameraRotation;
	cameraRotation[0][0] = side.x; cameraRotation[1][0] = side.y; cameraRotation[2][0] = side.z;
	cameraRotation[0][1] = up.x; cameraRotation[1][1] = up.y; cameraRotation[2][1] = up.z;
	cameraRotation[0][2] = -forward.x; cameraRotation[1][2] = -forward.y; cameraRotation[2][2] = -forward.z;

	glm::mat4 cameraTranslation;
	cameraTranslation[3] = glm::vec4(-position, 1.0f);

	return cameraRotation * cameraTranslation;
}
