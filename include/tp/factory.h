

#ifndef TPFACTORY_H
#define TPFACTORY_H

#include <tp/globals.h>
#include <tp/map.h>

template <typename T>
class tpFactory {

	struct Base
	{
		virtual T* _create() { return 0L; } 
	};

	typedef typename tpMap< int, Base* > map_type;
	typedef typename map_type::iterator map_type_iterator;
	
	map_type _instantiators;
	
	// make singleton
	tpFactory() {};
	tpFactory(const factory<T>& other) {}
	tpFactory& operator = (const tpFactory<T>& rhs) {}
	
public:
	
	static 
	tpFactory<T>& instance()
	{
		static tpFactory<T> gs_instance;
		return gs_instance;
	}
	
	T* 
	create(int type_id) 
	{
		map_type_iterator mit = _instantiators.find(type_id);
		if (mit != _instantiators.end()) 
		{
			return mit->second->_create();
		} 
		return 0L;
	}
	
	void 
	add(Base* h, int type_id) { _instantiators[type_id] = h; }

	void 
	remove(Base* h) 
	{
		for (map_type_iterator mit = _instantiators.begin(); mit != _instantiators.end(); mit++)
		{
			if (mit->second == h) { _instantiators.erase(mit); return; }
		}
	}
	
	int 
	size() const { return _instantiators.size(); }
	
	// helper construct for registering implementations
	template <typename N,int type_id>
	struct Registry : Base {
		Registry() { tpFactory<T>::instance().add(this,type_id); }
		~Registry() { tpFactory<T>::instance().remove(this); }
		
		T* _create() { return new N; }
	};
	
};


#endif
