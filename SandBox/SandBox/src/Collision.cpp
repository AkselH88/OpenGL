#include "Collision.h"

Collision::Collision()
{}
Collision::~Collision()
{}

bool fall(float& x, float& y, float& z, float m)
{
	bool insideXR = (x <= 5.3);
	bool iXRm = (x >= 5.3 -m);
	bool insideXL = (x >= -5.3);
	bool iXLm = (x <= -5.3+m);
	bool insideZT = (z <= 7.3);
	bool iZTm = (z >= 7.3-m);
	bool insideZB = (z >= -7.3);
	bool iZBm = (z <= -7.3+m);

	bool outsideXR = (x >= 5.1);
	bool outsideXL = (x <= -5.1);
	bool outsideZT = (z >= 7.1);
	bool outsideZB = (z <= -7.1);

	if (insideZB && insideZT)
	{
		if (insideXL && iXLm)
			x = -5.3f;
		else if (insideXR && iXRm)
			x = 5.3f;
			
	}
	if (insideXR && insideXL)
	{
		if (insideZB && iZBm)
			z = -7.3f;
		else if (insideZT && iZTm)
			z = 7.3f;
	}
	
	if ((outsideXL || outsideXR) || (outsideZB || outsideZT))
	{
		if (y <= -32)
		{
			y = -32.0f;
			return false;
		}
			
		return true;
	}

	
		

	return false;
}