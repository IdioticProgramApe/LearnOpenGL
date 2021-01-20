#include "Scene.h"

int main()
{
	Scene scene;
	if (scene.launch() != 0)
	{
		return -1;
	}
	return 0;
}