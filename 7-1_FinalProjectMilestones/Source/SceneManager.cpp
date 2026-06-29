///////////////////////////////////////////////////////////////////////////////
// shadermanager.cpp
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

// Shader uniform names used throughout the class
namespace {
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  Constructor. Initializes the shader manager and mesh
 *  manager. Also prepares the texture storage array
 *  (declared in SceneManager.h).
 ***********************************************************/
SceneManager::SceneManager(ShaderManager* pShaderManager) {
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();

	// Initialize texture slots (max 16 textures supported)
	for (int i = 0; i < 16; i++) {
		m_textureIDs[i].tag = "/0";
		m_textureIDs[i].ID = 0;
	}
	m_loadedTextures = 0;
}

/***********************************************************
 *  ~SceneManager()
 *
 *  Destructor. Releases memory and deletes OpenGL textures.
 ***********************************************************/
SceneManager::~SceneManager() {
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;

	DestroyGLTextures();
}

/***********************************************************
 *  SetTransformations()
 *
 *  Creates the model matrix (scale × rotation × translation)
 *  and sends it to the vertex shader.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	scale = glm::scale(scaleXYZ);
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationX * rotationY * rotationZ * scale;

	if (NULL != m_pShaderManager) {
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  Sends a solid color to the fragment shader and turns
 *  texturing OFF. Used when we want a shape to remain colored.
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	glm::vec4 currentColor;
	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager) {
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This is the core texture loading function.
 *  It uses stb_image to read the file, creates an OpenGL
 *  texture, sets parameters, generates mipmaps, and stores
 *  the texture with a string tag for later use.
 *
 *  Square textures are used to prevent stretching on triangle and quad faces.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag) {
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// OpenGL expects texture origin at bottom-left, so we flip the image
	stbi_set_flip_vertically_on_load(true);

	unsigned char* image = stbi_load(filename, &width, &height, &colorChannels, 0);

	if (image) {
		std::cout << "Successfully loaded texture: " << filename
			<< " (" << width << "x" << height << ", " << colorChannels << " channels)" << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture wrapping and filtering (important for good visual quality)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Upload image data to the GPU
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		glGenerateMipmap(GL_TEXTURE_2D);   // Create mipmaps for distance rendering
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Store texture in our array using the tag
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "ERROR: Could not load texture file: " << filename << std::endl;
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  Binds every loaded texture to a texture unit (0, 1, 2...).
 *  The shader then uses the unit number to sample the correct texture.
 ***********************************************************/
void SceneManager::BindGLTextures() {
	for (int i = 0; i < m_loadedTextures; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  Deletes all OpenGL texture objects when the program closes.
 ***********************************************************/
void SceneManager::DestroyGLTextures() {
	for (int i = 0; i < m_loadedTextures; i++) {
		glDeleteTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  Looks up a texture by its tag name and returns the
 *  texture unit index. Returns -1 if not found.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag) {
	for (int i = 0; i < m_loadedTextures; i++) {
		if (m_textureIDs[i].tag == tag) {
			return i;
		}
	}
	return -1;
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  Same as FindTextureID (declared in SceneManager.h).
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag) {
	return FindTextureID(tag);
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  Enables texturing in the shader and sets which texture
 *  unit the sampler should read from. This is the main
 *  function called before drawing a textured shape.
 ***********************************************************/
void SceneManager::SetShaderTexture(std::string textureTag) {
	if (NULL != m_pShaderManager) {
		m_pShaderManager->setIntValue(g_UseTextureName, true);
		int textureUnit = FindTextureID(textureTag);

		if (textureUnit >= 0) {
			m_pShaderManager->setIntValue(g_TextureValueName, textureUnit);
		}
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  Allows scaling the UV coordinates (useful for tiling).
 *  Implemented because it is declared in SceneManager.h.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v) {
	if (NULL != m_pShaderManager) {
		m_pShaderManager->setVec2Value("UVScale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  LoadSceneTextures()
 *
 *  This is the student method (declared public in SceneManager.h).
 *  All texture loading for the scene happens here.
 *  We load six different textures from the provided folder.
 ***********************************************************/
void SceneManager::LoadSceneTextures() {

    // Used existing textures to the best of my ability
    CreateGLTexture("../../Utilities/textures/rusticwood.jpg", "rusticwood");
    CreateGLTexture("../../Utilities/textures/knife_handle.jpg", "knifeHandle");
    CreateGLTexture("../../Utilities/textures/drywall.jpg", "drywall");
    CreateGLTexture("../../Utilities/textures/stainless.jpg", "metalSilver");
    CreateGLTexture("../../Utilities/textures/gold-seamless-texture.jpg", "goldMetal");
    CreateGLTexture("../../Utilities/textures/backdrop.jpg", "backdrop");
    CreateGLTexture("../../Utilities/textures/pavers.jpg", "pavers");
    CreateGLTexture("../../Utilities/textures/breadcrust.jpg", "breadcrust");

    // Secondary texture for pinecone blending (complex texturing technique)
    CreateGLTexture("../../Utilities/textures/rusticwood.jpg", "pineconeOverlay");

	// Bind all loaded textures to OpenGL texture units
	BindGLTextures();
}

/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/

/***********************************************************
 * SetupSceneLights()
 *
 * This method sets up the lighting for the 3D scene.
 * It creates a strong "sun" light + a secondary fill lights.
 ***********************************************************/
void SceneManager::SetupSceneLights() {
    // Enable lighting in the shader
    m_pShaderManager->setBoolValue(g_UseLightingName, true);

    // =====================================================
    // LIGHT 0 - Main Overhead Key Light (Strongest)
    // =====================================================
    m_pShaderManager->setVec3Value("lightSources[0].position", 0.0f, 16.0f, 6.0f);
    m_pShaderManager->setVec3Value("lightSources[0].ambientColor", 0.30f, 0.28f, 0.26f);
    m_pShaderManager->setVec3Value("lightSources[0].diffuseColor", 0.85f, 0.82f, 0.78f);
    m_pShaderManager->setVec3Value("lightSources[0].specularColor", 0.95f, 0.93f, 0.90f);
    m_pShaderManager->setFloatValue("lightSources[0].focalStrength", 18.0f);
    m_pShaderManager->setFloatValue("lightSources[0].specularIntensity", 0.65f);

    // =====================================================
    // LIGHT 1 - Left Fill Light
    // =====================================================
    m_pShaderManager->setVec3Value("lightSources[1].position", -12.0f, 13.0f, 5.0f);
    m_pShaderManager->setVec3Value("lightSources[1].ambientColor", 0.22f, 0.22f, 0.24f);
    m_pShaderManager->setVec3Value("lightSources[1].diffuseColor", 0.72f, 0.72f, 0.75f);
    m_pShaderManager->setVec3Value("lightSources[1].specularColor", 0.60f, 0.60f, 0.65f);
    m_pShaderManager->setFloatValue("lightSources[1].focalStrength", 26.0f);
    m_pShaderManager->setFloatValue("lightSources[1].specularIntensity", 0.45f);

    // =====================================================
    // LIGHT 2 - Right Fill Light
    // =====================================================
    m_pShaderManager->setVec3Value("lightSources[2].position", 13.0f, 12.0f, -4.0f);
    m_pShaderManager->setVec3Value("lightSources[2].ambientColor", 0.20f, 0.20f, 0.22f);
    m_pShaderManager->setVec3Value("lightSources[2].diffuseColor", 0.68f, 0.68f, 0.72f);
    m_pShaderManager->setVec3Value("lightSources[2].specularColor", 0.55f, 0.55f, 0.60f);
    m_pShaderManager->setFloatValue("lightSources[2].focalStrength", 28.0f);
    m_pShaderManager->setFloatValue("lightSources[2].specularIntensity", 0.40f);

    // =====================================================
    // LIGHT 3 - Back / Top Fill Light (helps brighten the scene)
    // =====================================================
    m_pShaderManager->setVec3Value("lightSources[3].position", 2.0f, 18.0f, -8.0f);
    m_pShaderManager->setVec3Value("lightSources[3].ambientColor", 0.18f, 0.18f, 0.20f);
    m_pShaderManager->setVec3Value("lightSources[3].diffuseColor", 0.65f, 0.65f, 0.68f);
    m_pShaderManager->setVec3Value("lightSources[3].specularColor", 0.50f, 0.50f, 0.55f);
    m_pShaderManager->setFloatValue("lightSources[3].focalStrength", 22.0f);
    m_pShaderManager->setFloatValue("lightSources[3].specularIntensity", 0.35f);
}

/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{
    // Load all the basic meshes we need for the desk scene
    m_basicMeshes->LoadPlaneMesh();       
    m_basicMeshes->LoadBoxMesh();         // desk, keyboard, notebook, glasses bridge/arms, keys
    m_basicMeshes->LoadSphereMesh();      // mug foam top
    m_basicMeshes->LoadCylinderMesh();    // mug body, coaster, spiral binding, pencils, pen
    m_basicMeshes->LoadTorusMesh();       // mug handle, glasses lenses
    m_basicMeshes->LoadConeMesh();        // pinecone + eucalyptus (simplified)

	// add textures to the scene
	LoadSceneTextures();

	// Add lighting to the scene
	SetupSceneLights();
}

/***********************************************************
* RenderScene()
*
* This method is used for rendering the 3D scene by
* transforming and drawing the basic 3D shapes
***********************************************************/
void SceneManager::RenderScene() {
    // Variables used for every object
    glm::vec3 scaleXYZ;
    float XrotationDegrees = 0.0f;
    float YrotationDegrees = 0.0f;
    float ZrotationDegrees = 0.0f;
    glm::vec3 positionXYZ;

    // =====================================================
    // DESK SURFACE
    // =====================================================
    // I personally didn't like the white desk, so I made it a wood texture to improve it
    scaleXYZ = glm::vec3(35.0f, 2.5f, 19.0f);
    positionXYZ = glm::vec3(0.0f, -2.2f, 0.0f);
    SetTransformations(scaleXYZ, 0.0f, 0.0f, 0.0f, positionXYZ);
    SetShaderTexture("rusticwood");
    SetTextureUVScale(6.0f, 4.0f);
    m_basicMeshes->DrawBoxMesh();

    // =====================================================
    // KEYBOARD + KEYS
    // =====================================================
    scaleXYZ = glm::vec3(10.5f, 0.55f, 4.2f);
    positionXYZ = glm::vec3(-9.0f, -0.45f, -2.5f);
    SetTransformations(scaleXYZ, 0.0f, 2.0f, 0.0f, positionXYZ);
    SetShaderTexture("drywall");           // Light/white keyboard
    m_basicMeshes->DrawBoxMesh();

    // Keyboard keys
    scaleXYZ = glm::vec3(0.55f, 0.32f, 0.55f);
    for (int i = 0; i < 11; i++) {
        positionXYZ = glm::vec3(-12.5f + (i * 0.78f), 0.0f, -2.7f);
        SetTransformations(scaleXYZ, 0.0f, 2.0f, 0.0f, positionXYZ);
        SetShaderTexture("pavers");
        m_basicMeshes->DrawBoxMesh();
    }

    // =====================================================
    // COMPUTER MOUSE
    // =====================================================
    scaleXYZ = glm::vec3(1.4f, 0.55f, 2.1f);
    positionXYZ = glm::vec3(-1.5f, -0.68f, -2.5f); // Near keyboard
    SetTransformations(scaleXYZ, 0.0f, -20.0f, 0.0f, positionXYZ);
    SetShaderTexture("drywall");           // White/light mouse
    m_basicMeshes->DrawBoxMesh();

    // =====================================================
    // MUG + COASTER
    // =====================================================

    // Coaster
    scaleXYZ = glm::vec3(1.7f, 0.11f, 1.7f);
    positionXYZ = glm::vec3(-9.3f, -0.72f, 4.0f);
    SetTransformations(scaleXYZ, 0.0f, 0.0f, 0.0f, positionXYZ);
    SetShaderTexture("knifeHandle");
    m_basicMeshes->DrawCylinderMesh();

    // Mug body
    scaleXYZ = glm::vec3(1.1f, 1.3f, 1.1f);
    positionXYZ = glm::vec3(-9.3f, -0.5f, 4.0f);
    SetTransformations(scaleXYZ, 0.0f, 0.0f, 0.0f, positionXYZ);
    SetShaderTexture("drywall");
    m_basicMeshes->DrawCylinderMesh();

    // Coffee (on top of mug)
    scaleXYZ = glm::vec3(1.0f, 0.25f, 1.0f);
    positionXYZ = glm::vec3(-9.3f, 0.60f, 4.0f);
    SetTransformations(scaleXYZ, 0.0f, 0.0f, 0.0f, positionXYZ);
    SetShaderTexture("backdrop");
    m_basicMeshes->DrawCylinderMesh();

    // Latte foam (smaller sphere sitting on the coffee)
    scaleXYZ = glm::vec3(0.72f, 0.20f, 0.72f);
    positionXYZ = glm::vec3(-9.3f, 0.80f, 4.0f);
    SetTransformations(scaleXYZ, 0.0f, 0.0f, 0.0f, positionXYZ);
    SetShaderColor(0.97f, 0.95f, 0.91f, 1.0f);
    m_basicMeshes->DrawSphereMesh();

    // Mug handle
    scaleXYZ = glm::vec3(0.525f, 0.775f, 0.525f);
    positionXYZ = glm::vec3(-8.55f, -0.05f, 4.0f);
    SetTransformations(scaleXYZ, 0.0f, 0.0f, 90.0f, positionXYZ);
    SetShaderTexture("goldMetal");
    m_basicMeshes->DrawTorusMesh();

    // =====================================================
    // GLASSES (Left side - flat on desk, arms closed)
    // =====================================================
    // Left lens (smaller)
    scaleXYZ = glm::vec3(1.25f, 1.25f, 0.22f);
    positionXYZ = glm::vec3(-6.0f, -0.50f, 2.3f);
    SetTransformations(scaleXYZ, 68.0f, 0.0f, 0.0f, positionXYZ);
    SetShaderTexture("metalSilver");
    m_basicMeshes->DrawTorusMesh();

    // Right lens (smaller + separated)
    positionXYZ = glm::vec3(-3.4f, -0.50f, 2.3f);
    SetTransformations(scaleXYZ, 68.0f, 0.0f, 0.0f, positionXYZ);
    m_basicMeshes->DrawTorusMesh();

    // Bridge (now properly visible between the separated lenses)
    scaleXYZ = glm::vec3(1.1f, 0.12f, 0.18f);
    positionXYZ = glm::vec3(-4.7f, -0.50f, 2.3f);
    SetTransformations(scaleXYZ, 0.0f, 0.0f, 0.0f, positionXYZ);
    SetShaderTexture("metalSilver");
    m_basicMeshes->DrawBoxMesh();

    // Left arm (bigger + properly attached to left lens)
    scaleXYZ = glm::vec3(0.13f, 0.13f, 1.9f);
    positionXYZ = glm::vec3(-6.9f, -0.50f, 2.9f);
    SetTransformations(scaleXYZ, 0.0f, 20.0f, 0.0f, positionXYZ);
    SetShaderTexture("metalSilver");
    m_basicMeshes->DrawBoxMesh();

    // Right arm (bigger + properly attached to right lens)
    positionXYZ = glm::vec3(-2.5f, -0.50f, 2.9f);
    SetTransformations(scaleXYZ, 0.0f, -20.0f, 0.0f, positionXYZ);
    SetShaderTexture("metalSilver");
    m_basicMeshes->DrawBoxMesh();

    // =====================================================
    // NOTEBOOK + BROWN BACKING (Right side, lower)
    // =====================================================

    // Light brown backing
    scaleXYZ = glm::vec3(6.9f, 0.24f, 9.6f);
    positionXYZ = glm::vec3(7.3f, -0.68f, 4.5f);
    SetTransformations(scaleXYZ, 0.0f, -5.0f, 0.0f, positionXYZ);
    SetShaderTexture("rusticwood");
    SetTextureUVScale(2.0f, 2.0f);
    m_basicMeshes->DrawBoxMesh();

    // White notebook paper (on top of brown)
    scaleXYZ = glm::vec3(6.4f, 0.32f, 8.9f);
    positionXYZ = glm::vec3(7.3f, -0.48f, 4.5f);
    SetTransformations(scaleXYZ, 0.0f, -5.0f, 0.0f, positionXYZ);
    SetShaderTexture("drywall");
    SetTextureUVScale(3.0f, 3.0f);
    m_basicMeshes->DrawBoxMesh();

    // Spiral binding (attached to the paper)
    scaleXYZ = glm::vec3(0.19f, 7.1f, 0.19f);
    positionXYZ = glm::vec3(11.2f, -0.3f, 0.5f);
    SetTransformations(scaleXYZ, 90.0f, 0.0f, 95.0f, positionXYZ);
    SetShaderTexture("metalSilver");
    m_basicMeshes->DrawCylinderMesh();

    // =====================================================
    // PENCILS (LEFT of notebook)
    // =====================================================
    scaleXYZ = glm::vec3(0.15f, 0.15f, 2.0f);
    // Pencil 1
    positionXYZ = glm::vec3(-0.5f, -0.50f, 5.5f);
    SetTransformations(scaleXYZ, 5.0f, 48.0f, 0.0f, positionXYZ);
    SetShaderTexture("goldMetal");
    m_basicMeshes->DrawCylinderMesh();

    // Pencil 2
    positionXYZ = glm::vec3(1.5f, -0.50f, 6.2f);
    SetTransformations(scaleXYZ, -7.0f, 30.0f, 0.0f, positionXYZ);
    SetShaderTexture("goldMetal");
    m_basicMeshes->DrawCylinderMesh();

    // =====================================================
    // PEN + PEN CAP (RIGHT of notebook)
    // =====================================================
    // Pen body
    scaleXYZ = glm::vec3(0.16f, 0.16f, 2.2f);
    positionXYZ = glm::vec3(12.4f, -0.50f, 5.8f);
    SetTransformations(scaleXYZ, 10.0f, 0.0f, 0.0f, positionXYZ);
    SetShaderTexture("metalSilver");
    m_basicMeshes->DrawCylinderMesh();

    // Pen cap
    scaleXYZ = glm::vec3(0.17f, 0.17f, 0.95f);
    positionXYZ = glm::vec3(13.6f, -0.50f, 6.4f);
    SetTransformations(scaleXYZ, 10.0f, -45.0f, 0.0f, positionXYZ);
    SetShaderTexture("metalSilver");
    m_basicMeshes->DrawCylinderMesh();

    // =====================================================
    // PINECONE (Bottom left)
    // =====================================================
    // COMPLEX TEXTURING TECHNIQUE:
    // This section demonstrates the required complex texturing technique
    // by blending TWO different textures together on the same object.
    // Base texture = breadcrust.jpg (rough brown)
    // Overlay texture = rusticwood.jpg (different wood grain pattern)
    // The two textures are blended using the textureBlend uniform.
    // This creates more visual detail than using a single texture.
    scaleXYZ = glm::vec3(1.15f, 0.85f, 1.15f);
    positionXYZ = glm::vec3(-13.0f, -0.78f, 6.0f);
    SetTransformations(scaleXYZ, 22.0f, 32.0f, 0.0f, positionXYZ);

    // Apply base texture
    SetShaderTexture("breadcrust");

    // Apply second texture as overlay using dynamic texture unit (correct way)
    int overlayUnit = FindTextureSlot("pineconeOverlay");
    if (overlayUnit >= 0 && NULL != m_pShaderManager) {
        m_pShaderManager->setIntValue("dirtOverlay", overlayUnit);
        m_pShaderManager->setFloatValue("textureBlend", 0.45f); // 45% overlay
    }
    m_basicMeshes->DrawConeMesh();

    // Reset blend so it doesn't affect other objects
    if (NULL != m_pShaderManager) {
        m_pShaderManager->setFloatValue("textureBlend", 0.0f);
    }

    // =====================================================
    // EUCALYPTUS (Laying on desk)
    // =====================================================
    scaleXYZ = glm::vec3(0.32f, 3.6f, 0.32f);
    // Left group
    positionXYZ = glm::vec3(-13.8f, -0.38f, 1.2f);
    SetTransformations(scaleXYZ, 62.0f, 20.0f, 0.0f, positionXYZ);
    SetShaderColor(0.20f, 0.55f, 0.25f, 1.0f);   // Green color
    m_basicMeshes->DrawConeMesh();

    positionXYZ = glm::vec3(-13.0f, -0.38f, 2.8f);
    SetTransformations(scaleXYZ, 68.0f, -12.0f, 0.0f, positionXYZ);
    SetShaderColor(0.20f, 0.55f, 0.25f, 1.0f);
    m_basicMeshes->DrawConeMesh();

    // Right group
    positionXYZ = glm::vec3(12.0f, 0.18f, -3.5f);
    SetTransformations(scaleXYZ, 80.0f, -25.0f, -50.0f, positionXYZ);
    SetShaderColor(0.20f, 0.55f, 0.25f, 1.0f);
    m_basicMeshes->DrawConeMesh();

    positionXYZ = glm::vec3(11.3f, 0.18f, -1.0f);
    SetTransformations(scaleXYZ, 85.0f, 15.0f, -20.0f, positionXYZ);
    SetShaderColor(0.20f, 0.55f, 0.25f, 1.0f);
    m_basicMeshes->DrawConeMesh();
}
