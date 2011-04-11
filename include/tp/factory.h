#ifndef TPFACTORY_H
#define TPFACTORY_H

#include <tp/globals.h>
#include <tp/map.h>

/*!

\class tpFactory
\brief simple helper template for abstract factories

This is mostly used internally for all module based 
registries. The usage is rather simple. In order to 
create a factory for nodes of type tpMyNode only one
line of statically initialized instance is necessary.
Please note the usage of the template parameters for 
the base and derived classes:

\code

class tpMyNode : public tpNode {
public:
	tpMyNode() : tpNode("MyNode!") {};
};

static tpFactory<tpNode>::Registry<tpMyNode,200> gs_mynodefactory;

\endcode
*/
template <typename T>
class tpFactory {

	struct Base
	{
		virtual T* _create() = 0;
	};
	
	typedef tpMap< int, Base* > FactoryMap;
	FactoryMap _instantiators;
	
	//! C'tor private make singleton
	tpFactory() {};
	//! copy C'tor private make singleton
	tpFactory(const tpFactory<T>& other) {}
	//! Assignment operator make singleton
	tpFactory& operator = (const tpFactory<T>& rhs) {}
	
public:
	
	//! get an instance of the factory
	static 
	tpFactory<T>& get()
	{
		static tpFactory<T> gs_instance;
		return gs_instance;
	}
	
	//! create an object from type
	T* 
	create(int type_id) 
	{
		typename FactoryMap::iterator mit = _instantiators.find(type_id);
		if (mit != _instantiators.end()) 
		{
			return mit->getValue()->_create();
		} 
		return T(0L);
	}
	
	//! only used internally - adding a new factory
	void add(Base* h, int type_id) { _instantiators.add(type_id,h); }

	//! removing a factory
	void remove(Base* h) 
	{
		for (typename FactoryMap::iterator iter = _instantiators.begin();
			iter != _instantiators.end();
			++iter)
		{
			if ((*iter).getValue() == h) { 
				_instantiators.erase(iter);
				return;
			}
		}
	}
	
	//! get number of factories registered
	int 
	getSize() const { return _instantiators.getSize(); }
	
	//! helper construct for registering implementations
	template <typename N,int type_id>
	struct Registry : Base {
		Registry() { tpFactory<T>::get().add(this,type_id); }
		~Registry() { tpFactory<T>::get().remove(this); }
		
		virtual T* _create() { return new N; }
	};
	
};


#endif
