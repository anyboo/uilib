#include "stdafx.h"
#include "Setting.h"
#include <fstream>

using namespace rapidjson;

Config::Config()
:tmp(_T("A"))
{
	TCHAR PATH[MAX_PATH] = { 0 };
	STDSTRING AppPath = STDSTRING(PATH, ::GetModuleFileName(NULL, PATH, MAX_PATH));
	WriteablePath = AppPath.substr(0, AppPath.find_last_of(_T('\\')) + 1);
	AppLetter = AppPath.substr(0, AppPath.find_first_of(_T('\\')));
	configfile = WriteablePath + _T("config.json");
}

Config::~Config()
{

}

void Config::LoadConfig(vector<LayOut_Info>& AllLyt, CPaintManagerUI& m_pm)
{
	ifstream ifs(configfile);
	IStreamWrapper isw(ifs);
	Document d;
	d.ParseStream(isw);
	size_t file_size = isw.Tell();
	if (isw.Tell() == 0)
		return;

	typedef Value::ConstMemberIterator Iter;
	for (Iter it = d.MemberBegin(); it != d.MemberEnd(); it++)
	{
		STDSTRING TypeName = it->name.GetString();
		const Value& a = d[TypeName.c_str()];
		assert(a.IsArray());
		if (!a.IsArray() || a.Size() < 5)
			continue;
		STDSTRING letter = a[1].GetString();
		STDSTRING relPath = a[2].GetString();
		STDSTRING display = a[4].GetString();

		display.empty() ? (display = TypeName) : display;

		STDSTRING str = letter + relPath + TypeName;

		m_MyHandle.GetIcon(str, AllLyt, m_pm);
	}
}

void Config::SaveToConfig(vector<LayOut_Info>& AllLyt)
{
	Document document;

	document.Parse(configfile.c_str());
	ofstream ofs(configfile);
	OStreamWrapper osw(ofs);
	Document::AllocatorType& alloc = document.GetAllocator();
	Value root(kObjectType);

	for (auto& var : AllLyt)
	{
		STDSTRING absPath, relPath, name, extension, path, letter, display;
		path = var.FilePath;
		display = var.Display;

		name = path.substr(path.find_last_of(_T('\\')) + 1);
		absPath = path.substr(0, path.find_last_of(_T('\\')) + 1);
		extension = name.substr(name.find_last_of(_T('.')) + 1);
		relPath = absPath.substr(absPath.find_first_of(_T(':')) + 1);
		letter = absPath.substr(0, absPath.find_first_of(_T(':')) + 1);

		Value a(kArrayType);
		Value n(name.c_str(), name.length(), alloc);
		Value r(relPath.c_str(), relPath.length(), alloc);
		Value p(absPath.c_str(), absPath.length(), alloc);
		Value e(extension.c_str(), extension.length(), alloc);
		Value l(letter.c_str(), letter.length(), alloc);
		Value d(display.c_str(), display.length(), alloc);

		a.PushBack(p, alloc).PushBack(l, alloc).PushBack(r, alloc).PushBack(e, alloc).PushBack(d, alloc);
		root.AddMember(n.Move(), a.Move(), alloc);
	}

	Writer<OStreamWrapper> writer(osw);
	root.Accept(writer);

}





