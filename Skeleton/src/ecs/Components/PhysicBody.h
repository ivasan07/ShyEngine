#pragma once

#include "Component.h"
#include <box2d/b2_fixture.h>
#include <Vector2D.h>

class b2BodyDef;
class b2Body;
class b2FixtureDef;
class b2World;
class b2Shape;
enum b2BodyType;

using namespace Utilities;

namespace Physics {
    class PhysicsManager;
}

namespace ECS {

    class Transform;

    class PhysicBody : public Component {

    public:

        enum class BODY_TYPE {
            STATIC = 0, KINEMATIC = 1, DYNAMIC = 2
        };

        PhysicBody();

        ~PhysicBody();

        void init() override;

        void start() override;

        void onDestroy() override;

        void fixedUpdate(float fixedDeltaTime) override;

        void onActive() override;

        void onDeacitve() override;

        void onSceneUp() override;

        void onSceneDown() override;


    publish:

        // Setters & getters
        void setTrigger(bool trigger);
        bool isTrigger();

        void setFriction(float friction);
        float getFriction();

        void setBounciness(float bounciness);
        float getBounciness();

    public:

        // Common body methods
        Vector2D getOffSet();
        void addOffSet(float x, float y);

        void setRotationFreezed(bool freezed);
        bool isRotationFreezed();
        
        void setBodyType(int type);
        int getBodyType();
        
        void setLinearDrag(float drag);
        float getLinearDrag();
        
        void setAngularDrag(float drag);
        float getAngularDrag();
                
        void setGravityScale(float scale);
        float getGravityScale();

        virtual void setMass(float mass) = 0;
        float getMass();
        
        virtual float getArea() = 0;
        float getAngle();

        // Collision Filtering
        void setCollisionLayer(const std::string& layerName);

        // Collison/Trigger Stay
        void setCollisionStay(bool stay, Entity* b);
        void setTriggerStay(bool stay, Entity* b);

    publish:
        void setLinearVelocity(float x, float y);
        Vector2D getLinearVelocity();
    public:

        // Force methods

        void setAngularVelocity(float a);
        float getAngularVelocity();

        void applyForce(const Vector2D& force, const Vector2D& point);
        
        void applyForceToCenter(const Vector2D& force);

        void applyTorque(float torque);

        void applyLinearImpulse(const Vector2D& impulse, const Vector2D& point);

        void applyLinearImpulseToCenter(const Vector2D& impulse);

        void applyAngularImpulse(float impulse);

    protected:

        virtual void scaleShape() = 0;

        Vector2D size;
        Vector2D offSet;

        float screenToWorldFactor;

        reflect int bodyType;

        // Box2d
        b2World* world;

        // Fixture properties
        reflect float mass;
        reflect float bounciness;
        reflect bool trigger;   
        reflect bool freezeRotation;

        // Box2d properties
        b2BodyDef* bodyDef;
        b2Body* body;
        b2FixtureDef* fixtureDef;
        b2Fixture* fixture;

        // Entity Components
        Transform* transform;

        Physics::PhysicsManager* pm;

    private:

        Vector2D lastPositionSync;
        float lastRotationSync;
        Vector2D lastScaleInfo;

        // Collision Layer
        std::string layerName;

        // Collison/Trigger Stay
        bool onCollisonStay;
        bool onTriggerStay;

        Entity* b;

    };
}
