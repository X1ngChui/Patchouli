#include "Patchouli.h"
#include <iostream>

class SObject : public Patchouli::PObject
{
public:
	SObject()
	{
		std::cout << "Create PObject" << std::endl;
	}

	virtual ~SObject()
	{
		std::cout << "Destroy PObject" << std::endl;
	}
};

int main()
{
	Patchouli::WeakRef<SObject> wref;
	{
		Patchouli::Ref<SObject> ref = Patchouli::makeRef<SObject>();
		wref = ref;
		std::cout << (wref.expired() ? "expired" : "not expired") << std::endl;
	}
	std::cout << (wref.expired() ? "expired" : "not expired") << std::endl;
}