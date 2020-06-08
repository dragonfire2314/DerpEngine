#include <DerpEngine.h>
#include <stdio.h>
#include <iostream>

#include "Movement.h"
#include "CameraMovement.h"
#include "RotateAroundScene.h"
#include "PlayerControls.h"
#include "AudioTesting.h"
#include "AnimTest.h"

using namespace DERP;

int main()
{
	SystemManager sm;

	Application app;

	app.Init(1024, 768);
    EM::Init();

    MeshManager meshManager;

    int chest = meshManager.loadMeshes("../models/Treasure_Chest/treasure_chest2.obj");
    int bar = meshManager.loadMeshes("../models/Chemical_Barrel/chemicalbarrel.obj");
    int cube = meshManager.loadMeshes("../models/handmade/cube.obj");
    int ico = meshManager.loadMeshes("../models/handmade/ico.obj");
    int cone = meshManager.loadMeshes("../models/handmade/cone.obj");
    int torus = meshManager.loadMeshes("../models/handmade/torus.obj");
    int plane = meshManager.loadMeshes("../models/handmade/plane.obj");
    int testAnim = meshManager.loadMeshes("../models/anims/Swing_Dancing.fbx");
    int man = meshManager.loadMeshes("../models/anims/model.dae");

    Animation_Info info = loadAnimationFromFile("../models/anims/Swing_Dancing.fbx");
    Animation_Info Jumpinfo = loadAnimationFromFile("../models/anims/Jumping.fbx");
    Animation_Info ManInfo = loadAnimationFromFile("../models/anims/model.dae");

    //Chest
    uint32_t ent = EM::CreateEntity();
    EM::getEntity(ent)->name = "Chest";
    CM::AddComponent<Transform>(ent);
    CM::AddComponent<Mesh>(ent);
    CM::AddComponent<Material>(ent);
    CM::GetComponent<Mesh>(ent)->mesh = meshManager.getMesh(chest, 0);
    CM::GetComponent<Transform>(ent)->position = glm::vec3(3, -2, 0);
    CM::GetComponent<Material>(ent)->mat = meshManager.getMaterial(chest, 0);

    //Floor
    uint32_t ent3 = EM::CreateEntity();
    EM::getEntity(ent3)->name = "Plane";
    CM::AddComponent<Transform>(ent3);
    CM::AddComponent<Mesh>(ent3);
    CM::AddComponent<Material>(ent3);
    CM::AddComponent<RigidBody>(ent3);
    CM::AddComponent<BoxCollider>(ent3);
    CM::GetComponent<RigidBody>(ent3)->e = 0.4f;
    CM::GetComponent<BoxCollider>(ent3)->halfSides = glm::vec3(4, 0.01, 4);
    CM::GetComponent<Mesh>(ent3)->mesh = meshManager.getMesh(plane, 0);
    CM::GetComponent<Transform>(ent3)->position = glm::vec3(0, -2, 0);
    CM::GetComponent<Material>(ent3)->mat->Kd = objl::Vector3(0.5, 0.1, 0.7);

    //Create a camera
    uint32_t cam = EM::CreateEntity();
    EM::getEntity(cam)->name = "Camera";
    CM::AddComponent<Camera>(cam);
    CM::AddComponent<Transform>(cam);
    Transform* cam_t = CM::GetComponent<Transform>(cam);
    cam_t->position = glm::vec3(-4.44764, 3.3252, 10.8293);
    cam_t->rotation = glm::quat(glm::vec3(-0.278857, -0.310267, -0.0491606));
    CM::AddComponent<Script>(cam);
    Script* s3 = CM::GetComponent<Script>(cam);
    s3->script = new CameraMovement();

    //Create a Point Light
    uint32_t pLight = EM::CreateEntity();
    EM::getEntity(pLight)->name = "Ico";
    CM::AddComponent<DirectionalLight>(pLight);
    CM::AddComponent<Transform>(pLight);
    CM::AddComponent<Script>(pLight);
    CM::GetComponent<Script>(pLight)->script = new RotateAroundScene();
    CM::AddComponent<Mesh>(pLight);
    CM::AddComponent<Material>(pLight);
    CM::GetComponent<Mesh>(pLight)->mesh = meshManager.getMesh(ico, 0);
    CM::GetComponent<Transform>(pLight)->position.y = 1;

    //Character
    uint32_t character = EM::CreateEntity();
    EM::getEntity(character)->name = "Player";
    CM::AddComponent<Transform>(character);
    CM::AddComponent<Mesh>(character);
    CM::AddComponent<Material>(character);
    CM::AddComponent<Script>(character);
    CM::AddComponent<BoxCollider>(character);
    CM::AddComponent<RigidBody>(character);
    CM::GetComponent<RigidBody>(character)->useGravity = true;
    CM::GetComponent<RigidBody>(character)->inverseMass = 0.1f;
    CM::GetComponent<RigidBody>(character)->e = 0.4f;
    CM::GetComponent<Script>(character)->script = new PlayerControls();
    CM::GetComponent<Mesh>(character)->mesh = meshManager.getMesh(cube, 0);
    CM::GetComponent<Transform>(character)->position.y = 3;    
    CM::GetComponent<Material>(character)->mat->Kd = objl::Vector3(1, 0.5, 0.3);

    uint32_t sound = EM::CreateEntity();
    EM::getEntity(sound)->name = "Player";
    CM::AddComponent<Transform>(sound);
    CM::AddComponent<Mesh>(sound);
    CM::AddComponent<Material>(sound);
    CM::AddComponent<AudioSource>(sound);
    CM::AddComponent<Script>(sound);
    CM::GetComponent<Script>(sound)->script = new AudioTest();
    //CM::GetComponent<Mesh>(sound)->mesh = meshManager.getMesh(cone, 0);
    CM::GetComponent<Material>(sound)->mat->Kd = objl::Vector3(1, 0.5, 0.3);
    AudioSource* a = CM::GetComponent<AudioSource>(sound);
    a->changeAudio("komikuOP.ogg");
    //a->play();

    //Player child
    uint32_t charChild = EM::CreateEntity(character);
    EM::getEntity(charChild)->name = "Player Child";
    CM::AddComponent<Transform>(charChild);
    CM::AddComponent<Mesh>(charChild);
    CM::AddComponent<Material>(charChild);
    CM::GetComponent<Mesh>(charChild)->mesh = meshManager.getMesh(cone, 0);
    CM::GetComponent<Material>(charChild)->mat->Kd = objl::Vector3(1, 0.5, 0.3);
    CM::GetComponent<Transform>(charChild)->position = glm::vec3(0, 2, 0);

    //Player child
    uint32_t charChildChild = EM::CreateEntity(charChild);
    EM::getEntity(charChildChild)->name = "Player Child Child";
    CM::AddComponent<Transform>(charChildChild);
    CM::AddComponent<Mesh>(charChildChild);
    CM::AddComponent<Material>(charChildChild);
    CM::GetComponent<Mesh>(charChildChild)->mesh = meshManager.getMesh(cone, 0);
    CM::GetComponent<Material>(charChildChild)->mat->Kd = objl::Vector3(1, 0.5, 0.3);
    CM::GetComponent<Transform>(charChildChild)->position = glm::vec3(0, 2, 0);

    //Anim test
    uint32_t anim = EM::CreateEntity();
    EM::getEntity(anim)->name = "anim";
    CM::AddComponent<Transform>(anim);
    CM::AddComponent<Mesh>(anim);
    CM::AddComponent<Material>(anim);
    CM::AddComponent<Animator>(anim);
    CM::AddComponent<Script>(anim);
    CM::GetComponent<Script>(anim)->script = new AnimTest();
    CM::GetComponent<Animator>(anim)->clipID = getAnimationClipID(info.animationGroupID, info.animationNames[0]);
    CM::GetComponent<Mesh>(anim)->mesh = meshManager.getMesh(testAnim, 0);
    CM::GetComponent<Mesh>(anim)->mesh->isAnimation = true;
    CM::GetComponent<Material>(anim)->mat->Kd = objl::Vector3(0.211, 0.509, 0.729);
    CM::GetComponent<Transform>(anim)->scale = glm::vec3(0.01, 0.01, 0.01);
    uint32_t animChild = EM::CreateEntity(anim);
    EM::getEntity(animChild)->name = "animChild";
    CM::AddComponent<Transform>(animChild);
    CM::AddComponent<Mesh>(animChild);
    CM::AddComponent<Material>(animChild);
    CM::AddComponent<Animator>(animChild);
    CM::GetComponent<Animator>(animChild)->clipID = getAnimationClipID(info.animationGroupID, info.animationNames[0]);
    CM::GetComponent<Mesh>(animChild)->mesh = meshManager.getMesh(testAnim, 1);
    CM::GetComponent<Mesh>(animChild)->mesh->isAnimation = true;
    CM::GetComponent<Material>(animChild)->mat->Kd = objl::Vector3(0.541, 0.541, 0.541);
    //CM::GetComponent<Transform>(animChild)->scale = glm::vec3(0.01, 0.01, 0.01);

    //Particle
    uint32_t particle = EM::CreateEntity();
    EM::getEntity(particle)->name = "Particle";
    CM::AddComponent<Transform>(particle);
    CM::AddComponent<Mesh>(particle);
    CM::AddComponent<Material>(particle);
    CM::AddComponent<ParticleEffect>(particle);
    CM::GetComponent<Material>(particle)->mat->Kd = objl::Vector3(1, 0.5, 0.3);
    CM::GetComponent<Transform>(particle)->position = glm::vec3(0, 2, 0);

    ////Physics obj 1
    //uint Phys1 = EntityManager::CreateEntity();
    //EntityManager::getEntity(Phys1)->name = "Physics 1";
    //CM::AddComponent<Transform>(Phys1);
    //CM::AddComponent<Mesh>(Phys1);
    //CM::AddComponent<Material>(Phys1);
    //CM::AddComponent<RigidBody>(Phys1);
    //CM::AddComponent<BoxCollider>(Phys1);
    //CM::GetComponent<RigidBody>(Phys1)->useGravity = true;
    //CM::GetComponent<Mesh>(Phys1)->mesh = meshManager.getMesh(cube, 0);
    //CM::GetComponent<Transform>(Phys1)->position.y = 3;
    //CM::GetComponent<Material>(Phys1)->mat->Kd = objl::Vector3(1, 0.5, 0.3);

    ////Physics obj 1
    //uint Phys2 = EntityManager::CreateEntity();
    //EntityManager::getEntity(Phys2)->name = "Physics 2";
    //CM::AddComponent<DirectionalLight>(Phys2);
    //CM::AddComponent<Transform>(Phys2);
    //CM::AddComponent<Mesh>(Phys2);
    //CM::AddComponent<Material>(Phys2);
    //CM::AddComponent<RigidBody>(Phys2);
    //CM::AddComponent<BoxCollider>(Phys2);
    //CM::GetComponent<Mesh>(Phys2)->mesh = meshManager.getMesh(cube, 0);
    //CM::GetComponent<Transform>(Phys2)->position.y = -1.5;
    //CM::GetComponent<Transform>(Phys2)->position.x = 0.5;
    //CM::GetComponent<Transform>(Phys2)->position.z = 0.5;

	app.Run();

	return 0;
}




