#pragma once

#include <string>

class CSetting
{
public:
	CSetting();
	~CSetting();

	static CSetting& Inst();

	std::string GetLocation()const;
	std::string GetEncode()const;
	std::string GetConfiguration() const;

	void GetLocation(std::wstring& wstr);
	void GetEncode(std::wstring& wstr);
	void GetConfiguration(std::wstring& wstr) ;

	void SetLocation(const std::string& location);
	void SetEncode(const std::string& container);
	void SetConfiguration(const std::string& configuration);

	void SetLocation(const std::wstring& location);
	void SetEncode(const std::wstring& container);
	void SetConfiguration(const std::wstring& configuration);

protected:
	template <typename Writer>
	void Serialize(Writer& writer) const
	{
		writer.StartObject();

		writer.String("location");
		writer.String(location);

		writer.String("container");
		writer.String(container);

		writer.String("configuration");
		writer.String(configuration);

		writer.EndObject();
	}

	void initialize();
	void preload();

	void load();
	void save();

private:
	std::string location;
	std::string container;
	std::string configuration;
};

