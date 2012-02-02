
#include <tp/array.h>
#include <tp/mat.h>
#include <tp/log.h>
#include <tp/logutils.h>


// boilerplate
#include <gtest/gtest.h>
using ::testing::InitGoogleTest;

// tests
TEST(tpArray, negative) {
	tpArray<int> t;
	EXPECT_FALSE(!t.isEmpty());
}

TEST(tpArray, positive) {
	tpArray<int> t;
	EXPECT_TRUE(t.isEmpty());
}

TEST(tpMat, positive) {

	tpMat<4,4,float> m;
	m.fill(0);

	EXPECT_EQ(0,m.at(0));

}

TEST(tpVec, create) {
	tpVec4f v(1,2,3,4);
	EXPECT_EQ(1,v[0]);
	EXPECT_EQ(2,v[1]);
	EXPECT_EQ(3,v[2]);
	EXPECT_EQ(4,v[3]);

//	tpVec4f v2(v - v);
//	EXPECT_EQ(2,v2[0]);
//	EXPECT_EQ(4,v2[1]);
//	EXPECT_EQ(6,v2[2]);
//	EXPECT_EQ(8,v2[3]);

}

TEST(tpMat, mul) {

	tpMat<4,4,float> a = tpMat<4,4,float>::Identity();
	tpMat<4,4,float> b = tpMat<4,4,float>::Identity();
	tpMat<4,4,float> c = tpMat<4,4,float>::Identity();

	tpMat<4,4,float> abc = a * b * c;

	EXPECT_EQ(1,abc(0,0));
	EXPECT_EQ(1,abc(1,1));
	EXPECT_EQ(1,abc(2,2));
	EXPECT_EQ(1,abc(3,3));
	EXPECT_EQ(0,abc(1,0));
	EXPECT_EQ(0,abc(1,0));
	EXPECT_EQ(0,abc(2,0));
	EXPECT_EQ(0,abc(0,2));

	tpLog::get() << abc;

}

// run
int main(int argc, char* argv[])
{
	InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
