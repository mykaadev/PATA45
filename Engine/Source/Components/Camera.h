
#include "../Physics/Point.h"
#include "../Physics/Vector2.h"
#include "SDL.h"

class Camera
{
public:

	static Camera* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new Camera(); }

	void Update(float deltaTime);

	inline SDL_Rect GetViewport() { return m_Viewport; }
	inline Vector2 GetPosition() { return m_Position; }

	inline void SetCameraFollowTarget(bool state) { bCameraFollowsTarget = state; }
	inline void SetCameraTarget(Point* inTarget) { m_Target = inTarget; }


protected:
private:


	Camera();
	~Camera();
	static Camera* m_Instance;

	Vector2 m_Position;
	SDL_Rect m_Viewport;
	Point* m_Target;
	bool bCameraFollowsTarget;

};
