#include <tp/traverser.h>
#include <tp/node.h>
#include <tp/transform.h>
#include <tp/log.h>
#include <tp/stringformater.h>



struct tpMatTraverser : tpTraverser {


	unsigned char getDirection() const { return kUpward; }

	void enter(tpNode* node)
	{
		tpLog::get().printf("<node name='%s'>\n",node->getName().c_str());

		tpTransform* aT = node->getType()->self_cast<tpTransform>(node);

		if (aT)
		{
			tpMat44r m; m.identity();
			aT->getMatrix(true,m);

			tpString ms;
			ms << m;

			tpLog::get().printf(ms.c_str());


		}
	}

	void leave(tpNode* node)
	{
		tpLog::get().printf("</node> <!-- '%s'-->\n\n",node->getName().c_str());

		if (node->getParentsCount() == 0) {
			tpLog::get().printf("ROOOT!!");
		}
	}

};

struct tpPathCollector : tpTraverser {

	tpNodeArray nodepath;
	tpNodeArrayList nodepaths;

	unsigned char getDirection() const { return kUpward; }

	void enter(tpNode* node)
	{
		nodepath.add(node);
	}

	void leave(tpNode* node)
	{
		if (node->getParentsCount() == 0)
		{
			nodepaths.add(nodepath);

			printf("Np %d\n",nodepath.getSize());
			printf("Np %d\n",nodepaths.back().getSize());

			nodepath.clear();
		}
	}

	void report()
	{
		for (tpNodeArrayList::iterator i = nodepaths.begin();
			 i != nodepaths.end();
			 ++i)
		{
			for (tpNodeArray::iterator j = (*i).begin();
				 j != (*i).end();
				 ++j)
			{
				printf("%s, ",(*j)->getName().c_str());
			}
			printf("\n");
		}
	}

};


int main(int argc, char* argv[])
{
	tpRefPtr<tpNode> leaf = new tpNode(); leaf->setName("Leaf");
	tpRefPtr<tpNode> root = new tpNode(); root->setName("Root");

	tpRefPtr<tpTransform> tfn_1 = new tpTransform(); tfn_1->setName("Transform 1");
	tpRefPtr<tpTransform> tfn_2 = new tpTransform(); tfn_2->setName("Transform 2");

	tpMat44r m;
	m.setTranslation(1,2,3);
	tfn_1->setMatrix(m);
	m.setTranslation(4,5,6);
	tfn_2->setMatrix(m);


	root->addChild(tfn_1.get());
	tfn_1->addChild(tfn_2.get());

	tfn_1->addChild(leaf.get());
	tfn_2->addChild(leaf.get());


	tpMatTraverser mt;

	//root->traverse(mt);

	leaf->traverse(mt);

	tpPathCollector pc;
	leaf->traverse(pc);

	pc.report();


	return 0;
}
