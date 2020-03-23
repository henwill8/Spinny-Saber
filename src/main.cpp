#include "../include/main.hpp"

#include <string>
#include <sstream>

#include <dlfcn.h>

float Spin1 = 0.0f;
float Speed = 1.0f;

MAKE_HOOK_OFFSETLESS(PlayerController_Update, void, Il2CppObject* self) {
    PlayerController_Update(self);
    Il2CppObject* leftSaber = il2cpp_utils::GetFieldValue(self, "_leftSaber");
    Il2CppObject* rightSaber = il2cpp_utils::GetFieldValue(self, "_rightSaber");

    if(leftSaber != nullptr && rightSaber != nullptr) {
        log(INFO, "Saber's online");
        Il2CppObject* leftSaberTransform = nullptr;
        Il2CppObject* rightSaberTransform = nullptr;

        Il2CppClass* componentsClass = il2cpp_utils::GetClassFromName("", "Saber");
        il2cpp_utils::RunMethod(&leftSaberTransform, leftSaber, il2cpp_functions::class_get_method_from_name(componentsClass, "get_transform", 0));
        il2cpp_utils::RunMethod(&rightSaberTransform, rightSaber, il2cpp_functions::class_get_method_from_name(componentsClass, "get_transform", 0));

        if(leftSaberTransform != nullptr && rightSaberTransform != nullptr) {

            Il2CppClass* transformClass = il2cpp_utils::GetClassFromName("UnityEngine", "Transform");
            const MethodInfo* getMethod = il2cpp_functions::class_get_method_from_name(transformClass, "get_localPosition", 0);
            const MethodInfo* setMethod = il2cpp_functions::class_get_method_from_name(transformClass, "set_localPosition", 1);
            const MethodInfo* setRotate = il2cpp_functions::class_get_method_from_name(transformClass, "Rotate", 1);
            const MethodInfo* setTranslate = il2cpp_functions::class_get_method_from_name(transformClass, "Translate", 1);
            const MethodInfo* getLocalRotation = il2cpp_functions::class_get_method_from_name(transformClass, "get_localRotation", 0);
            const MethodInfo* setLocalRotation = il2cpp_functions::class_get_method_from_name(transformClass, "set_localRotation", 1);

                Vector3 rightSaberLocalPosition;
                il2cpp_utils::RunMethod(&rightSaberLocalPosition, rightSaberTransform, getMethod);
                Quaternion rightSaberLocalRotation;
            il2cpp_utils::RunMethod(&rightSaberLocalRotation, rightSaberTransform, getLocalRotation);

            il2cpp_utils::RunMethod(leftSaberTransform, setRotate, Vector3{ 0, 0, Spin1});
            il2cpp_utils::RunMethod(rightSaberTransform, setRotate, Vector3{ 0, 0, Spin1});
        }
    }
    Spin1 += Speed;
}

__attribute__((constructor)) void lib_main()
{
    log(INFO, "Hello from the first time this mod is loaded!");
}

extern "C" void load() {
    log(INFO, "Hello from il2cpp_init!");
    log(INFO, "Installing hooks...");
    INSTALL_HOOK_OFFSETLESS(PlayerController_Update, il2cpp_utils::FindMethodUnsafe("", "PlayerController", "Update", 0));
    log(INFO, "Installed all hooks!");
}