//#include <DerpEngine.h>
//#include <stdio.h>
//
//#include "Movement.h"
//
//using namespace DERP;
//
//int main() 
//{
//	//Creating entites
//	Entity* ent = EntityManager::getInstance().createEntity();
//	Entity* ent2 = EntityManager::getInstance().createEntity(ent);
//
//	//Interfaces
//	ApplicationDebug app;
//	ComponentManager componentManager;
//
//	MeshManager meshManage;
//
//	//Loading a mesh
//	int temp = meshManage.loadMeshes("../models/Treasure_Chest/treasure_chest2.obj");
//
//	if (temp == -1) 
//	{
//		printf("Bad Dir");
//	}
//
//	//Adding transforms
//	componentManager.addComponent(ComponentTransform::getInstance(), ent);
//	//componentManager.addComponent(ComponentTransform::getInstance(), ent2);
//	//Transform* ent2_t = componentManager.getComponent<Transform>(ComponentTransform::getInstance(), ent2);
//	//ent2_t->position = glm::vec3(1, 0, -2);
//	//Adding scripts
//	componentManager.addScript(new Movement(), ent);
//
//	//Adding a mesh
//	componentManager.addComponent(ComponentMesh::getInstance(), ent);
//	Mesh* m = ent->getComponent<Mesh>(ComponentMesh::getInstance());
//	m->mesh = meshManage.getMesh(0,0);
//
//	//componentManager.addComponent(ComponentMesh::getInstance(), ent2);
//	//Mesh* m2 = ent2->getComponent<Mesh>(ComponentMesh::getInstance());
//	//m2->mesh = meshManage.getMesh(0, 0);
//
//	//Adding a material
//	componentManager.addComponent(ComponentMaterial::getInstance(), ent);
//	Material* mat = ent->getComponent<Material>(ComponentMaterial::getInstance());
//	mat->setShader("vertex.v", "pixel.p");
//	mat->mat = meshManage.getMaterial(0, 0);
//
//	//componentManager.addComponent(ComponentMaterial::getInstance(), ent2);
//	//Material* mat2 = ent2->getComponent<Material>(ComponentMaterial::getInstance());
//	//mat2->setShader("vertex.v", "pixel.p");
//
//
//	app.Run();
//
//	return 0;
//}