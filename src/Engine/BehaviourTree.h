


// class Behaviour
// {
// public:
//     Behaviour();
//     virtual void Update(const float aDeltaTime);
//     virtual void DoAction();
// };

// class AttackBehaviour : public Behaviour
// {
// public:
//     void Update(const float aDeltaTime) override
//     {
//         RunToEnemy();
//         if(Close())
//             Attack();
//     }

// };

// class DefensiveBehaviour : public Behaviour
// {
// public:
//     void Update(const float aDeltaTime) override
//     {
//         RunFromEnemy();
//     }
// };

// class BehaviourTree
// {
// public:
//     void SetBehaviour(Behaviour aBehaviour) 
//     {
//         m_ActiveBehaviour = aBehaviour;
//     }

//     void Update() 
//     {
//         m_ActiveBehaviour.Update();
//     }
// };