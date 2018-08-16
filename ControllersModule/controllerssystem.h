#ifndef CONTROLLERSSYSTEM_H
#define CONTROLLERSSYSTEM_H

#include <SharedModule/internal.hpp>

class ControllerBase;
class ControllersContainer;

class _Export ControllersSystem
{
    ControllersSystem();
public:    
    static ControllerBase* GetController(const Name& name);
    template<class T>
    static T* GetController(const Name &name) {
        return (T*)GetController(name);
    }

private:
    friend class ControllerBase;

    static void registerController(const Name& name, ControllerBase* controller);
};

#endif // CONTROLLERSSYSTEM_H
