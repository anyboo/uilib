#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <QApplication>
#include <QLibrary>
#include "IVideoServer.h"

IVideoServerFactory* getBlueSkyFactory()
{
	pfGetVideoServerFactory GetVideoServerFactory = nullptr;
	QLibrary hdll2(QApplication::applicationDirPath());
	QLibrary hdll(QApplication::applicationDirPath() + "/../../factorys/BlueSky/BlueSkyVideoServer.dll");
	GetVideoServerFactory = (pfGetVideoServerFactory)hdll.resolve("VideoServerFactory");
	if (GetVideoServerFactory != nullptr)
	{
		IVideoServerFactory* pFactory = GetVideoServerFactory();
		return pFactory;
	}
	return NULL;
}

TEST_CASE("BlueSky", "[factorial]") {
	IVideoServerFactory* BlueSky = getBlueSkyFactory();
	SECTION("init")
	{
//		REQUIRE(a->init() == true);
		CHECK(BlueSky->init());
	}

	SECTION("name")
	{
		REQUIRE(strcmp(BlueSky->name(), "蓝色星际") == 0);
	}

	SECTION("defaultPort")
	{
		REQUIRE(BlueSky->defaultPort() == 3721);
	}

	SECTION("defaultUser")
	{
		REQUIRE(strcmp(BlueSky->defaultUser(), "admin") == 0);
	}

	SECTION("defaultPasswords")
	{
		REQUIRE(strcmp(BlueSky->defaultPasswords(), "123456") == 0);
	}

	SECTION("factory")
	{
		REQUIRE(BlueSky->factory() == SISC_IPC_BLUESKY);
	}

	SECTION("create")
	{
		CHECK(BlueSky->create());
	}
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	int iRet = Catch::Session().run(argc, argv);
	return iRet;
}
